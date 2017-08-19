#include "DrawingToolsTitleWidget.h"
#include "ScreenshotWindow.h"
#include "CameraWindow.h"
#include "KeTime.h"
#include "NotifyWindow.h"
#include "TextElement.h"
#include "HtmlTextElement.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

DrawingToolsTitleWidget::DrawingToolsTitleWidget(KeTime* keTime, QWidget *parent):
QWidget(parent),
_keTime(keTime)
{
	_draw = new ScCheckableButton(tr("像粉笔一样写字,快捷键F1"));
	_line = new ScCheckableButton(tr("直线写字,快捷键F2"));
	_rect = new ScCheckableButton(tr("矩形"));
	_pRect = new ScCheckableButton(tr("正方形"));
	_ellipse = new ScCheckableButton(tr("椭圆"));
	_pEllipse = new ScCheckableButton(tr("正圆"));
	_select =  new ScCheckableButton(tr("选择文字,图片,板书,快捷键F3,在文字,图片上点击选择，按住Ctrl,移动到上面自动选择"));

	_lineStyle = new TitleComboBox();
	_lineStyle->addItem(tr("实线"));
	_lineStyle->addItem(tr("虚线"));
	connect(_lineStyle, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &DrawingToolsTitleWidget::OnPenStyleChangeTriggered);

	_revise = new	 TitleCheckBox();
	_revise->setText(tr("修正粉笔"));
	//_revise->setChecked(true);
	connect(_revise, &TitleCheckBox::stateChanged, this, &DrawingToolsTitleWidget::OnReviseChecked);
	_revise->setToolTip(tr("修正后，粉笔笔迹会圆滑，适合画心数学函数等图形，不修正会使用原始笔迹"));

	_draw->setText(tr("粉笔"));
	_draw->setChecked(true);
	_line->setText(tr("直线"));
	_rect->setText(tr("矩形"));
	_pRect->setText(tr("正方"));
	_ellipse->setText(tr("椭圆"));
	_pEllipse->setText(tr("正圆"));
	_select->setText(tr("选择"));

	connect(_draw, &ScCheckableButton::toggled, this, &DrawingToolsTitleWidget::OnCheckedTriggered);
	connect(_draw, &ScCheckableButton::OnChecked, this, &DrawingToolsTitleWidget::OnDrawTriggered);
	connect(_line, &ScCheckableButton::toggled, this, &DrawingToolsTitleWidget::OnCheckedTriggered);
	connect(_line, &ScCheckableButton::OnChecked, this, &DrawingToolsTitleWidget::OnLineTriggered);
	connect(_rect, &ScCheckableButton::toggled, this, &DrawingToolsTitleWidget::OnCheckedTriggered);
	connect(_rect, &ScCheckableButton::OnChecked, this, &DrawingToolsTitleWidget::OnRectTriggered);
	connect(_pRect, &ScCheckableButton::toggled, this, &DrawingToolsTitleWidget::OnCheckedTriggered);
	connect(_pRect, &ScCheckableButton::OnChecked, this, &DrawingToolsTitleWidget::OnPRectTriggered);
	connect(_ellipse, &ScCheckableButton::toggled, this, &DrawingToolsTitleWidget::OnCheckedTriggered);
	connect(_ellipse, &ScCheckableButton::OnChecked, this, &DrawingToolsTitleWidget::OnEllipseTriggered);
	connect(_pEllipse, &ScCheckableButton::toggled, this, &DrawingToolsTitleWidget::OnCheckedTriggered);
	connect(_pEllipse, &ScCheckableButton::OnChecked, this, &DrawingToolsTitleWidget::OnPEllipseTriggered);
	connect(_select, &ScCheckableButton::toggled, this, &DrawingToolsTitleWidget::OnCheckedTriggered);
	connect(_select, &ScCheckableButton::OnChecked, this, &DrawingToolsTitleWidget::OnSelectTriggered);

	QShortcut *f1 = new QShortcut(QKeySequence(Qt::Key_F1), this);
	f1->setContext(Qt::ApplicationShortcut);
	connect(f1, &QShortcut::activated, _draw, &ScCheckableButton::click);
	QShortcut *f2 = new QShortcut(QKeySequence(Qt::Key_F2), this);
	f2->setContext(Qt::ApplicationShortcut);
	connect(f2, &QShortcut::activated, _line, &ScCheckableButton::click);
	QShortcut *f3 = new QShortcut(QKeySequence(Qt::Key_F3), this);
	f3->setContext(Qt::ApplicationShortcut);
	connect(f3, &QShortcut::activated, _select, &ScCheckableButton::click);

	_paste = new TitleButton(tr("粘贴"));
	_undo = new TitleButton(tr("撤销"));
	_redo = new TitleButton(tr("重做"));
	_lineWidth = new NumberButton(tr("笔的宽度"));
	_lineWidth->ToPenMode();
	//_lineWidth->setText(tr("线宽"));
	connect(_lineWidth, &NumberButton::OnWidthChanged, this, &DrawingToolsTitleWidget::OnPenWidthChangedTriggered);
	_lineColor = new ColorButton(tr("笔的颜色"));
	_lineColor->setText(tr("线色"));
	connect(_lineColor, &ColorButton::OnColorChanged, this, &DrawingToolsTitleWidget::OnPenColorChangedTriggered);

	QShortcut *f6 = new QShortcut(QKeySequence(Qt::Key_F6), this);
	f6->setContext(Qt::ApplicationShortcut);
	connect(f6, &QShortcut::activated, this, &DrawingToolsTitleWidget::OnScreenshotTriggered);

	_screenshot = new TitleButton(tr("截图"));
	connect(_screenshot, &QPushButton::clicked, this, &DrawingToolsTitleWidget::OnScreenshotTriggered);
	_screenshot->setToolTip(tr("屏幕截图,快捷键F6"));

	QShortcut *f7 = new QShortcut(QKeySequence(Qt::Key_F7), this);
	f7->setContext(Qt::ApplicationShortcut);
	connect(f7, &QShortcut::activated, this, &DrawingToolsTitleWidget::OnCameraTriggered);

	_camera = new TitleButton(tr("照相"));
	connect(_camera, &QPushButton::clicked, this, &DrawingToolsTitleWidget::OnCameraTriggered);
	_camera->setToolTip(tr("用摄像头照相,快捷键F7"));
	
	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->addWidget(_draw);
	mainLayout->addWidget(_line);
	/*
	mainLayout->addWidget(_rect);
	mainLayout->addWidget(_pRect);
	mainLayout->addWidget(_ellipse);
	mainLayout->addWidget(_pEllipse);
	*/
	mainLayout->addWidget(_select);
	mainLayout->addWidget(_revise);
	mainLayout->addWidget(_lineStyle);
	mainLayout->addWidget(_lineWidth);
	mainLayout->addWidget(_lineColor);
	mainLayout->addWidget(_paste);
	mainLayout->addWidget(_undo);
	mainLayout->addWidget(_redo);
	mainLayout->addWidget(_screenshot);
	mainLayout->addWidget(_camera);
	//mainLayout->addStretch();
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	setFixedWidth(37*14);
	setFixedHeight(22);
	setLayout(mainLayout);

	ScreenshotWindow* screen = ScreenshotWindow::Instance();
	connect(screen, &ScreenshotWindow::OnScreenPixmap, this, &DrawingToolsTitleWidget::OnGetPixmap);
#ifdef Q_OS_WIN32
	CameraWindow* camera = CameraWindow::Instance();
	connect(camera, &CameraWindow::OnCameraPixmap, this, &DrawingToolsTitleWidget::OnGetPixmap);
#endif
}

