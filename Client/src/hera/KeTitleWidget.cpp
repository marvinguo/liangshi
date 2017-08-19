#include "KeTitleWidget.h"
#include "BanBrowserDialog.h"
#include "KeTime.h"
#include "BanFile.h"
#include "TestBaseWidget.h"
#include "../common/library.h"
#include "NotifyWindow.h"
#ifdef _LiangshiDesktop
	#include "NotMobile/PdfAnalyser.h"
	#include "NotMobile/PptAnalyser.h"
#endif
#include "ChineseMessageBox.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

KeTitleWidget::KeTitleWidget(KeTime* keTime, QWidget *parent):
QWidget(parent),
_keTime(keTime),
_paused(true)
{
	_record = new TitleButton(tr("录制"));
	_record->setToolTip(tr("录制/暂停课件, 快捷键F5"));
	connect(_record, &QPushButton::clicked, this, &KeTitleWidget::OnRecrodButtonTriggered);

	_volume = new AudioVolumeIndicator(tr("麦克风"));
	_volume->setFixedWidth(50);

	_insert = new TitleButton(tr("插入板书"));
	_insert->setToolTip(tr("插入别的板书"));
	connect(_insert, &TitleButton::clicked, this, &KeTitleWidget::OnInsertTriggered);

	_new = new TitleButton(tr("新建黑板"));
	_new->setToolTip(tr("新建黑板"));
	connect(_new, &TitleButton::clicked, this, &KeTitleWidget::OnNewPageTriggered);

	_switch = new TitleButton(tr("切换黑板"));
	_switch->setToolTip(tr("切换黑板"));
	connect(_switch, &TitleButton::clicked, this, &KeTitleWidget::OnSwitchPageTriggered);

	_preview = new TitleButton(tr("预览"));
	_preview->setToolTip(tr("预览课件"));
	connect(_preview, &TitleButton::clicked, this, &KeTitleWidget::OnPreviewTriggered);
	_preview->setDisabled(true);

	_test = new TitleButton(tr("加入试题"));
	_test->setToolTip(tr("加入试题"));
	connect(_test, &TitleButton::clicked, this, &KeTitleWidget::OnAddTestTriggered);

#ifdef _LiangshiDesktop
	_import = new TitleButton(tr("导入"));
	_import->setToolTip(tr("导入ppt或pdf文件。导入ppt功能必须要求系统安装Office或者WPS."));
	connect(_import, &TitleButton::clicked, this, &KeTitleWidget::OnImportTriggered);
#endif

	_time = new QLabel();
	_time->setFixedWidth(30);
	_time->setText(tr("00:00"));

	QShortcut *f5 = new QShortcut(QKeySequence(Qt::Key_F5), this);
	f5->setContext(Qt::ApplicationShortcut);
	connect(f5, &QShortcut::activated, _record, &QPushButton::click);


	QShortcut *pageUp = new QShortcut(QKeySequence(Qt::Key_PageUp), this);
	pageUp->setContext(Qt::ApplicationShortcut);
	connect(pageUp, &QShortcut::activated, this, &KeTitleWidget::OnPreviousPage);

	QShortcut *pageDown = new QShortcut(QKeySequence(Qt::Key_PageDown), this);
	pageDown->setContext(Qt::ApplicationShortcut);
	connect(pageDown, &QShortcut::activated, this, &KeTitleWidget::OnNextPage);


	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->addWidget(_time);
	mainLayout->addWidget(_record);
	mainLayout->addWidget(_volume);
	mainLayout->addWidget(_insert);
	mainLayout->addWidget(_new);
	mainLayout->addWidget(_switch);
	mainLayout->addWidget(_preview);
#ifdef QT_DEBUG
	//mainLayout->addWidget(_test);
#endif
#ifdef _LiangshiDesktop
	mainLayout->addWidget(_import);
#endif
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	setFixedWidth(360);
	setFixedHeight(22);
	setLayout(mainLayout);

	BanBrowserDialog* dialog = BanBrowserDialog::Instance();
	connect(dialog, &BanBrowserDialog::OnScFileSelected, this, &KeTitleWidget::OnInsertScFile);
}

KeTitleWidget::~KeTitleWidget()
{
	if(_test != NULL)
	{
		_test->deleteLater();
		_test = NULL;
	}

}

