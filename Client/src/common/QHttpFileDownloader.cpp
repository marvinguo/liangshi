#include "QHttpFileDownloader.h"

QHttpFileDownloader::QHttpFileDownloader(QString host, QString path, QString local):
_remote(path),
_local(local)
{
	qDebug() << "begin HttpFileDownloader::HttpFileDownloader";
//	_access = new QNetworkAccessManager(this);
//	_access->setCookieJar(&_cookieJar);
	connect(&_http, &QHttp::done, this, &QHttpFileDownloader::DownloadedTriggered);
	_http.setHost(host);
	qDebug() << "end HttpFileDownloader::HttpFileDownloader";
}

QHttpFileDownloader::~QHttpFileDownloader()
{
//	_access->deleteLater();
}

void QHttpFileDownloader::Download()
{
	qDebug() << "begin HttpFileDownloader::Download";
	qDebug() << "remote:" << _remote;
	QUrl url(_remote);
	qDebug() << "url:" << url;
	QNetworkRequest request(url);
	qDebug() << "create request";
	_http.get(_remote);
	qDebug() << "end HttpFileDownloader::Download";
}

void QHttpFileDownloader::DownloadedTriggered(bool hasError)
{
	if(!hasError)
	{
		QFile file(_local);
		file.open(QIODevice::WriteOnly);
		file.write(_http.readAll());
		file.flush();
		file.close();
		Q_EMIT DownloadFinished(_local);
	}
	else
		Q_EMIT HasError();
}

QString QHttpFileDownloader::GetCookie()
{
	QString cookie = _http.lastResponse().value("set-cookie");
	return cookie;
}