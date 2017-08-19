#pragma once

#include "../common/Qt.h"
#include "TouchableScrollArea.h"

class MainWindow;

//仿造android的activity
class MobileActivity : public QWidget
{
	Q_OBJECT
public:
	MobileActivity(MainWindow* mainWindow, QWidget *parent = 0);
	virtual ~MobileActivity();

	//数据第一次加载，应该在这里设置是否第一次加载数据，因为不能放在构造函数里
	virtual void OnBackKey() = 0;
	virtual void OnPause() = 0;
	virtual void OnInitialize();
	virtual void OnActive();
	virtual void OnDeactive();

protected:
	bool _hasInitialized;
	MainWindow* _mainWindow;

	//主界面是scroll, 如果了类需要这样的东西
	TouchableScrollArea* _scroll;
};

