#include "qtpieitem.h"
#include "qtpiemenu.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

QtPieItem::QtPieItem(const QString &title, unsigned int weight, QWidget *parent):
QWidget(parent, Qt::Popup),
_text(title),
_isEnable(true),
_weight(weight)
{
	SetType(Invalid);
	SetText(title);
}

QtPieItem::QtPieItem(const QIcon &icon, const QString &text, unsigned int weight, QWidget *parent):
QWidget(parent, Qt::Popup),
_icon(icon),
_isEnable(true),
_weight(weight)
{
	SetType(Invalid);
	SetText(text);
}

QtPieItem::~QtPieItem()
{
}

void QtPieItem::SetText(const QString &text)
{
	_text = text;
	if (text.length() > _text.replace(QRegExp(QLatin1String("<[^>/]*br[^>]*>")), " ").length() || 
		text.length() > _text.replace(QRegExp(QLatin1String("<[^>/]*p[^>]*>")), " ").length() || 
		text.length() > _text.remove(QRegExp(QLatin1String("<[^>]+>"))).length())
	{
		qWarning("QtPieItem::setText: HTML support not implemented until Qt 4.1. All tags removed from text.");
	}
}

QString QtPieItem::GetText() const
{
	return _text;
}

void QtPieItem::SetIcon(const QIcon &icon)
{
	_icon = icon;
}

QIcon QtPieItem::GetIcon() const
{
	return _icon;
}

void QtPieItem::SetWeight(int weight)
{
	_weight = weight;
}

int QtPieItem::GetWeight() const
{
	return _weight;
}

bool QtPieItem::IsEnabled() const
{
	return _isEnable;
}

void QtPieItem::SetEnabled(bool enabled)
{
	_isEnable = enabled;
}
