#include "MainWindow.h"
#include "../common/ThorClient.h"
#include "../common/ThorProtocol.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
	#pragma execution_character_set("utf-8")
#endif

MainWindow::MainWindow(QWidget *parent):
QMainWindow(parent),
_widget(NULL),
_menu(new MainWindowMenu(this)),
_server(new SocketServer(this))
{
	InitMenu();
	InitMainWindow();
}

MainWindow::~MainWindow()
{
}

void MainWindow::InitMainWindow()
{
	setWindowTitle(tr("Sync"));
	setWindowIcon(QIcon(":/Resources/AppIcons/sync.png"));

	connect(_server, SIGNAL(OnNewClient(SocketClient*)), this, SLOT(OnNewSocketClientTriggered(SocketClient*)));
	_server->StartListen();
}

void MainWindow::InitMenu()
{
	_menu->InitMenu();
	connect(_menu, SIGNAL(SyncDevice()), this, SLOT(SyncDeviceTriggered()));
	QTimer *timer = new QTimer(this);
	timer->setSingleShot(true);
	connect(timer, SIGNAL(timeout()), this, SLOT(FirstShowMenuOnTimer()));
	timer->start(500);
}

void MainWindow::FirstShowMenuOnTimer()
{
	_menu->Show(QPoint(width()/2, height()/2));
}

void MainWindow::SyncDeviceTriggered()
{
	//ThorClient立即删除
	ThorClient* client = new ThorClient();
	ThorProtocol* protocol = new ThorProtocol(client, NULL, client);
	protocol->SendServerInfo(ThorProtocol::Sync, _server->GetLocalNetIp(), _server->GetPort());
	client->deleteLater();
}

//应该是sync发命令,teach返回
void MainWindow::OnNewSocketClientTriggered(SocketClient* client)
{
	connect(client, SIGNAL(OnAboutToDelete(SocketClient*)), this, SLOT(OnSocketClientDisconnectTriggered(SocketClient*)));
	if(_widget != NULL)
	{
		setCentralWidget(NULL);
		_widget->setParent(NULL);
		_widget->deleteLater();
	}
	_widget = new SyncWidget(client, this);
	setCentralWidget(_widget);
}

void MainWindow::OnSocketClientDisconnectTriggered(SocketClient* obj)
{
	disconnect(obj, SIGNAL(OnAboutToDelete(SocketClient*)), this, SLOT(OnSocketClientDisconnectTriggered(SocketClient*)));
	if(_widget != NULL)
	{
		_widget->setParent(NULL);
		_widget->deleteLater();
		_widget = NULL;
		setCentralWidget(NULL);
	}
}

void MainWindow::contextMenuEvent(QContextMenuEvent *e)
{
	_menu->Show(e->globalPos());
}

void MainWindow::closeEvent(QCloseEvent *e)
{
	e->accept();
}

void MainWindow::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	QPixmap pixmap;
	pixmap.load(":/Resources/Images/background.jpg");
	painter.drawPixmap(0, 0, width(), height(), pixmap);
	QMainWindow::paintEvent(e);
}

