#pragma once

#include "../common/Qt.h"
#include "../hera/HeraMainWidget.h"

class PlayerPlugin : public QWidget
{
	Q_OBJECT
public:
	PlayerPlugin(QString cachefile, QWidget *parent = 0);
	~PlayerPlugin();

	void LoadCacheKe();

public Q_SLOTS:
	void OnFinishDownload();

private:
	QString _cachefile;
	HeraMainWidget* _hera;
	QStackedLayout* _mainLayout;
};

