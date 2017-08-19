#ifndef RESPONDERMANAGER_H
#define RESPONDERMANAGER_H

#include "responderInterface.h"
#include "../serverconfig.h"

class HttpServer;
class HttpRequest;
class HttpResponse;

typedef ResponderInterface * (*LPFNCREATERESPONDER)(HttpRequest * req, HttpResponse * resp);

class ResponderCreatorFactory
{
    ResponderCreatorFactory(const ResponderCreatorFactory &);
    ResponderCreatorFactory & operator=(const ResponderCreatorFactory &);
    ResponderCreatorFactory();

public:
    ~ResponderCreatorFactory();

    static ResponderCreatorFactory * instance();
    static void destroy();
    LPFNCREATERESPONDER responderCreator(const char * szKeyword);

    bool regist(const char * szKeyword, LPFNCREATERESPONDER lpfnCreator);

private:
    const char ** m_keywords;
    LPFNCREATERESPONDER * m_functions;
    int m_iCount;
    int m_iCursor;
};

class ResponderCreatorRegisterHelper
{
public:
    ResponderCreatorRegisterHelper(const char * szName, LPFNCREATERESPONDER lpfnCreator)
    {
        ResponderCreatorFactory::instance()->regist(szName, lpfnCreator);
    }
};

#define REGISTER_RESPONDER_CREATOR(TAG, szName,lpfnCreator) \
    static ResponderCreatorRegisterHelper g_registerHelper##TAG(szName, lpfnCreator)


class ResponderManagerPrivate;
class ResponderManager : public QObject
{
    Q_OBJECT
public:
    ResponderManager(QObject * parent = 0);
    ResponderManager(const ResponderManager & rhs);
    ResponderManager & operator=(const ResponderManager & rhs);
    ~ResponderManager();

    void handleRequest(HttpRequest * req, HttpResponse * resp);

signals:
    void oneResponderDestroyed();

protected slots:
    void onResponderDestroyed();

protected:
    void timerEvent(QTimerEvent *e);

private:
    ResponderManagerPrivate * m_d;
};

#endif // RESPONDERMANAGER_H
