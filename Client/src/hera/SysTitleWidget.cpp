#include "SysTitleWidget.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

SysTitleWidget::SysTitleWidget(bool hasMax, bool hasFull, QWidget *parent):
QWidget(parent),
_isMax(false),
_isFullscreen(false),
_hasMax(hasMax)
{
	_min = new FourStateImageButton();
	_max = new FourStateImageButton();
	_close = new FourStateImageButton();
	_full = new TwoStateImageButton(":/System/fullscreen", ":/System/fullscreen_exit");

	connect(_min, &QToolButton::clicked, this, &SysTitleWidget::OnMinTriggered);
	connect(_max, &QToolButton::clicked, this, &SysTitleWidget::OnMaxTriggered);
	connect(_close, &QToolButton::clicked, this, &SysTitleWidget::OnCloseTriggered);
	connect(_full, &QToolButton::clicked, this, &SysTitleWidget::OnFullscreenTriggered);

	_min->LoadPixmap(":/System/min");
	_max->LoadPixmap(":/System/max");
	_close->LoadPixmap(":/System/close");

	_min->setToolTip(tr("最小化窗口"));
	_max->setToolTip(tr("最大化窗口"));
	_close->setToolTip(tr("关闭窗口"));
	_full->setToolTip(tr("全屏幕观看"));

	//_min->setIcon(style()->standardPixmap(QStyle::SP_TitleBarMinButton));
	//_max->setIcon(style()->standardPixmap(QStyle::SP_TitleBarMaxButton));
	//_close->setIcon(style()->standardPixmap(QStyle::SP_TitleBarCloseButton));

	QHBoxLayout* mainLayout = new QHBoxLayout();
	if(hasFull)
	{
		mainLayout->addWidget(_full);
	}
	mainLayout->addWidget(_min);
	if(hasMax)
	{
		mainLayout->addWidget(_max);
	}
	mainLayout->addWidget(_close);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	int number = 2;

	if(hasFull)
	{
		number++;
	}

	if(hasMax)
	{
		number++;
	}


	setFixedWidth(35*number);

	setFixedHeight(22);
	setLayout(mainLayout);
}

SysTitleWidget::~SysTitleWidget()
{
	if(_full != NULL)
	{
		_full->deleteLater();
		_full = NULL;
	}
	if(_min != NULL)
	{
		_min->deleteLater();
		_min = NULL;
	}
	if(_max != NULL)
	{
		_max->deleteLater();
		_max = NULL;
	}
	if(_close != NULL)
	{
		_close->deleteLater();
		_close = NULL;
	}
}

void SysTitleWidget::OnFullscreenTriggered()
{
	QWidget* window = topLevelWidget();
	if(window != NULL)
	{
		if(_isFullscreen)
		{
			window->showNormal();
		}
		else
		{
			window->showFullScreen();
		}
	}
	_isFullscreen = !_isFullscreen;
}

void SysTitleWidget::OnMinTriggered()
{
	QWidget* window = topLevelWidget();
	if(window != NULL)
		window->showMinimized();
	Q_EMIT OnMin();
}

void SysTitleWidget::OnMaxTriggered()
{
	if(!_hasMax)
		return;

	QWidget* window = topLevelWidget();
	if(window != NULL)
	{
		if(_isMax)
		{
			window->restoreGeometry(_savedGeometry);
		}
		else
		{
			_savedGeometry = window->saveGeometry();
			QDesktopWidget *desktop = QApplication::desktop();
			window->resize(desktop->availableGeometry().width(), desktop->availableGeometry().height());
			window->move(0, 0);
			//window->showMaximized();
		}
	}
	_isMax = !_isMax;
	Q_EMIT OnMax();
}

void SysTitleWidget::OnCloseTriggered()
{
	QWidget* window = topLevelWidget();
	if(window != NULL)
		window->close();
	Q_EMIT OnClose();
}