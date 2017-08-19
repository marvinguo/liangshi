#pragma once

#include "../common/Qt.h"
#include "../common/YunHttp.h"

class UserAgentPage : public QWebPage
{
	Q_OBJECT
public:
	UserAgentPage(QObject *parent);
	QString userAgentForUrl ( const QUrl & url ) const;
};

