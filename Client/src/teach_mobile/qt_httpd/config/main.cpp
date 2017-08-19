#include <QCoreApplication>
#include <QSettings>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    {
        QSettings settings("config.conf", QSettings::IniFormat);
        settings.beginGroup("server");
        settings.setValue("port", 80);
        settings.setValue("threads", 4);
        settings.setValue("log_level", "debug");
        settings.endGroup();

        settings.setValue("server/max_connection", 1024);

        qDebug() << "create configuration end" << endl;
    }

    {
        QSettings conf("config.conf", QSettings::IniFormat);
        qDebug() << "server port: "
                 << conf.value("server/port").toInt();
        conf.beginGroup("server");
        qDebug() << "server threads: " << conf.value("threads").toInt();
        qDebug() << "server log level: " << conf.value("log_level").toString();
        conf.endGroup();
    }


    return a.exec();
}
