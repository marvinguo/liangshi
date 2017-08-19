#include "serverconfig.h"
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include "utils/asyncLog.h"
#include "utils/log.h"
#include <QFileSystemWatcher>

static LPFNPARSECONFIG * g_configParser = NULL;
static int g_parserCapacity = 0;
static int g_parserSize = 0;
#define PARSER_INIT_SIZE 8
#define PARSER_INCREMENT 4

void registerConfigParser(LPFNPARSECONFIG lpfn)
{
    if(g_configParser == NULL)
    {
        g_configParser = (LPFNPARSECONFIG*)calloc(PARSER_INIT_SIZE, sizeof(LPFNPARSECONFIG));
        g_parserCapacity = PARSER_INIT_SIZE;
        g_parserSize = 0;
    }

    if(g_parserSize == g_parserCapacity)
    {
        g_parserCapacity += PARSER_INCREMENT;
        g_configParser = (LPFNPARSECONFIG*)realloc(g_configParser, sizeof(LPFNPARSECONFIG)*g_parserCapacity);
    }

    g_configParser[g_parserSize++] = lpfn;
}


HttpServerConfig::HttpServerConfig()
    : m_address(QHostAddress::Any)
    , m_port(8080)
    , m_threads(4)
    , m_strDocumentRoot(".")
{
    m_fileWatcher = new QFileSystemWatcher(this);
    connect(m_fileWatcher, SIGNAL(fileChanged(QString)),
            this, SLOT(onConfigFileChanged(QString)));
}

HttpServerConfig::~HttpServerConfig()
{
}

HttpServerConfig & HttpServerConfig::instance()
{
    static HttpServerConfig config;
    return config;
}

void HttpServerConfig::loadConfig(const char *config)
{
    if(!config || !QFile::exists(config))
    {
        config = "httpserver.conf";
        if(!QFile::exists(config))
        {
            aWarn()<<"HttpServerConfig::loadConfig failed: no "<< config << endl;
            return ;
        }
    }

    m_fileWatcher->addPath(config);
    parseConfig(config);
}

void HttpServerConfig::onConfigFileChanged(const QString &path)
{
    aInfo() << "config changed " << path <<", reparse module options" << endl;
    QSettings conf(path, QSettings::IniFormat);
    parseModuleConfig(&conf);
}

void HttpServerConfig::parseConfig(const QString &config)
{
    QSettings conf(config, QSettings::IniFormat);
    conf.beginGroup("server");
    QString strHost = conf.value("host", "").toString();
    if(!strHost.isEmpty() && !m_address.setAddress(strHost))
    {
        m_address = QHostAddress::LocalHost;
    }
    m_port = conf.value("port", 8088).toUInt();
    m_threads = conf.value("worker_threads", 4).toUInt();
    m_maxConnections = conf.value("connections", 200).toInt();
    m_strDocumentRoot = conf.value("document_root", ".").toString();
    QString strAliases = conf.value("alias").toString();
    if(!strAliases.isEmpty())
    {
        QStringList aliasPairs = strAliases.split(";;");
        foreach(QString pair, aliasPairs)
        {
            int sep = pair.indexOf(',');
            if(sep != -1)
            {
                m_alias.insert(pair.left(sep), pair.mid(sep+1));
            }
        }
    }
    QString strForbiddenPatterFile = conf.value("forbidden").toString();
    if(!strForbiddenPatterFile.isEmpty())
    {
        parseForbiddenPatters(strForbiddenPatterFile);
    }
    conf.endGroup();

    parseModuleConfig(&conf);
}

void HttpServerConfig::parseForbiddenPatters(const QString &fileName)
{
    QFile file(fileName);
    if(file.open(QFile::ReadOnly))
    {
        QTextStream ts(&file);
        while(!ts.atEnd())
        {
            m_forbiddenPatters.append(ts.readLine());
        }
    }
}

void HttpServerConfig::parseModuleConfig(QSettings *settings)
{
    for(int i = 0; i < g_parserSize; i++)
    {
        if(g_configParser[i])
        {
            g_configParser[i](settings);
        }
    }
}
