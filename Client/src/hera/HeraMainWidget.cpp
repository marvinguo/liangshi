#include "HeraMainWidget.h"
#include "BanBaseWidget.h"
#ifdef _LiangshiDesktop
	#include "NotMobile/BanScriptWidget.h"
#endif
#include "TestBaseWidget.h"
#include "KeWidget.h"
#include "LiveWidget.h"
#include "PlayerWidget.h"
#include "TitleWidget.h"
#include "BanElement.h"
#include "BanFile.h"
#include "KeFile.h"
#include "ChineseMessageBox.h"
#include "HeraDocManager.h"
#include "../common/FileUtil.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

HeraMainWidget::HeraMainWidget(NewableDocType type, QWidget* docWidget):
QDialog(NULL),
_keTime(new KeTime(topLevelWidget())),
_mainDoc(NULL),
_title(new TitleWidget(_keTime, type)),
_type(type)
{
	//	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
	setWindowFlags(Qt::FramelessWindowHint);

	//setWindowFlags(this->windowFlags() | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
//	setWindowState(this->windowState() | Qt::WindowFullScreen);

	_title->OnSetTitleTriggered(tr("无标题"), false);

	QFile file(":/qss/default");
	file.open(QFile::ReadOnly);
	QString styleSheet = QString::fromLatin1(file.readAll());
	file.close();
	setStyleSheet(styleSheet);
	
	//草，这个问题也太严重了
	//setModal(true);
	setAttribute(Qt::WA_DeleteOnClose, true);

	_centerLayout = new QVBoxLayout();
	_centerLayout->setSpacing(0);
	_centerLayout->setContentsMargins(1, 0, 1, 1);

	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->addWidget(_title);
	mainLayout->addLayout(_centerLayout);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	setLayout(mainLayout);

	QDesktopWidget *desktop = QApplication::desktop();
	resize(desktop->availableGeometry().width(), desktop->availableGeometry().height());

	QWidget* widget = docWidget;
	if(_type == NewableDocType_Ban)
	{
		setWindowIcon(QIcon(":/App/collect"));
		setWindowTitle(tr("制作板书"));
		if(widget == NULL)
			widget = new BanBaseWidget();
		setAcceptDrops(true);
	}
#ifdef _LiangshiDesktop
	else if(_type == NewableDocType_BanScript)
	{
		setWindowIcon(QIcon(":/App/collect"));
		setWindowTitle(tr("制作程序板书"));
		if(widget == NULL)
			widget = new BanScriptWidget();
	}
#endif
	else if(_type == NewableDocType_BanTest)
	{
		setWindowIcon(QIcon(":/App/collect"));
		setWindowTitle(tr("制作试题板书"));
		if(widget == NULL)
			widget = new TestBaseWidget();
	}
	else if(_type == NewableDocType_Ke || _type == NewableDocType_KeEmbedded)
	{
		setWindowIcon(QIcon(":/App/collect"));
		setWindowTitle(tr("录制课件"));
		if(widget == NULL)
			widget = new KeWidget(_keTime);
		setAcceptDrops(true);

		//和戴讨论了一下，限制窗口大小
		int width = BOARD_WIDTH;
		int height = BOARD_HEIGHT;
		if(desktop->availableGeometry().width() < width)
			width = desktop->availableGeometry().width();
		if(desktop->availableGeometry().height() < height)
			height = desktop->availableGeometry().height();
		resize(width, height);
	}
	else if(_type == NewableDocType_BanEmbedded)
	{
		setWindowIcon(QIcon(":/App/collect"));
		setWindowTitle(tr("制作嵌入板书"));
		if(widget == NULL)
			widget = new BanBaseWidget();
	}
	else if(_type == NewableDocType_TestEmbedded)
	{
		setWindowIcon(QIcon(":/App/collect"));
		setWindowTitle(tr("制作试题板书"));
		if(widget == NULL)
			widget = new TestBaseWidget();
	}
	else if(_type == NewableDocType_Live)
	{
		setWindowIcon(QIcon(":/App/live"));
		setWindowTitle(tr("直播辅导"));
		if(widget == NULL)
			widget = new LiveWidget();
		setAcceptDrops(true);
		bool hasMax = false;
		QStringList arguments = QCoreApplication::arguments();
		if (arguments.size() >= 3)
		{
			hasMax = arguments[2].toInt() == 1;
		}
		if(hasMax)
		{
			setSizeGripEnabled(true);
		}
	}
	else
	{
		if(_type != NewableDocType_PlayerPreview)
			setAcceptDrops(true);
		if(_type != NewableDocType_PlayerInBrowser)
			setSizeGripEnabled(true);

		setWindowIcon(QIcon(":/App/player"));
		setWindowTitle(tr("课件播放器"));

		if(widget == NULL)
			widget = new PlayerWidget();
	}
	_centerLayout->addWidget(widget);
	_mainDoc = dynamic_cast<HeraDocument*>(widget);
	InitSlots();
	InitKeTimeSlots();
	
	HeraDocManager* manager = HeraDocManager::Instance();
	manager->AddHera(this);
}