DrawingToolsTitleWidget::~DrawingToolsTitleWidget()
{
	if(_rect != NULL)
	{
		_rect->deleteLater();
		_rect = NULL;
	}
	if(_pRect != NULL)
	{
		_pRect->deleteLater();
		_pRect = NULL;
	}
	if(_ellipse != NULL)
	{
		_ellipse->deleteLater();
		_ellipse = NULL;
	}
	if(_pEllipse != NULL)
	{
		_pEllipse->deleteLater();
		_pEllipse = NULL;
	}
}

void DrawingToolsTitleWidget::OnDrawTriggered()
{
	_lineWidth->ToPenMode();
	NotifyWindow* notify = NotifyWindow::Instance();
	notify->ShowNotify(tr("切换为粉笔模式"));
	Q_EMIT OnDraw();
}

void DrawingToolsTitleWidget::OnLineTriggered()
{
	_lineWidth->ToPenMode();
	NotifyWindow* notify = NotifyWindow::Instance();
	notify->ShowNotify(tr("切换为直线模式"));
	Q_EMIT OnLine();
}

void DrawingToolsTitleWidget::OnRectTriggered()
{
	_lineWidth->ToPenMode();
	NotifyWindow* notify = NotifyWindow::Instance();
	notify->ShowNotify(tr("切换为长方形模式"));
	Q_EMIT OnRect();
}

void DrawingToolsTitleWidget::OnPRectTriggered()
{
	_lineWidth->ToPenMode();
	NotifyWindow* notify = NotifyWindow::Instance();
	notify->ShowNotify(tr("切换为正方形模式"));
	Q_EMIT OnPRect();
}

void DrawingToolsTitleWidget::OnEllipseTriggered()
{
	_lineWidth->ToPenMode();
	NotifyWindow* notify = NotifyWindow::Instance();
	notify->ShowNotify(tr("切换为椭圆模式"));
	Q_EMIT OnEllipse();
}

void DrawingToolsTitleWidget::OnPEllipseTriggered()
{
	_lineWidth->ToPenMode();
	NotifyWindow* notify = NotifyWindow::Instance();
	notify->ShowNotify(tr("切换为圆模式"));
	Q_EMIT OnPEllipse();
}

void DrawingToolsTitleWidget::OnSelectTriggered()
{
	_lineWidth->ToTextMode();
	NotifyWindow* notify = NotifyWindow::Instance();
	notify->ShowNotify(tr("切换为选择模式"));
	Q_EMIT OnSelect();
}

