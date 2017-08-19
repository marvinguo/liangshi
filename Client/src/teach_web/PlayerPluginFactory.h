#pragma once

#include "../common/Qt.h"

class PlayerPluginFactory : public QWebPluginFactory
{
	Q_OBJECT
public:
	PlayerPluginFactory(QWidget *parent = 0);
	~PlayerPluginFactory();

public:
	virtual QObject *create(const QString &mimeType, const QUrl &url, const QStringList &argumentNames, const QStringList &argumentValues) const;
	virtual QList<QWebPluginFactory::Plugin> plugins() const;

private:
	QNetworkAccessManager* _access;
};