HeraMainWidget::~HeraMainWidget()
{
	HeraDocManager* manager = HeraDocManager::Instance();
	manager->RemoteHera(this);
}

void HeraMainWidget::InitSlots()
{
	connect(_title->GetFileWidget()->GetSave(), &OneImageButton::clicked, this, &HeraMainWidget::OnSaveTriggered);
	connect(_title->GetFileWidget()->GetSaveAs(), &OneImageButton::clicked, this, &HeraMainWidget::OnSaveAsTriggered);

#ifdef _LiangshiDesktop
	if(_type == NewableDocType_BanScript)
	{
		connect(_title->GetScriptWidget()->GetRun(), &QPushButton::clicked, (BanScriptWidget*)_mainDoc, &BanScriptWidget::OnRunScriptTriggered);
		connect(_title->GetScriptWidget()->GetDebug(), &QPushButton::clicked, (BanScriptWidget*)_mainDoc, &BanScriptWidget::OnScriptDebuggerTriggered);
		connect(_title->GetScriptWidget()->GetEditor(), &QPushButton::clicked, (BanScriptWidget*)_mainDoc, &BanScriptWidget::OnScriptEditorTriggered);
		connect(_title->GetScriptWidget()->GetDeps(), &QPushButton::clicked, (BanScriptWidget*)_mainDoc, &BanScriptWidget::OnShowDeps);

		connect((BanScriptWidget*)_mainDoc, &BanScriptWidget::OnSaveFileNameChanged, _title, &TitleWidget::OnSetTitleTriggered);
	}
#endif

	if(_type == NewableDocType_Ban || _type == NewableDocType_BanEmbedded)
	{
		connect((BanBaseWidget*)_mainDoc, &BanBaseWidget::OnSaveFileNameChanged, _title, &TitleWidget::OnSetTitleTriggered);

		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnDraw, (BanBaseWidget*)_mainDoc, &BanBaseWidget::DrawModeTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnLine, (BanBaseWidget*)_mainDoc, &BanBaseWidget::LineModeTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnRect, (BanBaseWidget*)_mainDoc, &BanBaseWidget::RectModeTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnPRect, (BanBaseWidget*)_mainDoc, &BanBaseWidget::PerectRectModeTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnEllipse, (BanBaseWidget*)_mainDoc, &BanBaseWidget::EclipseModeTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnPEllipse, (BanBaseWidget*)_mainDoc, &BanBaseWidget::PerfectEclipseModeTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnSelect, (BanBaseWidget*)_mainDoc, &BanBaseWidget::SelectModeTriggered);
		connect(_title->GetDrawingToolsWidget()->GetPasteButton(), &QPushButton::clicked, (BanBaseWidget*)_mainDoc, &BanBaseWidget::OnPasteTriggered);
		connect(_title->GetDrawingToolsWidget()->GetUndoButton(), &QPushButton::clicked, (BanBaseWidget*)_mainDoc, &BanBaseWidget::OnUndoTriggered);
		connect(_title->GetDrawingToolsWidget()->GetRedoButton(), &QPushButton::clicked, (BanBaseWidget*)_mainDoc, &BanBaseWidget::OnRedoTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnGetPixmap, (BanBaseWidget*)_mainDoc, &BanBaseWidget::OnInsertPixmapTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnLineWidthChanged, (BanBaseWidget*)_mainDoc, &BanBaseWidget::LineWidthTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnLineStyleChanged, (BanBaseWidget*)_mainDoc, &BanBaseWidget::LineStyleTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnLineColorChanged, (BanBaseWidget*)_mainDoc, &BanBaseWidget::LineColorTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::EnableReviseDraw, (BanBaseWidget*)_mainDoc, &BanBaseWidget::ReviseDrawTriggered);

		connect(_title->GetMaterialWidget(), &BanTitleWidget::OnInsertScFile, (BanBaseWidget*)_mainDoc, &BanBaseWidget::OnInsertBanTriggered);
		connect((BanBaseWidget*)_mainDoc, &BanBaseWidget::OnSaveFileNameChanged, _title, &TitleWidget::OnSetTitleTriggered);
		connect((BanBaseWidget*)_mainDoc, &BanBaseWidget::OnHighlightElementChanged, _title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnHighlightElementChangeTriggered);
	}

	if(_type == NewableDocType_Ke || _type == NewableDocType_KeEmbedded)
	{
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnDraw, (KeWidget*)_mainDoc, &KeWidget::DrawModeTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnLine, (KeWidget*)_mainDoc, &KeWidget::LineModeTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnRect, (KeWidget*)_mainDoc, &KeWidget::RectModeTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnPRect, (KeWidget*)_mainDoc, &KeWidget::PerectRectModeTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnEllipse, (KeWidget*)_mainDoc, &KeWidget::EclipseModeTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnPEllipse, (KeWidget*)_mainDoc, &KeWidget::PerfectEclipseModeTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnSelect, (KeWidget*)_mainDoc, &KeWidget::SelectModeTriggered);
		connect(_title->GetDrawingToolsWidget()->GetPasteButton(), &QPushButton::clicked, (KeWidget*)_mainDoc, &KeWidget::OnPaste);
		connect(_title->GetDrawingToolsWidget()->GetUndoButton(), &QPushButton::clicked, (KeWidget*)_mainDoc, &KeWidget::OnUndo);
		connect(_title->GetDrawingToolsWidget()->GetRedoButton(), &QPushButton::clicked, (KeWidget*)_mainDoc, &KeWidget::OnRedo);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnGetPixmap, (KeWidget*)_mainDoc, &KeWidget::OnInsertPixmapTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnLineWidthChanged, (KeWidget*)_mainDoc, &KeWidget::LineWidthTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnLineStyleChanged, (KeWidget*)_mainDoc, &KeWidget::LineStyleTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnLineColorChanged, (KeWidget*)_mainDoc, &KeWidget::LineColorTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::EnableReviseDraw, (KeWidget*)_mainDoc, &KeWidget::ReviseDrawTriggered);
		connect(_title->GetKeWidget(), &KeTitleWidget::OnInsertScFile, (KeWidget*)_mainDoc, &KeWidget::OnInsertBanTriggered);
		connect(_title->GetKeWidget(), &KeTitleWidget::OnAttachScFile, (KeWidget*)_mainDoc, &KeWidget::OnAttachBanTriggered);
		connect(_title->GetKeWidget(), &KeTitleWidget::OnAddSlidePresenter, (KeWidget*)_mainDoc, &KeWidget::OnAddSlidePresenterTriggered);
		connect(_title->GetKeWidget(), &KeTitleWidget::OnNewPage, (KeWidget*)_mainDoc, &KeWidget::OnNewPageTriggered);
		connect(_title->GetKeWidget(), &KeTitleWidget::OnNewPageWithBackground, (KeWidget*)_mainDoc, &KeWidget::OnNewPageWithBackgroundTriggered);
		connect(_title->GetKeWidget(), &KeTitleWidget::OnSwitchPage, (KeWidget*)_mainDoc, &KeWidget::OnSwitchPageTriggered);
		connect(_title->GetKeWidget(), &KeTitleWidget::OnPreviousPage, (KeWidget*)_mainDoc, &KeWidget::OnPreviousPageTriggered);
		connect(_title->GetKeWidget(), &KeTitleWidget::OnNextPage, (KeWidget*)_mainDoc, &KeWidget::OnNextPageTriggered);
		connect(_title->GetKeWidget(), &KeTitleWidget::OnPreview, this, &HeraMainWidget::OnPreviewTriggered);

		connect((KeWidget*)_mainDoc, &KeWidget::OnSaveFileNameChanged, _title, &TitleWidget::OnSetTitleTriggered);
		connect((KeWidget*)_mainDoc, &KeWidget::OnAudioVolumeChanged, _title->GetKeWidget(), &KeTitleWidget::OnAudioVolumeTriggered);
		connect((KeWidget*)_mainDoc, &KeWidget::OnHighlightElementChanged, _title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnHighlightElementChangeTriggered);
	}

	if(_type == NewableDocType_BanTest || _type == NewableDocType_TestEmbedded)
	{
		_title->GetTestWidget()->InitWithTestWidget((TestBaseWidget*)_mainDoc);
		connect(_title->GetTestWidget()->GetColorButton(), SIGNAL(OnColorChangeed(QColor)), (TestBaseWidget*)_mainDoc, SLOT(LineColorTriggered(QColor)));
		connect((TestBaseWidget*)_mainDoc, &TestBaseWidget::OnSaveFileNameChanged, _title, &TitleWidget::OnSetTitleTriggered);
		connect((TestBaseWidget*)_mainDoc, &TestBaseWidget::OnHighlightElementChanged, _title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnHighlightElementChangeTriggered);
	}

	if(_type == NewableDocType_Player || _type == NewableDocType_PlayerPreview ||_type == NewableDocType_PlayerInBrowser)
	{
		_title->GetPlayerWidget()->Init(this, (PlayerWidget*)_mainDoc);
		connect(_title->GetFileWidget()->GetOpen(), &OneImageButton::clicked, this, &HeraMainWidget::OnOpenFileTriggered);
		connect((PlayerWidget*)_mainDoc, &PlayerWidget::OnSaveFileNameChanged, _title, &TitleWidget::OnSetTitleTriggered);
	}

	if(_type == NewableDocType_Live)
	{
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnDraw, (LiveWidget*)_mainDoc, &LiveWidget::DrawModeTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnLine, (LiveWidget*)_mainDoc, &LiveWidget::LineModeTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnRect, (LiveWidget*)_mainDoc, &LiveWidget::RectModeTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnPRect, (LiveWidget*)_mainDoc, &LiveWidget::PerectRectModeTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnEllipse, (LiveWidget*)_mainDoc, &LiveWidget::EclipseModeTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnPEllipse, (LiveWidget*)_mainDoc, &LiveWidget::PerfectEclipseModeTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnSelect, (LiveWidget*)_mainDoc, &LiveWidget::SelectModeTriggered);
		connect(_title->GetDrawingToolsWidget()->GetPasteButton(), &QPushButton::clicked, (LiveWidget*)_mainDoc, &LiveWidget::OnPaste);
		connect(_title->GetDrawingToolsWidget()->GetUndoButton(), &QPushButton::clicked, (LiveWidget*)_mainDoc, &LiveWidget::OnUndo);
		connect(_title->GetDrawingToolsWidget()->GetRedoButton(), &QPushButton::clicked, (LiveWidget*)_mainDoc, &LiveWidget::OnRedo);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnGetPixmap, (LiveWidget*)_mainDoc, &LiveWidget::OnInsertPixmapTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnGetLocalCamera, (LiveWidget*)_mainDoc, &LiveWidget::OnGetLocalCameraTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnLineWidthChanged, (LiveWidget*)_mainDoc, &LiveWidget::LineWidthTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnLineStyleChanged, (LiveWidget*)_mainDoc, &LiveWidget::LineStyleTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnLineColorChanged, (LiveWidget*)_mainDoc, &LiveWidget::LineColorTriggered);
		connect(_title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::EnableReviseDraw, (LiveWidget*)_mainDoc, &LiveWidget::ReviseDrawTriggered);
		connect(_title->GetLiveWidget(), &LiveTitleWidget::OnInsertScFile, (LiveWidget*)_mainDoc, &LiveWidget::OnInsertBanTriggered);
		connect(_title->GetLiveWidget(), &LiveTitleWidget::OnAddSlidePresenter, (LiveWidget*)_mainDoc, &LiveWidget::OnAddSlidePresenterTriggered);
		connect(_title->GetLiveWidget(), &LiveTitleWidget::OnNewPage, (LiveWidget*)_mainDoc, &LiveWidget::OnNewPageTriggered);
		connect(_title->GetLiveWidget(), &LiveTitleWidget::OnNewPageWithBackground, (LiveWidget*)_mainDoc, &LiveWidget::OnNewPageWithBackgroundTriggered);
		connect(_title->GetLiveWidget(), &LiveTitleWidget::OnSwitchPage, (LiveWidget*)_mainDoc, &LiveWidget::OnSwitchPageTriggered);
		connect(_title->GetLiveWidget(), &LiveTitleWidget::OnPreviousPage, (LiveWidget*)_mainDoc, &LiveWidget::OnPreviousPageTriggered);
		connect(_title->GetLiveWidget(), &LiveTitleWidget::OnNextPage, (LiveWidget*)_mainDoc, &LiveWidget::OnNextPageTriggered);
		connect(_title->GetLiveWidget(), &LiveTitleWidget::OnApplyBoard, (LiveWidget*)_mainDoc, &LiveWidget::OnApplyBoardTriggered);
		connect(_title->GetLiveWidget(), &LiveTitleWidget::OnTakeBoard, (LiveWidget*)_mainDoc, &LiveWidget::OnTakeBoardTriggered);
		connect((LiveWidget*)_mainDoc, &LiveWidget::OnAudioVolumeChanged, _title->GetLiveWidget(), &LiveTitleWidget::OnAudioVolumeTriggered);
		connect((LiveWidget*)_mainDoc, &LiveWidget::OnLiveCanApplyAndTake, this, &HeraMainWidget::OnLiveCanApplyAndTakeTriggered);
		connect((LiveWidget*)_mainDoc, &LiveWidget::OnHighlightElementChanged, _title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnHighlightElementChangeTriggered);
	}
}

