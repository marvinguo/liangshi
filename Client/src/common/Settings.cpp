#include "Settings.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

Settings::Settings()
{
	QString path = "";
#if defined(Q_OS_WIN32)
	path = QCoreApplication::applicationDirPath() + "/settings.ini";
#elif defined(Q_OS_ANDROID)
	path = "assets:/settings.ini";
#endif
	qDebug() << "settings.ini path:" << path;
	_ini = new QSettings(path, QSettings::IniFormat);
}

Settings::~Settings()
{
	delete _ini;
}

//configIniWrite->setValue("/ip/first", "192.168.0.1");  

QString Settings::GetHomeUrl()
{
	return _ini->value("/basic/homeurl").toString();
}

QString Settings::GetP2PUrl()
{
	return _ini->value("/basic/p2purl").toString();
}