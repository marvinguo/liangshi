#pragma once

#include "Qt.h"

class HttpFileDownloader : public QObject
{
	Q_OBJECT
public:
	HttpFileDownloader(QString remote, QString local);
	~HttpFileDownloader();
	
	void Download();

Q_SIGNALS:
	void DownloadFinished(QString localfile);

private Q_SLOTS:
	void DownloadedTriggered(QNetworkReply* pReply);

private:
	QNetworkAccessManager _access;
	QString _remote;
	QString _local;
};

