#include "MetroMainWidget.h"
#include "../hera/HeraMainWidget.h"
#include "../hera/ChineseMessageBox.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

MetroMainWidget::MetroMainWidget(QWidget *parent):
QWidget(parent)
{
	setWindowTitle(tr("板书，课件制作工具"));
	setWindowIcon(QIcon(":/App/collect"));

	_newMaterial = new MetroButton(":/Buttons/material");
	_newMaterial->setText(tr("新建板书"));
	_newScriptMaterial = new MetroButton(":/Buttons/script");
	_newScriptMaterial->setText(tr("新建程序板书"));

	/*
	_newTest = new MetroButton(":/Buttons/test");
	_newTest->setText(tr("新建试题板书"));
	_newMultimedia = new MetroButton(":/Buttons/multimedia");
	_newMultimedia->setText(tr("新建音视频板书"));
	*/

	_newKe = new MetroButton(":/Buttons/ke");
	_newKe->setText(tr("新建课件"));
	_open = new MetroButton(":/Buttons/open");
	_open->setText(tr("打开文件"));
	_quit = new MetroButton(":/Buttons/quit");
	_quit->setText(tr("退出"));

	connect(_newMaterial, &MetroButton::clicked, this, &MetroMainWidget::OnNewMaterialTriggered);
	connect(_newScriptMaterial, &MetroButton::clicked, this, &MetroMainWidget::OnNewScriptMaterialTriggered);
	//connect(_newTest, &MetroButton::clicked, this, &MetroMainWidget::OnNewTestMaterialTriggered);
	connect(_newKe, &MetroButton::clicked, this, &MetroMainWidget::OnNewKeTriggered);
	connect(_open, &MetroButton::clicked, this, &MetroMainWidget::OnOpenTriggered);
	connect(_quit, SIGNAL(clicked()), qApp, SLOT(quit()));

	QHBoxLayout* centerLayout = new QHBoxLayout();
	centerLayout->addStretch();
	centerLayout->addWidget(_newMaterial);
	centerLayout->addWidget(_newScriptMaterial);

	/*
	centerLayout->addWidget(_newTest);
	_newTest->setVisible(false);
	centerLayout->addWidget(_newMultimedia);
	_newMultimedia->setVisible(false);
	*/

	centerLayout->addWidget(_newKe);
	centerLayout->addWidget(_open);
	centerLayout->addWidget(_quit);
	centerLayout->addStretch();
	centerLayout->setSpacing(0);
	centerLayout->setContentsMargins(0, 0, 0, 0);

	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->addLayout(centerLayout);
	setLayout(mainLayout);
}

MetroMainWidget::~MetroMainWidget()
{

}

void MetroMainWidget::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	/*
	QPalette pal(palette());
	QPixmap pixmap(":/App/background");
	pal.setBrush(QPalette::Window, QBrush(pixmap.scaled(event->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
	setPalette(pal);
	*/
}

void MetroMainWidget::paintEvent(QPaintEvent *e)
{
	QPixmap pixmap(":/App/background");

	QRect r = rect();
	QSize size(r.width(), r.height());
	QPixmap scaledPix = pixmap.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	QPainter painter(this);
	painter.drawPixmap(0, 0, scaledPix.width(), scaledPix.height(), scaledPix);
	QWidget::paintEvent(e);
}

void MetroMainWidget::OnNewMaterialTriggered()
{
	HeraMainWidget* widget = new HeraMainWidget(NewableDocType_Ban);
	connect(widget, &HeraMainWidget::OnClosed, this, &MetroMainWidget::OnShowTriggered);
	hide();
	widget->ShowModal();
}

void MetroMainWidget::OnNewScriptMaterialTriggered()
{
	HeraMainWidget* widget = new HeraMainWidget(NewableDocType_BanScript);
	connect(widget, &HeraMainWidget::OnClosed, this, &MetroMainWidget::OnShowTriggered);
	hide();
	widget->ShowModal();
}

void MetroMainWidget::OnNewTestMaterialTriggered()
{
	HeraMainWidget* widget = new HeraMainWidget(NewableDocType_BanTest);
	connect(widget, &HeraMainWidget::OnClosed, this, &MetroMainWidget::OnShowTriggered);
	hide();
	widget->ShowModal();
}

void MetroMainWidget::OnNewKeTriggered()
{
	HeraMainWidget* widget = new HeraMainWidget(NewableDocType_Ke);
	connect(widget, &HeraMainWidget::OnClosed, this, &MetroMainWidget::OnShowTriggered);
	hide();
	widget->ShowModal();
}

void MetroMainWidget::OnOpenTriggered()
{
	QString path = QFileDialog::getOpenFileName(this, QObject::tr("打开板书文件"), ".", QObject::tr("板书文件(*.ban)"));
	if(!path.isEmpty())
	{
		HeraMainWidget* widget = HeraMainWidget::LoadScFile(path);
		if(widget != NULL)
		{
			connect(widget, &HeraMainWidget::OnClosed, this, &MetroMainWidget::OnShowTriggered);
			hide();
			widget->ShowModal();
		}
		else
		{
			ChineseMessageBox::Show(tr("错误"), tr("打开文件错误"), ChineseMessageBox::OK, ChineseMessageBox::Error);
		}
	}
}

void MetroMainWidget::OnShowTriggered()
{
	QThread::msleep(200);
	show();
}
