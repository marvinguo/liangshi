#include "networkAccessHelper.h"
#include <QFile>

QString responseString(QNetworkReply *reply)
{
    QString joinedHeader;
    QVariant variant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(variant.isValid())
    {
        joinedHeader = QString("STATUS: %1\n").arg(variant.toInt());
    }

    QList<QByteArray> headerList = reply->rawHeaderList();
    int count = headerList.size();
    QByteArray value;
    for(int i = 0; i < count; i++)
    {
        const QByteArray &tag = headerList.at(i);
        value = reply->rawHeader(tag);
        joinedHeader += QString("%1: %2\n").arg(tag.data()).arg(value.data());
    }

    return joinedHeader;
}

void dumpToFile(const QString &fileName, QByteArray &data)
{
    QString timedFileName = QString("%1_%2")
            .arg( QDateTime::currentDateTime().toString("yyyyMMdd_HH-mm-ss-zzz"))
            .arg(fileName);

    QFile file(timedFileName);
    if(file.open(QFile::WriteOnly))
    {
        file.write(data);
        file.flush();
        file.close();
    }
}
