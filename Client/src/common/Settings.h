#pragma once

#include "stl.h"
#include "Qt.h"

class Settings 
{
public:
	Settings();
	virtual ~Settings();

	QString GetHomeUrl();
	QString GetP2PUrl();

private:
	QSettings *_ini;
};

