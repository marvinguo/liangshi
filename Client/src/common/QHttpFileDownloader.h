#pragma once

#include "Qt.h"
#include "qhttp.h"

//对于verify,还点取cookie
class QHttpFileDownloader : public QObject
{
	Q_OBJECT
public:
	QHttpFileDownloader(QString host, QString path, QString local);
	~QHttpFileDownloader();
	
	void Download();
	QString GetCookie();
	void SetData(const QVariant& value){_value = value;}
	QVariant GetData(){return _value;}

Q_SIGNALS:
	void DownloadFinished(QString localfile);
	void HasError();

private Q_SLOTS:
	void DownloadedTriggered(bool hasError);

private:
	QHttp _http;

	QString _remote;
	QString _local;

	QVariant _value;
};

