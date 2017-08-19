#pragma once

#include "qtpieitem.h"

class QtPieAction : public QtPieItem
{
	Q_OBJECT
public:
	QtPieAction(const QString &text, QVariant& data, QObject *receiver, const char *member);
	QtPieAction(const QString &text, QObject *receiver, const char *member);
	QtPieAction(const QIcon &icon, const QString &text, QObject *receiver, const char *member);

	void Activate();
	QVariant GetData(){return _extraData;}

Q_SIGNALS:
	void activated();

protected:
	QVariant _extraData;
};