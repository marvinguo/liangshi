#include "MainWindow.h"
#include "../hera/ChineseMessageBox.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

#ifdef Q_OS_ANDROID
JNIEXPORT jboolean JNICALL Java_org_qtproject_qt5_android_bindings_QtActivity_QtActivty_OnStop(JNIEnv *env, jobject context)
{
	qDebug() << "c++ part onstop";
	return true;
}
#endif

MainWindow::MainWindow(QWidget *parent):
QMainWindow(parent),
_currentActivity(NULL)
{
	setWindowIcon(QIcon(":/App/mobile"));
	setWindowTitle(tr("良师益友手机版"));

	_sliding = new SlidingWidget(this);
	_sliding->setSpeed(500);

	//0
	_login = new LoginActivity(this);
	_sliding->addWidget(_login);
	connect(_login, &LoginActivity::OnLoginSuccess, this, &MainWindow::OnGotoNewMenu);

	//1
	_mainMenu = new MainMenuActivity(this);
	_sliding->addWidget(_mainMenu);

	//2
	_class = new ClasseActivity(this);
	_sliding->addWidget(_class);

	//3
	_collect = new KeActivity(this);
	_sliding->addWidget(_collect);

	//4
	_player = new PlayerActivity(this);
	_sliding->addWidget(_player);

	//5
	_live = new LiveActivity(this);
	_sliding->addWidget(_live);

	//6
	_preview = new PreviewActivity(this);
	_sliding->addWidget(_preview);

	//7
	_teaching = new TeachingActivity(this);
	_sliding->addWidget(_teaching);

	//8
	_offline = new OfflineClasseActivity(this);
	_sliding->addWidget(_offline);

	QWidget* centralWidget = new QWidget();
	QVBoxLayout* mainLayout = new QVBoxLayout();
	//mainLayout->addStretch();
	mainLayout->addWidget(_sliding);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	centralWidget->setLayout(mainLayout);
	setCentralWidget(centralWidget);

#define TEST_WIDTH 400
#define TEST_HEIGHT 700

	QDesktopWidget *desktop = QApplication::desktop();
#ifdef _WIN32
	setMaximumWidth(TEST_WIDTH);
	setMaximumHeight(TEST_HEIGHT);
#else
	//setMaximumWidth(desktop->availableGeometry().width());
	//setMaximumHeight(desktop->availableGeometry().height());
#endif
	//setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

	QString deviceID= Util::GetAndroidDeviceID();
	qDebug() << "android device ID:" << deviceID;

	OnGotoLogin();

	connect(qApp, &QGuiApplication::applicationStateChanged, this, &MainWindow::OnApplicationStateChanged);
}

void MainWindow::keyPressEvent(QKeyEvent *k)
{
	if(_currentActivity != NULL)
	{
		if(k->key() == Qt::Key_MediaPrevious)
		{
			_currentActivity->OnBackKey();
		}
	}
}

void MainWindow::OnApplicationStateChanged(Qt::ApplicationState state)
{
	qDebug() << state;
	switch(state)
	{
		case Qt::ApplicationSuspended:
		case Qt::ApplicationHidden:
		case Qt::ApplicationInactive:
			qDebug() << "MainWindow::OnApplicationStateChanged";
			_currentActivity->OnPause();
			break;
		case Qt::ApplicationActive:
			break;
	}
}

void MainWindow::OnGotoLogin()
{
	_sliding->slideInIdx(0);
	_login->OnActive();
	_currentActivity = _login;
}

void MainWindow::OnGotoMenu()
{
	_sliding->slideInIdx(1);
	_mainMenu->OnActive();
	_currentActivity = _mainMenu;
}

void MainWindow::OnGotoNewMenu()
{
	_sliding->slideInIdx(1);
	_mainMenu->UpdateData();
	_currentActivity = _mainMenu;
}

void MainWindow::OnGotoClass()
{
	_sliding->slideInIdx(2);
	_currentActivity = _class;
}

void MainWindow::OnGotoNewClass(ClassDTO dto)
{
	_sliding->slideInIdx(2);
	_class->OnActive();
	_class->UpdateData(dto);
	_currentActivity = _class;
}

void MainWindow::OnGotoKe()
{
	_sliding->slideInIdx(3);
	_mainMenu->OnActive();
	_currentActivity = _mainMenu;
}

void MainWindow::OnGotoNewCollect(QString id)
{
	_sliding->slideInIdx(3);
	_collect->OnActive();
	_collect->UpdateData(id);
	_currentActivity = _collect;
}

void MainWindow::OnGotoNewPlay(bool classPlay, QString classTitle, KeDTO keDTO)
{
	_sliding->slideInIdx(4);
	_player->OnActive();
	_player->UpdateData(classPlay, classTitle, keDTO);
	_currentActivity = _player;
}

void MainWindow::OnGotoNewLive(bool classLive, bool allTeachers, QString& session)
{
	_sliding->slideInIdx(5);
	_live->OnActive();
	_live->UpdateData(classLive, allTeachers, session);
	_currentActivity = _live;
}

void MainWindow::OnGotoPreview(bool offline, QString filepath)
{
	_sliding->slideInIdx(6);
	_preview->OnActive();
	_preview->UpdateData(offline, filepath);
	_currentActivity = _preview;
}

void MainWindow::OnGotoTeaching()
{
	_sliding->slideInIdx(7);
	_currentActivity = _teaching;
}

void MainWindow::OnGotoNewTeaching(TeachingDTO dto)
{
	_sliding->slideInIdx(7);
	_teaching->OnActive();
	_teaching->UpdateData(dto);
	_currentActivity = _teaching;
}

void MainWindow::OnGotoOffline()
{
	_sliding->slideInIdx(8);
	_offline->OnActive();
	_offline->UpdateData();
	_currentActivity = _offline;
}

void MainWindow::NeedNetworkErrorTriggered()
{
	ChineseMessageBox::Show(tr("网络连接错误，请确认Wifi或3G网可用"), tr("错误"), ChineseMessageBox::OK, ChineseMessageBox::Error);
}

void MainWindow::UseOffliceTriggered()
{
	ChineseMessageBox::Show(tr("网络连接错误，如果想继续，请使用离线学习"), tr("错误"), ChineseMessageBox::OK, ChineseMessageBox::Error);
}