void KeTitleWidget::OnRecrodButtonTriggered()
{
	KeTime* keTime = _keTime;

	//当前状态
	if(_paused)
	{
		keTime->Start();
		NotifyWindow* notify = NotifyWindow::Instance();
		notify->ShowNotify(tr("开始录制"));
	}
	else
	{
		keTime->Pause();
		NotifyWindow* notify = NotifyWindow::Instance();
		notify->ShowNotify(tr("暂停录制"));
	}
	_preview->setDisabled(!_paused);
}

void KeTitleWidget::OnStartTriggered()
{
	_record->setText(tr("暂停"));
	//_new->setDisabled(false);
	//_switch->setDisabled(false);
	_paused = false;
}

void KeTitleWidget::OnPauseTriggered()
{
	_record->setText(tr("录制"));
	//_new->setDisabled(true);
	//_switch->setDisabled(true);
	_paused = true;
}

void KeTitleWidget::OnInsertTriggered()
{
	KeTime* keTime = _keTime;
	bool needContinue = !keTime->IsPausing();
	keTime->Pause();
	BanBrowserDialog* dialog = BanBrowserDialog::Instance();
	dialog->ShowModal();
	if(needContinue)
		keTime->Start();
}

void KeTitleWidget::OnNewPageTriggered()
{
	Q_EMIT OnNewPage();
}

void KeTitleWidget::OnSwitchPageTriggered()
{
	Q_EMIT OnSwitchPage();
}

void KeTitleWidget::OnPreviewTriggered()
{
	Q_EMIT OnPreview();
}

void KeTitleWidget::OnAddTestTriggered()
{
	KeTime* keTime = _keTime;
	keTime->Pause();
	QString path = QFileDialog::getOpenFileName(this, QObject::tr("打开试题板书"), ".", QObject::tr("板书文件(*.ban)"));
	if(!path.isEmpty())
	{
		//verify whether it is test sc;
		BanDocument* document = BanFile::LoadAsScDocument(path);
		TestBaseWidget* test = dynamic_cast<TestBaseWidget*>(document);
		if(test != NULL)
		{
			test->deleteLater();
			Q_EMIT OnAttachScFile(path);
		}
		else
		{
			ChineseMessageBox::Show(tr("打开的文件不是试题板书"), tr("失败"), ChineseMessageBox::OK, ChineseMessageBox::Error);
		}
	}
}

#ifdef _LiangshiDesktop
void KeTitleWidget::OnImportTriggered()
{
	KeTime* keTime = _keTime;
	bool needContinue = !keTime->IsPausing();
	keTime->Pause();
	QString path = QFileDialog::getOpenFileName(NULL, QObject::tr("打开幻灯片文件"), ".", QObject::tr("所有支持文件(*.ppt *.pptx *.pdf);;微软演示格式(*.ppt *.pptx);;Pdf格式(*.pdf)"));
	SlidePresenter* presenter = NULL;
	if(!path.isEmpty())
	{
		path = QDir::toNativeSeparators(path);
		if(path.contains(".pdf"))
		{
			PdfAnalyser pdf;
			presenter = pdf.GeneratePresenter(path);

		}
		else if(path.contains(".ppt"))
		{
			PptAnalyser ppt;
			presenter = ppt.GeneratePresenter(path);
		}
	}

	if(presenter != NULL)
	{
		NotifyWindow* notify = NotifyWindow::Instance();
		notify->ShowNotify(QString(tr("导入成功,共%1页幻灯片")).arg(presenter->GetSlideCount()));
		Q_EMIT OnAddSlidePresenter(presenter);
	}
	else
	{
		NotifyWindow* notify = NotifyWindow::Instance();
		notify->ShowNotify(tr("导入失败"));
	}
	if(needContinue)
		keTime->Start();
}
#endif

void KeTitleWidget::OnAudioVolumeTriggered(int percent)
{
	_volume->setValue(percent);
}

void KeTitleWidget::OnTimeUpdateTriggered(int time)
{
	KeTime* keTime = _keTime;

	if(false)//keTime->IsPausing())
	{
		_time->setText(tr("暂停中"));
	}
	else
	{
		QDateTime t1 = QDateTime::fromMSecsSinceEpoch(time);
		//t1.setTimeSpec(Qt::OffsetFromUTC);
		QDateTime t2 = t1.toUTC();
		//_time->setText(t2.toString("hh:mm:ss"));
		_time->setText(t2.toString("mm:ss"));
	}
}

