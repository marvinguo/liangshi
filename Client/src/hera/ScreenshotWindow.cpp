#include "ScreenshotWindow.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ScreenshotWindow::ScreenshotWindow():
QDialog(NULL),
QtSingleton<ScreenshotWindow>(this),
_isDrawing(false)
{
	setModal(true);
	setCursor(Qt::CrossCursor);

	_toolbar = new QToolBar(this);
	QAction *okAction = new QAction(tr("确定"), this);
	connect(okAction, &QAction::triggered, this, &ScreenshotWindow::OnOKTriggered);

	QAction *cancelAction = new QAction(tr("取消"), this);
	connect(cancelAction, &QAction::triggered, this, &ScreenshotWindow::close);

	_toolbar->setWindowFlags(Qt::Popup);
	_toolbar->addAction(okAction);
	_toolbar->addAction(cancelAction);
	//_toolbar->resize(50,15);
	_toolbar->setMovable(false);
	_toolbar->hide();

	//QList<QMenu*> menulist(_toolbar->findChildren<QMenu*>());
	//for(int mnum = 0; mnum < menulist.size(); ++mnum)
	//{
	//	QList<QAction*> actionlist(menulist.at(mnum)->actions());
	//	for(int actnum = 0; actnum < actionlist.size(); ++actnum)
	//	{
	//		actionlist.at(actnum)->setStatusTip("");
	//	}
	//}
}

ScreenshotWindow::~ScreenshotWindow()
{
	if(_toolbar != NULL)
	{
		_toolbar->deleteLater();
		_toolbar = NULL;
	}
}

void ScreenshotWindow::ShowModal()
{
	//QApplication::desktop()->update();
	//在windows7不是aero下，截不到图，sleep一下就好，不知是什么问题
	QThread::msleep(200);
	QScreen *screen = QGuiApplication::primaryScreen();
	_fullPixmap = screen->grabWindow(QApplication::desktop()->winId());
	_backgroundPixmap = _fullPixmap.copy();
	_drawingPixmap = _fullPixmap.copy();
	QPainter Toppainter(&_fullPixmap);
	Toppainter.setBrush(Qt::Dense4Pattern);
	QRect TopRect(0, 0, _fullPixmap.width(), _fullPixmap.height());
	Toppainter.drawRect(TopRect);
	Toppainter.end();
	showFullScreen();
	exec();
}

void ScreenshotWindow::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.drawPixmap(0, 0, _drawingPixmap);
}

void ScreenshotWindow::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		_isDrawing = true;
		_startPoint = event->pos();
	}
}

void ScreenshotWindow::mouseMoveEvent(QMouseEvent *event)
{
	if (_isDrawing)
	{
		_drawingPixmap = _fullPixmap.copy();
		_endPoint = event->pos();
		PaintPixmap(_drawingPixmap);
	}
	else
	{
		if (_captureRect.contains(event->pos()))
		{
			setCursor(Qt::SizeAllCursor);
		}
		else
		{
			setCursor(Qt::ArrowCursor);
		}
		update();
	}
}

void ScreenshotWindow::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		_isDrawing = false;
		_endPoint = event->pos();
		_captureRect = QRect(_startPoint, _endPoint);
		PaintPixmap(_drawingPixmap);
		_toolbar->move(_endPoint);
		_toolbar->show();
	}
}

void ScreenshotWindow::PaintPixmap(QPixmap &pixmap)
{
	QRect rect(_startPoint, _endPoint);
	QRect rect1 = rect.normalized();
	QPainter painter(&pixmap);
	painter.drawPixmap(rect1, _backgroundPixmap, rect1);
	update();
}

void ScreenshotWindow::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape)
	{
		close();
	}
}

void ScreenshotWindow::closeEvent(QCloseEvent * event)
{
	_toolbar->hide();
}

void ScreenshotWindow::OnOKTriggered()
{
	_captureRect = _captureRect.normalized();
	QPixmap capture = _backgroundPixmap.copy(_captureRect);
	Q_EMIT OnScreenPixmap(capture);
	close();
}