void HeraMainWidget::InitKeTimeSlots()
{
	KeTime* keTime = _keTime;
	connect(keTime, &KeTime::OnStart, _title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnStartTriggered);
	connect(keTime, &KeTime::OnReset, _title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnPauseTriggered);
	connect(keTime, &KeTime::OnPause, _title->GetDrawingToolsWidget(), &DrawingToolsTitleWidget::OnPauseTriggered);

	connect(keTime, &KeTime::OnStart, _title->GetPlayerWidget(), &PlayerTitleWidget::OnStartTriggered);
	connect(keTime, &KeTime::OnPause, _title->GetPlayerWidget(), &PlayerTitleWidget::OnPauseTriggered);
	connect(keTime, &KeTime::OnReset, _title->GetPlayerWidget(), &PlayerTitleWidget::OnResetTriggered);
	connect(keTime, &KeTime::OnUpdateCurrentTime, _title->GetPlayerWidget(), &PlayerTitleWidget::OnTimeUpdateTriggered);


	connect(keTime, &KeTime::OnStart, _title->GetKeWidget(), &KeTitleWidget::OnStartTriggered);
	connect(keTime, &KeTime::OnReset, _title->GetKeWidget(), &KeTitleWidget::OnPauseTriggered);
	connect(keTime, &KeTime::OnPause, _title->GetKeWidget(), &KeTitleWidget::OnPauseTriggered);
	connect(keTime, &KeTime::OnUpdateCurrentTime, _title->GetKeWidget(), &KeTitleWidget::OnTimeUpdateTriggered);

	if(_type == NewableDocType_Ke || _type == NewableDocType_KeEmbedded)
	{
		connect(keTime, &KeTime::OnStart, (KeWidget*)_mainDoc, &KeWidget::OnRecordTriggered);
		connect(keTime, &KeTime::OnPause, (KeWidget*)_mainDoc, &KeWidget::OnPauseTriggered);
	}

	if(_type == NewableDocType_Player || _type == NewableDocType_PlayerPreview || _type == NewableDocType_PlayerInBrowser)
	{
		connect(keTime, &KeTime::OnStart, (PlayerWidget*)_mainDoc, &PlayerWidget::OnPlayTriggered);
		connect(keTime, &KeTime::OnPause, (PlayerWidget*)_mainDoc, &PlayerWidget::OnPauseTriggered);
		connect(keTime, &KeTime::OnReset, (PlayerWidget*)_mainDoc, &PlayerWidget::OnResetTriggered);
	}

	if(_type == NewableDocType_Ke ||
		_type == NewableDocType_KeEmbedded ||
		_type == NewableDocType_Player ||
		_type == NewableDocType_PlayerPreview ||
		_type == NewableDocType_PlayerInBrowser)
	{
		keTime->Reset();
	}
}

