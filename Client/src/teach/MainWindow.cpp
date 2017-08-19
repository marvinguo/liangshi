#include "MainWindow.h"
#include "MainWindowMenu.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
	#pragma execution_character_set("utf-8")
#endif

MainWindow::MainWindow(QWidget *parent):
QMainWindow(parent),
_menu(new MainWindowMenu(this)),
_teachMsgBox(new TeachMessageBox(this)),
_broadcast(new ThorProtocol(NULL, new ThorServer(this), this)),
_client(NULL),
_protocol(new TyrProtocol(this)),
_handler(new SyncHandler(this))
{
	_handler->SetTyrProtocol(_protocol);
	InitMenu();
	InitMainWindow();
	connect(_broadcast, SIGNAL(OnGetServerInfo(ThorProtocol::DiscoverType, QString, int)), this, SLOT(OnOnGetServerInfoTriggered(ThorProtocol::DiscoverType, QString, int)));
	connect(_teachMsgBox, SIGNAL(OnCancelWaitingForConnection()), this, SLOT(OnCancelWaitingForConnectionTriggered()));
	connect(_teachMsgBox, SIGNAL(OnCancelSyncing()), this, SLOT(OnCancelSyncingTriggered()));
	connect(_protocol, SIGNAL(OnCommand(Tyr::TyrCommand*)), _handler, SLOT(OnCommandTriggered(Tyr::TyrCommand*)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::InitMainWindow()
{
	//setFocus();
	setWindowTitle(tr("Teach"));
	setWindowIcon(QIcon(":/Resources/AppIcons/teach.png"));
	setAttribute(Qt::WA_AcceptTouchEvents, true);
}

void MainWindow::InitMenu()
{
	_menu->InitMenu();
	connect(_menu, SIGNAL(OnSyncCourseware()), this, SLOT(OnSyncCoursewareTriggered()));
	connect(_menu, SIGNAL(OnClass()), this, SLOT(OnClassTriggered()));
	QTimer *timer = new QTimer(this);
	timer->setSingleShot(true);
	connect(timer, SIGNAL(timeout()), this, SLOT(FirstShowMenuOnTimer()));
	timer->start(500);
}

void MainWindow::OnClassTriggered()
{
	_broadcastType = ThorProtocolType_Control;
	_broadcast->StartListen();
	_teachMsgBox->ShowWaitingForConnection();
}

void MainWindow::OnSyncCoursewareTriggered()
{
	_broadcastType = ThorProtocolType_Sync;
	_broadcast->StartListen();
	_teachMsgBox->ShowWaitingForConnection();
}

void MainWindow::OnOnGetServerInfoTriggered(ThorProtocolType type, QString ip, int port)
{
	if(_broadcastType != type)
		return;

	_broadcast->StopListen();
	_client = new SocketClient(NULL);
	_client->ConnectToHost(ip, port);
	connect(_client, SIGNAL(OnAboutToDelete(SocketClient*)), this, SLOT(OnClientDisconnectTriggered(SocketClient*)));

	if(type == ThorProtocolType_Sync)
	{
		_protocol->SetSocketClient(_client);
		_teachMsgBox->HideMsgBox();
		_teachMsgBox->ShowSyncing();
	}

	if(type == ThorProtocolType_Control)
	{
		_teachMsgBox->HideMsgBox();
		ControlWidget* widget = new ControlWidget(_client, this);
		setCentralWidget(widget);
	}
}

void MainWindow::OnClientDisconnectTriggered(SocketClient*)
{
	if(_client != NULL)
	{
		if(_broadcastType == ThorProtocolType_Sync)
		{
			_teachMsgBox->HideMsgBox();
			disconnect(_client, SIGNAL(OnAboutToDelete(SocketClient*)), this, SLOT(OnClientDisconnectTriggered(SocketClient*)));
			_client = NULL;
		}

		if(_broadcastType == ThorProtocolType_Control)
		{
			QWidget* widget = centralWidget();
			setCentralWidget(NULL);
			widget->setParent(NULL);
			widget->deleteLater();
		}
	}
}

void MainWindow::OnCancelWaitingForConnectionTriggered()
{
	_teachMsgBox->HideMsgBox();
}

void MainWindow::OnCancelSyncingTriggered()
{
	if(_client != NULL)
	{
		disconnect(_client, SIGNAL(OnAboutToDelete(SocketClient*)), this, SLOT(OnClientDisconnectTriggered(SocketClient*)));
		_client->Disconnect();
		_client = NULL;
	}
}

void MainWindow::FirstShowMenuOnTimer()
{
	_menu->Show(QPoint(width()/2, height()/2));
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
	//qDebug() << "show menu";
	//_menu->Show(e->globalPos());
}

void MainWindow::contextMenuEvent(QContextMenuEvent *e)
{
	_menu->Show(e->globalPos());
}

void MainWindow::closeEvent(QCloseEvent *e)
{
	return QMainWindow::closeEvent(e);
}

//需要研究piemenu机制，使它适应touch, 现在touch的event不准确
bool MainWindow::event(QEvent *e)
{	
	if (e->type() == QEvent::TouchBegin)
	{
		bool accept = false;

		//	this->log->appendPlainText(QString("touch  begin: %1 point(s)").arg(static_cast<QTouchEvent *>(event)->touchPoints().count()));
		QList<QTouchEvent::TouchPoint> touches = static_cast<QTouchEvent *>(e)->touchPoints();
		int count  = touches.count();

		if(count == 2)
		{

		}

		if(count == 3)
		{
			accept = true;
			int x = (touches[0].pos().x() + touches[1].pos().x() + touches[2].pos().x()) /3;
			int y = (touches[0].pos().y() + touches[1].pos().y() + touches[2].pos().y()) /3;
			QPoint point(x, y);
			_menu->Show(point);
		}

		if(accept)
		{
			e->accept();
			return true;
		}
	}

	if (e->type() == QEvent::TouchUpdate)
	{
		bool accept = false;
        
		QList<QTouchEvent::TouchPoint> touches = static_cast<QTouchEvent *>(e)->touchPoints();
		int count  = touches.count();
        
		if(count == 2)
		{
            
		}
        
		if(count == 3)
		{
			accept = true;
			int x = (touches[0].pos().x() + touches[1].pos().x() + touches[2].pos().x()) /3;
			int y = (touches[0].pos().y() + touches[1].pos().y() + touches[2].pos().y()) /3;
			QPoint point(x, y);
			_menu->Show(point);
		}
        
		if(accept)
		{
			e->accept();
			return true;
		}
	}

	if (e->type() == QEvent::TouchEnd) {
	//	this->log->appendPlainText(QString("touch    end: %1 point(s)").arg(static_cast<QTouchEvent *>(event)->touchPoints().count()));
		return true;
	}

	return QMainWindow::event(e);
}

void MainWindow::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	QPixmap pixmap;
	pixmap.load(":/Resources/Images/background.jpg");
	painter.drawPixmap(0, 0, width(), height(), pixmap);
	QMainWindow::paintEvent(e);
}

