#include "LiveTitleWidget.h"
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

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

LiveTitleWidget::LiveTitleWidget(QWidget *parent):
QWidget(parent),
_initFromNet(false)
{
	_localVolume = new AudioVolumeIndicator(tr("本地"));
	_localVolume->setFixedWidth(50);
	_remoteVolume = new AudioVolumeIndicator(tr("远程"));
	_remoteVolume->setFixedWidth(50);

	_insert = new TitleButton(tr("插入板书"));
	_insert->setToolTip(tr("插入别的板书"));
	connect(_insert, &TitleButton::clicked, this, &LiveTitleWidget::OnInsertTriggered);

	_new = new TitleButton(tr("新建黑板"));
	_new->setToolTip(tr("新建黑板"));
	connect(_new, &TitleButton::clicked, this, &LiveTitleWidget::OnNewPageTriggered);

	_switch = new TitleButton(tr("切换黑板"));
	_switch->setToolTip(tr("切换黑板"));
	connect(_switch, &TitleButton::clicked, this, &LiveTitleWidget::OnSwitchPageTriggered);

#ifdef _LiangshiDesktop
	_import = new TitleButton(tr("导入"));
	_import->setToolTip(tr("导入ppt或pdf文件。导入ppt功能必须要求系统安装Office或者WPS."));
	connect(_import, &TitleButton::clicked, this, &LiveTitleWidget::OnImportTriggered);
#endif

	_apply = new TitleButton(tr("申请黑板"));
	_apply->setToolTip(tr("向老师申请使用黑板"));
	connect(_apply, &TitleButton::clicked, this, &LiveTitleWidget::OnApplyTriggered);

	_take = new TitleButton(tr("收回黑板"));
	_take->setToolTip(tr("收回黑板使用权"));
	connect(_take, &TitleButton::clicked, this, &LiveTitleWidget::OnTakeTriggered);

	QShortcut *pageUp = new QShortcut(QKeySequence(Qt::Key_PageUp), this);
	pageUp->setContext(Qt::ApplicationShortcut);
	connect(pageUp, &QShortcut::activated, this, &LiveTitleWidget::OnPreviousPage);

	QShortcut *pageDown = new QShortcut(QKeySequence(Qt::Key_PageDown), this);
	pageDown->setContext(Qt::ApplicationShortcut);
	connect(pageDown, &QShortcut::activated, this, &LiveTitleWidget::OnNextPage);

	_mainLayout = new QHBoxLayout();
	//mainLayout->addWidget(_localVolume);
	//mainLayout->addWidget(_remoteVolume);
	_mainLayout->addWidget(_insert);
	_mainLayout->addWidget(_new);
	_mainLayout->addWidget(_switch);
#ifdef QT_DEBUG
	//_mainLayout->addWidget(_test);
#endif
#ifdef _LiangshiDesktop
	_mainLayout->addWidget(_import);
#endif
	//_mainLayout->addWidget(_apply);
	//_mainLayout->addWidget(_take);
	_mainLayout->setSpacing(0);
	_mainLayout->setContentsMargins(0, 0, 0, 0);

	setFixedWidth(360);
	setFixedHeight(22);
	setLayout(_mainLayout);

	BanBrowserDialog* dialog = BanBrowserDialog::Instance();
	connect(dialog, &BanBrowserDialog::OnScFileSelected, this, &LiveTitleWidget::OnInsertScFile);

	//hack
	_initFromNet = true;
	SetEnabled(false, true, false);
	_initFromNet = false;
}

LiveTitleWidget::~LiveTitleWidget()
{
	if(_apply != NULL)
	{
		_apply->deleteLater();
		_apply = NULL;
	}
	if(_take != NULL)
	{
		_take->deleteLater();
		_take = NULL;
	}
	if(_import != NULL)
	{
		_import->deleteLater();
		_import = NULL;
	}
	if(_localVolume != NULL)
	{
		_localVolume->deleteLater();
		_localVolume = NULL;
	}
	if(_remoteVolume != NULL)
	{
		_remoteVolume->deleteLater();
		_remoteVolume = NULL;
	}
}

void LiveTitleWidget::SetEnabled(bool whole, bool student, bool apply)
{
	_insert->setEnabled(whole);
	_new->setEnabled(whole);
	_switch->setEnabled(whole);
#ifdef _LiangshiDesktop
	_import->setEnabled(whole);
#endif

	if(!_initFromNet)
	{
		if(student)
		{
			_mainLayout->addWidget(_apply);
		}
		else
		{
			_mainLayout->addWidget(_take);
		}
		_initFromNet = true;
	}
	_apply->setEnabled(apply);
}

void LiveTitleWidget::OnInsertTriggered()
{
	BanBrowserDialog* dialog = BanBrowserDialog::Instance();
	dialog->ShowModal();
}

void LiveTitleWidget::OnNewPageTriggered()
{
	Q_EMIT OnNewPage();
}

void LiveTitleWidget::OnSwitchPageTriggered()
{
	Q_EMIT OnSwitchPage();
}

void LiveTitleWidget::OnApplyTriggered()
{
	Q_EMIT OnApplyBoard();
}

void LiveTitleWidget::OnTakeTriggered()
{
	Q_EMIT OnTakeBoard();
}

#ifdef _LiangshiDesktop
void LiveTitleWidget::OnImportTriggered()
{
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
}
#endif

void LiveTitleWidget::OnAudioVolumeTriggered(int percent)
{
	_localVolume->setValue(percent);
}