void HeraMainWidget::OnLiveCanApplyAndTakeTriggered(bool whole, bool student, bool apply)
{
	_title->GetDrawingToolsWidget()->setEnabled(whole);
	_title->GetLiveWidget()->SetEnabled(whole, student, apply);
}

void HeraMainWidget::dragEnterEvent(QDragEnterEvent *e)
{
	const QMimeData *mimeData = e->mimeData();
	if(mimeData->hasUrls())
	{
		QList<QUrl> urls = mimeData->urls();
		for(QList<QUrl>::iterator i = urls.begin(); i != urls.end(); ++i)
		{
			QUrl url = *i;
			//检测文件名，然后使用相应功能
			QString filepath = url.toLocalFile();
			QFileInfo info(filepath);
			QString extension = info.suffix();
			
			if(_type ==  NewableDocType_Player && extension == tr("ke"))
			{
				e->acceptProposedAction();
			}

			if(_type ==  NewableDocType_Ban || _type ==  NewableDocType_Ke || _type == NewableDocType_Live)
			{
				if(extension == tr("ban"))
					e->acceptProposedAction();

				if(extension == tr("jpg") || extension == tr("jpeg") || extension == tr("png") || extension == tr("gif") || extension == tr("bmp"))
					e->acceptProposedAction();
			}
			break;
		}
	}
}

