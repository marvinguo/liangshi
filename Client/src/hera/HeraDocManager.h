#pragma once

#include "../common/Qt.h"
#include "../common/library.h"
#include "../common/QtSingleton.h"
#include "../../../SharedCode/LokiBuffer.h"
#include "HeraMainWidget.h"

class LIANGSHIEXPORT HeraDocManager : public QObject, public QtSingleton<HeraDocManager>
{
	Q_OBJECT
public:
	HeraDocManager();
	virtual ~HeraDocManager();

	void AddHera(HeraMainWidget* hera);
	void RemoteHera(HeraMainWidget* hera);
	int GetHeraCount();

Q_SIGNALS:
	void OnHeraCountChanged(int count);

private:
	vector<HeraMainWidget*> _allDocs;
	QMutex _changeMutex;
};

