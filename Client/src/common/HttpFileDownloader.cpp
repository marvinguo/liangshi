#include "HttpFileDownloader.h"

HttpFileDownloader::HttpFileDownloader(QString remote, QString local):
_remote(remote),
_local(local)
{
	connect(&_access, &QNetworkAccessManager::finished, this, &HttpFileDownloader::DownloadedTriggered);
}

HttpFileDownloader::~HttpFileDownloader()
{
}

void HttpFileDownloader::Download()
{
	QUrl url(_remote);
	QNetworkRequest request(url);
	_access.get(request);
}

void HttpFileDownloader::DownloadedTriggered(QNetworkReply* reply)
{
	QFile file(_local);
	file.open(QIODevice::WriteOnly);
	file.write(reply->readAll());
	file.flush();
	file.close();
	reply->deleteLater();
	Q_EMIT DownloadFinished(_local);
}