void HeraMainWidget::dropEvent(QDropEvent *e)
{
	const QMimeData *mimeData = e->mimeData();
	if(mimeData->hasUrls())
	{
		QList<QUrl> urls = mimeData->urls();
		for(QList<QUrl>::iterator i = urls.begin(); i != urls.end(); ++i)
		{
			QUrl url = *i;
			QString filepath = url.toLocalFile();
			QFileInfo info(filepath);
			QString extension = info.suffix();
			if(_type ==  NewableDocType_Player && extension == tr("ke"))
			{
				Q_EMIT OnOpenKeFile(filepath);
			}

			if(extension == tr("ban"))
			{
				if(_type ==  NewableDocType_Ban)
				{
					BanBaseWidget* widget = (BanBaseWidget*)_mainDoc;
					widget->OnInsertBanTriggered(filepath);
				}
				if(_type ==  NewableDocType_Ke)
				{
					KeWidget* widget = (KeWidget*)_mainDoc;
					widget->OnInsertBanTriggered(filepath);

				}
				if(_type == NewableDocType_Live)
				{
					LiveWidget* widget = (LiveWidget*)_mainDoc;
					widget->OnInsertBanTriggered(filepath);
				}
			}

			if(extension == tr("jpg") || extension == tr("jpeg") || extension == tr("png") || extension == tr("gif") || extension == tr("bmp"))
			{
				QPixmap pixmap;
				if(pixmap.load(filepath))
				{
					if(_type ==  NewableDocType_Ban)
					{
						BanBaseWidget* widget = (BanBaseWidget*)_mainDoc;
						widget->OnInsertPixmapTriggered(pixmap);
					}
					if(_type ==  NewableDocType_Ke)
					{
						KeWidget* widget = (KeWidget*)_mainDoc;
						widget->OnInsertPixmapTriggered(pixmap);

					}
					if(_type == NewableDocType_Live)
					{
						LiveWidget* widget = (LiveWidget*)_mainDoc;
						widget->OnInsertPixmapTriggered(pixmap);
					}
				}
			}
			break;
		}
	}
}

