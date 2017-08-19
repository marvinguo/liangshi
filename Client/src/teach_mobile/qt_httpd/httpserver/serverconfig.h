#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H
#include <QHostAddress>
#include <QMap>
#include <QString>
class QSettings;
class QFileSystemWatcher;

typedef void (*LPFNPARSECONFIG)(QSettings * conf);
void registerConfigParser(LPFNPARSECONFIG lpfn);
class ConfigParserRegisterHelper
{
public:
    ConfigParserRegisterHelper(LPFNPARSECONFIG lpfn)
    {
        registerConfigParser(lpfn);
    }
};
#define REGISTER_CONFIG_PARSER(fn) \
    static ConfigParserRegisterHelper g_parserRegister(fn)

class HttpServerConfig : public QObject
{
    Q_OBJECT
private:
    HttpServerConfig();
public:
    ~HttpServerConfig();

    static HttpServerConfig & instance();
    void loadConfig(const char * config);

    QHostAddress m_address;
    quint16 m_port;     ///< port
    quint16 m_threads;  ///< worker threads
    int m_maxConnections;  ///< concurrent connections
    QString m_strDocumentRoot; ///< document root
    QStringList m_forbiddenPatters;///< foridden access path in URI
    QMap<QString, QString> m_alias;

protected slots:
    void onConfigFileChanged(const QString &path);

private:
    void parseConfig(const QString &config);
    void parseForbiddenPatters(const QString &fileName);
    void parseModuleConfig(QSettings *settings);

private:
    QFileSystemWatcher *m_fileWatcher;
};

#endif // SERVERCONFIG_H
