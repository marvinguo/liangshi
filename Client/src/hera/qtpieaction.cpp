#include "qtpiemenu.h"
#include "qtpieaction.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

QtPieAction::QtPieAction(const QString &text, QVariant& data, QObject *receiver, const char *member):
QtPieItem(text),
_extraData(data)
{
	SetType(Action);
	connect(this, SIGNAL(activated()), receiver, member);
}

QtPieAction::QtPieAction(const QString &text, QObject *receiver, const char *member):
QtPieItem(text)
{
	SetType(Action);
	connect(this, SIGNAL(activated()), receiver, member);
}

QtPieAction::QtPieAction(const QIcon &icons, const QString &text, QObject *receiver, const char *member):
QtPieItem(icons, text)
{
	SetType(Action);
	connect(this, SIGNAL(activated()), receiver, member);
}

void QtPieAction::Activate()
{
	Q_EMIT activated();
}