void HeraMainWidget::resizeEvent(QResizeEvent *e)
{
	//这个是在手机里的，非常重要的东西
	QWidget::resizeEvent(e);

	QSize size = e->size();
}

HeraMainWidget* HeraMainWidget::LoadScFile(QString filepath)
{
	HeraMainWidget* hera = NULL;
	QFileInfo info(filepath);
	if(info.exists())
	{
		BanDocument* scDoc = BanFile::LoadAsScDocument(filepath);
		QWidget* widget = NULL;
		if(scDoc != NULL)
		{
			widget = scDoc->Convert2QWidget();
		}

		if(widget != NULL)
		{
			if(scDoc->GetType() == ScType_Basic)
			{
				hera = new HeraMainWidget(NewableDocType_Ban, widget);
			}

			if(scDoc->GetType() == ScType_Script)
			{
				hera = new HeraMainWidget(NewableDocType_BanScript, widget);
			}

			if(scDoc->GetType() == ScType_Test)
			{
				hera = new HeraMainWidget(NewableDocType_BanTest, widget);
				scDoc->UpdateDataFromMsgPack();
			}
			scDoc->UpdateFilepath(filepath);
		}
		else
		{
		}
	}
	return hera;
}

HeraMainWidget* HeraMainWidget::LoadKeFile(QString filepath, bool embeddInBrowser, bool preview)
{
	HeraMainWidget* hera = NULL;
	QFileInfo info(filepath);
	if(info.exists())
	{
		PlayerWidget* scDoc = KeFile::LoadKeAsPlayerWidget(filepath);
		QWidget* widget = NULL;
		if(scDoc != NULL)
		{
			widget = scDoc->Convert2QWidget();
		}

		if(widget != NULL)
		{
			if(embeddInBrowser)
			{
				hera = new HeraMainWidget(NewableDocType_PlayerInBrowser, widget);
			}
			else
			{
				if(preview)
				{
					hera = new HeraMainWidget(NewableDocType_PlayerPreview, widget);
				}
				else
				{
					hera = new HeraMainWidget(NewableDocType_Player, widget);
				}
			}
			scDoc->_keTime = hera->_keTime;
			hera->_title->GetPlayerWidget()->Enable(true);
			hera->_title->GetPlayerWidget()->SetTotalTime(scDoc->GetTotalTime());
			scDoc->UpdateFilepath(filepath);

			//这有个bug,load完，界面不刷新
			scDoc->OnGotoPositionTriggered(0.00001);

			if(preview)
			{
				//模拟点击的效果最好，
				scDoc->_keTime->Start();
				//hack,
				hera->_title->GetPlayerWidget()->GetPlayButton()->SetChecked(true);
				//hera->_title->GetPlayerWidget()->GetPlayButton()->click();
			}
		}
	}
	return hera;
}

