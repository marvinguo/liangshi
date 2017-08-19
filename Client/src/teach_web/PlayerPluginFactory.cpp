#include "PlayerPluginFactory.h"
#include "PlayerPlugin.h"
#include "../common/FileUtil.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

PlayerPluginFactory::PlayerPluginFactory(QWidget *parent):
QWebPluginFactory(parent),
_access(new QNetworkAccessManager(this))
{
}

PlayerPluginFactory::~PlayerPluginFactory()
{
}

QObject* PlayerPluginFactory::create(const QString &mimeType, const QUrl &url, const QStringList &argumentNames, const QStringList &argumentValues) const
{
	if (mimeType != "text/ke")
		return 0;

	QString filename = url.fileName();
	QString cacheFile = FileUtil::GetKeCacheFolder() + filename;
	PlayerPlugin* plugin = new PlayerPlugin(cacheFile);
	if(!QFile::exists(cacheFile))
	{
		QNetworkRequest request(url);
		QNetworkReply* reply = _access->get(request);
		connect(reply, &QNetworkReply::finished, plugin, &PlayerPlugin::OnFinishDownload);
		connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
	}
	else
	{
		plugin->LoadCacheKe();
	}

	return plugin;
}

QList<PlayerPluginFactory::Plugin> PlayerPluginFactory::plugins() const
{
	QWebPluginFactory::MimeType mimeType;
	mimeType.name = "text/ke";
	mimeType.description = "Ke file";
	mimeType.fileExtensions = QStringList() << "ke";

	QWebPluginFactory::Plugin plugin;
	plugin.name = "Ke File player";
	plugin.description = "Ke File player";
	plugin.mimeTypes = QList<MimeType>() << mimeType;

	return QList<QWebPluginFactory::Plugin>() << plugin;
}