#include "responderManager.h"
#include <QMap>
#include <QString>
#include <QTimerEvent>
#include <malloc.h>
#include <stdio.h>
#include "httpRequest.h"
#include "httpResponse.h"
#include "responder.h"
#include "httpserver.h"
#include <QTime>
#include "../utils/asyncLog.h"
#include "globalInit.h"
//
// responder factory
//

#define INITIALISE_SIZE    32
#define INCREMENT_SIZE     8
static ResponderCreatorFactory * s_instance = 0;
ResponderCreatorFactory::ResponderCreatorFactory()
    : m_keywords(0)
    , m_functions(0)
    , m_iCount(0)
    , m_iCursor(0)
{
}

ResponderCreatorFactory::~ResponderCreatorFactory()
{
    //TODO: release functions && keywords memory
}

ResponderCreatorFactory * ResponderCreatorFactory::instance()
{
    if(!s_instance)
    {
        s_instance = new ResponderCreatorFactory;
        aDebug() << "ResponderCreatorFactory initialised" << endl;
    }

    return s_instance;
}

void ResponderCreatorFactory::destroy()
{
    if(s_instance)
    {
        delete s_instance;
        s_instance = 0;
    }
}

bool ResponderCreatorFactory::regist(const char * szKeyword, LPFNCREATERESPONDER lpfnCreate)
{
    if(!szKeyword || !lpfnCreate)
    {
        aDebug() << "ResponderCreatorFactory::regist, invalid parameters" << endl;
        return false;
    }

    //repeat check
    for(int i = 0; i < m_iCursor; ++i)
    {
        if(!strcmp(m_keywords[i], szKeyword))
        {
            aDebug() << "ResponderCreatorFactory::regist, repeated responder creator" << endl;
            return false;
        }
    }

    if(!m_functions)
    {
        m_functions = (LPFNCREATERESPONDER*)calloc(INITIALISE_SIZE, sizeof(LPFNCREATERESPONDER));
        m_keywords = (const char**)calloc(INITIALISE_SIZE, sizeof(char*));
        m_iCount = INITIALISE_SIZE;
        m_iCursor = 0;
    }
    else if( m_iCursor == m_iCount )
    {
        m_iCount += INCREMENT_SIZE;
        m_functions = (LPFNCREATERESPONDER*)realloc( m_functions, m_iCount * sizeof(LPFNCREATERESPONDER) );
        m_keywords = (const char**)realloc( m_keywords, m_iCount * sizeof(char*));
    }

    m_keywords[m_iCursor] = (const char *)strdup(szKeyword);
    m_functions[m_iCursor] = lpfnCreate;

    m_iCursor++;
    aDebug() << "register create function for - " << szKeyword << endl;
    return true;
}

LPFNCREATERESPONDER ResponderCreatorFactory::responderCreator(const char * szKeyword)
{
    for(int i = 0; i < m_iCursor; ++i)
    {
        if(!strcmp(m_keywords[i], szKeyword))
        {
            //aDebug() << "find responder create function for - " << szKeyword << endl;
            return m_functions[i];
        }
    }
    //aDebug() << "no responder create function for - " << szKeyword << endl;
    return 0;
}

static void* responderFactoryInitializer()
{
    return ResponderCreatorFactory::instance();
}
REGISTER_GLOBAL_INITIALIZER("ResponderCreator", responderFactoryInitializer);

//
// ResponderManager
//
class ResponderManagerPrivate
{
public:
    ResponderManagerPrivate(ResponderManager * m)
        : m_config(HttpServerConfig::instance())
        , m_manager(m)
        , m_handleTimer(0)
        , m_factory(ResponderCreatorFactory::instance())
    {
    }

    ~ResponderManagerPrivate()
    {
    }

    /// @return 0 - handle end; 1 - should continue handle
    int handle()
    {
        stopScheduleTimer();
        QTime t;
        t.start();

        int iShouldContinue = 0;
        int count = m_responders.size();
        for(int i = 0; i < count; i++)
        {
            if( m_responders.at(i)->handle() == 1)
            {
                if(!iShouldContinue)
                {
                    iShouldContinue = 1;
                }
            }
        }

        if(iShouldContinue)
        {
            int useTime = t.elapsed();
            startScheduleTimer(useTime >= SERVER_POLLING_INTERVAL ? 0 : SERVER_POLLING_INTERVAL - useTime);
        }

        return iShouldContinue;
    }

    void startScheduleTimer(int interval = 3)
    {
        m_handleTimer = m_manager->startTimer(interval, Qt::PreciseTimer);
    }

    void stopScheduleTimer()
    {
        if(m_handleTimer > 0)
        {
            m_manager->killTimer(m_handleTimer);
            m_handleTimer = 0;
        }
    }

    HttpServerConfig & m_config;
    ResponderManager * m_manager;
    QList<ResponderInterface*> m_responders;
    int m_handleTimer;
    ResponderCreatorFactory * m_factory;
};

ResponderManager::ResponderManager(QObject *parent)
    : QObject(parent)
    , m_d(new ResponderManagerPrivate(this))
{
}

ResponderManager::~ResponderManager()
{
    delete m_d;
}

void ResponderManager::handleRequest(HttpRequest *req, HttpResponse *resp)
{
    if(m_d->m_responders.size() > m_d->m_config.m_maxConnections)
    {
        qWarning("too many connections, refused. max is %d", m_d->m_config.m_maxConnections);
        return;
    }

    QString path = req->path();

    LPFNCREATERESPONDER lpfnCreator = m_d->m_factory->responderCreator(path.toLatin1().data());
    if(!lpfnCreator)
    {
        int splash = path.lastIndexOf('/');
        if(splash > 0)
        {
            path = path.left(splash);
            lpfnCreator = m_d->m_factory->responderCreator(path.toLatin1().data());
        }
    }
    ResponderInterface * responder = NULL;
    if(lpfnCreator)
    {
        responder = lpfnCreator(req, resp);
    }
    else
    {
        responder = new Responder(req, resp);
    }

    connect(responder, SIGNAL(destroyed()), this, SIGNAL(oneResponderDestroyed()));
    if(responder->runAlone())
    {
        //do nothing
    }
    else
    {
        connect(responder, SIGNAL(destroyed()), this, SLOT(onResponderDestroyed())) ;
        m_d->m_responders.push_back(responder);

        if(m_d->m_handleTimer <= 0)
        {
            m_d->handle();
        }
        else
        {
            /// handle when handle timer occur
        }
    }
}

void ResponderManager::onResponderDestroyed()
{
    ResponderInterface * ri = (ResponderInterface*)sender();
    m_d->m_responders.removeOne(ri);
}

void ResponderManager::timerEvent(QTimerEvent *e)
{
    if(e->timerId() == m_d->m_handleTimer)
    {
        m_d->handle();
    }
}