HeraMainWidget* HeraMainWidget::BeginKeFromLokiBuffer(NewableDocType type, QPixmap& pixmap, QString taskBartitle)
{
	HeraMainWidget* hera = new HeraMainWidget(type);
	hera->setWindowTitle(taskBartitle);
	//hera->_title->OnSetTitleTriggered(title);
	KeWidget* widget = dynamic_cast<KeWidget*>(hera->_mainDoc);
	if(widget != NULL)
	{
//		widget->UpdateFilepath(title);
		widget->InitKe(pixmap);
	}
	return hera;
}

HeraMainWidget* HeraMainWidget::LoadFromLokiBuffer(NewableDocType type, LokiBuffer& buffer, QVariant& relatedData, QString title)
{
	HeraMainWidget* hera = new HeraMainWidget(type);
	hera->_relatedData = relatedData;
	hera->_title->OnSetTitleTriggered(title, false);
	BanBaseWidget* widget = dynamic_cast<BanBaseWidget*>(hera->_mainDoc);
	if(widget != NULL)
	{
		widget->UpdateFilepath(title);
		widget->SetMsgPackData(buffer);
		widget->UpdateDataFromMsgPack();
		widget->SetModifiedTriggered(true);
	}
	return hera;
}

HeraMainWidget* HeraMainWidget::LoadFromLokiBufferInLayer(NewableDocType type, LokiBuffer& buffer, QVariant& relatedData, int layer)
{
	HeraMainWidget* hera = new HeraMainWidget(type);
	hera->_relatedData = relatedData;
	BanBaseWidget* widget = dynamic_cast<BanBaseWidget*>(hera->_mainDoc);
	if(widget != NULL)
	{
		widget->SetMsgPackData(buffer);
		widget->UpdateDataFromMsgPack();
		widget->SetLayer(layer);
	}
	return hera;
}

void HeraMainWidget::ShowModal()
{
	show();
	//exec();
}

//这个open不对，如果是这里open的话，应该是打开新的player, 然后关闭当前这个
void HeraMainWidget::OnOpenFileTriggered()
{
	QString path = QFileDialog::getOpenFileName(this, QObject::tr("打开课件文件"), ".", QObject::tr("课件文件(*.ke)"));
	if(!path.isEmpty())
	{
		Q_EMIT OnOpenKeFile(path);
	}
}