void DrawingToolsTitleWidget::OnPenColorChangedTriggered(QColor color)
{
	//这里面以后不一定加入notify
	Q_EMIT OnLineColorChanged(color);
}

void DrawingToolsTitleWidget::OnPenWidthChangedTriggered(int width)
{
	NotifyWindow* notify = NotifyWindow::Instance();
	notify->ShowNotify(QString(tr("当前线宽为%1")).arg(width));
	Q_EMIT OnLineWidthChanged(width);
}

void DrawingToolsTitleWidget::OnPenStyleChangeTriggered(const QString& title)
{
	NotifyWindow* notify = NotifyWindow::Instance();
	if(title == tr("实线"))
	{
		notify->ShowNotify(tr("当前线弄为实线"));
		Q_EMIT OnLineStyleChanged(Qt::SolidLine);
	}
	if(title == tr("虚线"))
	{
		notify->ShowNotify(tr("当前线弄为虚线"));
		Q_EMIT OnLineStyleChanged(Qt::DotLine);
	}
}

void DrawingToolsTitleWidget::OnCheckedTriggered(bool checked)
{
	if(checked)
	{
		ScCheckableButton* thisButton = static_cast<ScCheckableButton*>(sender());

		vector<ScCheckableButton*> buttons;
		buttons.push_back(_draw);
		buttons.push_back(_line);
		buttons.push_back(_rect);
		buttons.push_back(_pRect);
		buttons.push_back(_ellipse);
		buttons.push_back(_pEllipse);
		buttons.push_back(_select);

		for(vector<ScCheckableButton*>::iterator i = buttons.begin(); i != buttons.end(); ++i)
		{
			ScCheckableButton* button = *i;
			if(button != thisButton)
				button->UnCheck();
		}
	}
}

void DrawingToolsTitleWidget::OnScreenshotTriggered()
{
	KeTime* keTime = _keTime;
	bool needContinue = !keTime->IsPausing();
	keTime->Pause();

	QWidget* window = topLevelWidget();
	if(window != NULL)
		window->hide();
	
	ScreenshotWindow* screen = ScreenshotWindow::Instance();
	screen->ShowModal();
	
	if(needContinue)
		keTime->Start();

	if(window != NULL)
	{
		window->show();
		//window->exec();
	}
}

void DrawingToolsTitleWidget::OnCameraTriggered()
{
	KeTime* keTime = _keTime;
	bool needContinue = !keTime->IsPausing();
	keTime->Pause();

	//check whether has local video???
	bool hasLocal = Q_EMIT OnGetLocalCamera();

	if(!hasLocal)
	{
#ifdef Q_OS_WIN32
		CameraWindow* camera = CameraWindow::Instance();
		camera->ShowModal();
#endif
	}

	if(needContinue)
		keTime->Start();
}

void DrawingToolsTitleWidget::OnStartTriggered()
{
	EnableButtons(true);
}

void DrawingToolsTitleWidget::OnPauseTriggered()
{
	//想想这个时候也不disable
	//EnableButtons(false);
}

void DrawingToolsTitleWidget::OnHighlightElementChangeTriggered(Element* element)
{
	int size = 0;
	TextElement* text = dynamic_cast<TextElement*>(element);
	HtmlTextElement* html = dynamic_cast<HtmlTextElement*>(element);

	if(text != NULL)
		size = text->_fontSize;
	if(html != NULL)
		size = html->_fontSize;

	if(text != NULL || html != NULL)
	{
		//_lineWidth->setText(QString::number(size));
		//_lineWidth->SetMiddleNumber(size);
	}
}

void DrawingToolsTitleWidget::EnableButtons(bool enabled)
{
	vector<ScCheckableButton*> buttons;
	buttons.push_back(_draw);
	buttons.push_back(_line);
	buttons.push_back(_rect);
	buttons.push_back(_pRect);
	buttons.push_back(_ellipse);
	buttons.push_back(_pEllipse);
	buttons.push_back(_select);

	for(vector<ScCheckableButton*>::iterator i = buttons.begin(); i != buttons.end(); ++i)
	{
		ScCheckableButton* button = *i;
		button->setEnabled(enabled);
	}

	_lineColor->setEnabled(enabled);
	_lineStyle->setEnabled(enabled);
	_lineWidth->setEnabled(enabled);
	_screenshot->setEnabled(enabled);

	_paste->setEnabled(enabled);
	_undo->setEnabled(enabled);
	_redo->setEnabled(enabled);
}

void DrawingToolsTitleWidget::OnReviseChecked(int state)
{
	NotifyWindow* notify = NotifyWindow::Instance();
	if(state == 0)
	{
		notify->ShowNotify(tr("不用算法修正粉笔"));
		Q_EMIT EnableReviseDraw(false);
	}
	else
	{
		//if(state == 2)
		notify->ShowNotify(tr("用算法修正粉笔"));
		Q_EMIT EnableReviseDraw(true);
	}
}