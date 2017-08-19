#include "UserAgentPage.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

UserAgentPage::UserAgentPage(QObject *parent):
QWebPage(parent)
{
}


QString UserAgentPage::userAgentForUrl ( const QUrl & url ) const
{
	return "Liangshi Browser";
}