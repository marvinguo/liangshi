#ifndef NETWORKACCESSHELPER_H
#define NETWORKACCESSHELPER_H

#include <QNetworkReply>
#include <QString>

QString responseString(QNetworkReply *reply);
void dumpToFile(const QString &fileName, QByteArray &data);

#endif // NETWORKACCESSHELPER_H