void HeraMainWidget::OnSaveTriggered()
{
	if(_mainDoc != NULL)
	{
		if(_type == NewableDocType_BanEmbedded ||
			_type == NewableDocType_TestEmbedded)
		{
			BanBaseWidget* base = dynamic_cast<BanBaseWidget*>(_mainDoc);
			if(base != NULL)
			{
				LokiBuffer buffer = base->GetMsgPackData();
				Q_EMIT OnGetScBuffer(buffer, _relatedData, false);
				base->SetModifiedTriggered(false);
				base->show();
			}
		}
		else
		{
			if(!_mainDoc->HasSavedPath())
			{
				KeTime* keTime = _keTime;
				bool needContinue = !keTime->IsPausing();
				keTime->Pause();
				QString path = QFileDialog::getSaveFileName(this, tr("保存文件"), ".", _mainDoc->GetSaveFileDialongExtension());
				if(!path.isEmpty())
				{
					_mainDoc->SaveDocument(path);
				}
				if(needContinue)
					keTime->Start();
			}
			else
			{
				_mainDoc->SaveDocument();
			}
		}
	}
}

void HeraMainWidget::OnSaveAsTriggered()
{
	if(_mainDoc != NULL)
	{
		KeTime* keTime = _keTime;
		bool needContinue = !keTime->IsPausing();
		keTime->Pause();
		QString path = QFileDialog::getSaveFileName(this, tr("另存为文件"), ".", _mainDoc->GetSaveFileDialongExtension());
		if(!path.isEmpty())
		{
			_mainDoc->SaveAsDocument(path);
		}
		if(needContinue)
			keTime->Start();
	}
}

void HeraMainWidget::OnPreviewTriggered()
{
	if(_mainDoc != NULL)
	{
		QString filename = QUuid::createUuid().toString();
		QString path = FileUtil::GetTempFolder() + filename;
		if(!path.isEmpty())
		{
			_mainDoc->SaveAsDocument(path);

			QProcess* process = new QProcess(this);
			process->setWorkingDirectory(QCoreApplication::applicationDirPath());
			QStringList arguments;
			arguments << tr("-preview");
			arguments << path;
			//process->setArguments(arguments);
			process->start("player.exe", arguments);
		}
	}
}

void HeraMainWidget::keyPressEvent(QKeyEvent *e)
{
	if(e->key() == Qt::Key_Escape)
	{
		e->ignore();
		return;
	}

	return QDialog::keyPressEvent(e);
}

void HeraMainWidget::closeEvent(QCloseEvent * event)
{
	if(_mainDoc != NULL)
	{
		if(!_mainDoc->CanQuit())
		{
			int rb = ChineseMessageBox::Show(tr("是否保存?"), tr("保存文件"), ChineseMessageBox::YesNoCancel, ChineseMessageBox::Question);
			if(rb == QMessageBox::Yes)
			{
				if(_type == NewableDocType_BanEmbedded ||
					_type == NewableDocType_TestEmbedded)
				{
					BanBaseWidget* base = dynamic_cast<BanBaseWidget*>(_mainDoc);
					if(base != NULL)
					{
						LokiBuffer buffer = base->GetMsgPackData();
						Q_EMIT OnGetScBuffer(buffer, _relatedData, true);
					}
				}
				else
				{
					QString path = QFileDialog::getSaveFileName(this, tr("保存文件"), ".", _mainDoc->GetSaveFileDialongExtension());
					if(!path.isEmpty())
					{
						_mainDoc->SaveDocument(path);
					}
				}
			}
			if(rb == QMessageBox::No)
			{
				if(_type == NewableDocType_BanEmbedded ||
					_type == NewableDocType_TestEmbedded)
				{
					Q_EMIT OnHavenotSaveRelatedElement(_relatedData);
				}
			}

			if(rb == QMessageBox::Cancel)
			{
				event->ignore();
				return;
			}
		}
		else
		{
			LokiBuffer buffer;
			Q_EMIT OnGetScBuffer(buffer, _relatedData, true);
		}
	}
	QDialog::closeEvent(event);
	Q_EMIT OnClosed();
}

void HeraMainWidget::OnShowTriggered()
{
	QThread::msleep(200);
	show();
}
