#include "KeWidget.h"
#include "../common/library.h"
#include "../common/FileUtil.h"

#include "KeTime.h"
#include "KePages.h"
#include "KeFile.h"

#include "DrawElement.h"
#include "LineElement.h"
#include "RectElement.h"
#include "EllipseElement.h"

#include "SwitchPageAction.h"
#include "ChineseMessageBox.h"
#include "NotifyWindow.h"
#include "SlidePresenterManager.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

KeWidget::KeWidget(KeTime* keTime, QWidget *parent):
QWidget(parent),
_keTime(keTime),
_hades(NULL),
_hasStarted(false),
_currentPage(NULL),
_cachedMode(DrawMode_Draw),
_cachedWidth(2),
_cachedStyle(Qt::SolidLine),
_cachedColor(QColor(Qt::white)),
_cachedReviseDraw(false),
_dirty(false),
_filepath(tr("无标题"))
{
	_keTime->DisableSeltTime();

	_mainLayout = new QStackedLayout();
	_mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(_mainLayout);

	KePages* pages = KePages::Instance();
	connect(pages, &KePages::OnSelectPage, this, &KeWidget::OnSelectPageTriggered);

	OnNewPageTriggered();
}

KeWidget::~KeWidget()
{
	if(_hades != NULL)
	{
		_hades->SignalRecordData.disconnect_all();
		_hades->Release();
		delete _hades;
		_hades = NULL;
	}
	QFile::remove(_hadesFile);

	for(vector<AnimationAction*>::iterator i = _actions.begin(); i != _actions.end(); ++i)
	{
		delete *i;
	}
	_actions.clear();
	
	//2014.10.31 allPage为啥不析构？？
	//利用stack析构的
	/*
	for(vector<ScBaseWidget*>::iterator i = _allPages.begin(); i < _allPages.end(); ++i)
	{
		(*i).deleteLater();
	}
	_allPages.clear();	
	*/
}

//from medial.dll
void KeWidget::OnAudioVolume(int percent)
{
	//qDebug() << percent;
	Q_EMIT OnAudioVolumeChanged(percent);
}

void KeWidget::OnRecordTime(int time)
{
	_keTime->OnHadesTime(time);
}

LokiBuffer KeWidget::GetMsgPackData()
{
	if(_hades != NULL)
		_hades->FlushFile();
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

void KeWidget::SetMsgPackData(LokiBuffer& buffer)
{
}

void KeWidget::InitKe(QPixmap& pixmap)
{
	QUuid oldUuid = _currentPage->_uuid;
	
	//不采用这种方式了，采用背景图的做法
	//_currentPage->SetMsgPackData(buffer);
	_currentPage->OnInsertPixmapBackgroundTriggered(pixmap);
	_currentPage->UpdateDataFromMsgPack();

	//SwitchPage action 里的uuid是上一次的uuid,而新的页面，需要uuid
	_currentPage->_uuid = oldUuid;

	//加入到里面的element,需要action
	for(list<Element*>::iterator i = _currentPage->_drawingElements.begin(); i != _currentPage->_drawingElements.end(); ++i)
	{
		Element* element = *i;
		ShowAction showAction;
		showAction.SetTartgetBanUuid(_currentPage->GetObjectUuid().toStdString());
		showAction.SetTartgetElementUuid(element->GetObjectUuid());
		AddActionTriggered(showAction, element);
	}
}

void KeWidget::InitHadesFromExistingFile(QString hadesFile, int seek, int time)
{
	if(_hades == NULL)
	{
		_hadesFile = hadesFile;
		_hades = new Hades();
		_hades->SignalRecordData.connect(this, &KeWidget::OnAudioVolume);
		_hades->RecordTimeUpdate.connect(this, &KeWidget::OnRecordTime);
		_hades->InitRecorder(_hadesFile.toUtf8().data(), seek, time);
		//keTime也需要更新
		_keTime->SetAnimatedTime(time);
		_keTime->Pause();
	}
}

bool KeWidget::CanQuit()
{
	return !_dirty;
}

QString KeWidget::GetSaveFileDialongExtension()
{
	return tr("课件文件(*.ke)");
}

void KeWidget::UpdateFilepath(QString filepath)
{
	_filepath = filepath;
	Q_EMIT OnSaveFileNameChanged(FileUtil::GetFilename(_filepath), false);
}

bool KeWidget::HasSavedPath()
{
	QFileInfo info(_filepath);
	return info.exists();
}

bool KeWidget::SaveDocument(QString filepath)
{
	KeTime* keTime = _keTime;
	_totalTime = keTime->GetAnimatedTime();

	if(filepath == "")
		filepath = _filepath;

	KeFile file(this);
	file.SaveFile(filepath);
	_filepath = filepath;
	_dirty = false;
	Q_EMIT OnSaveFileNameChanged(FileUtil::GetFilename(_filepath), false);
	NotifyWindow* notify = NotifyWindow::Instance();
	notify->ShowNotify(tr("保存成功"));
	return true;
}

bool KeWidget::SaveAsDocument(QString filepath)
{
	KeTime* keTime = _keTime;
	_totalTime = keTime->GetAnimatedTime();

	KeFile file(this);
	file.SaveFile(filepath);
	return true;
}

void KeWidget::DrawModeTriggered()
{
	_currentPage->DrawModeTriggered();
	_cachedMode = DrawMode_Draw;
}

void KeWidget::LineModeTriggered()
{
	_currentPage->LineModeTriggered();
	_cachedMode = DrawMode_Line;
}

void KeWidget::RectModeTriggered()
{
	_currentPage->RectModeTriggered();
	_cachedMode = DrawMode_Rect;
}

void KeWidget::PerectRectModeTriggered()
{
	_currentPage->PerectRectModeTriggered();
	_cachedMode = DrawMode_PerfectRect;
}

void KeWidget::EclipseModeTriggered()
{
	_currentPage->EclipseModeTriggered();
	_cachedMode = DrawMode_Ellipse;
}

void KeWidget::PerfectEclipseModeTriggered()
{
	_currentPage->PerfectEclipseModeTriggered();
	_cachedMode = DrawMode_PerfectEllipse;
}

void KeWidget::SelectModeTriggered()
{
	_currentPage->SelectModeTriggered();
	_cachedMode = DrawMode_Select;
}

void KeWidget::LineWidthTriggered(int width)
{
	_currentPage->LineWidthChanged(width);
	_cachedWidth = width;
}

void KeWidget::LineStyleTriggered(int style)
{
	_currentPage->LineStyleChanged(style);
	_cachedStyle = style;
}

void KeWidget::LineColorTriggered(QColor color)
{
	_currentPage->LineColorChanged(color);
	_cachedColor = color;
}

void KeWidget::ReviseDrawTriggered(bool ok)
{
	_currentPage->ReviseDrawTriggered(ok);
	_cachedReviseDraw = ok;
}

void KeWidget::OnPaste()
{
	_currentPage->OnPasteTriggered();
}

void KeWidget::OnUndo()
{
	_currentPage->OnUndoTriggered();
}

void KeWidget::OnRedo()
{
	_currentPage->OnRedoTriggered();
}

void KeWidget::OnInsertPixmapTriggered(QPixmap& pixmap)
{
	_currentPage->OnInsertPixmapTriggered(pixmap);
}

void KeWidget::OnInsertPixmapBackgroundTriggered(QPixmap& pixmap)
{
	_currentPage->OnInsertPixmapBackgroundTriggered(pixmap);
}

void KeWidget::OnRecordTriggered()
{
//	_currentPage->DisableEdit(false);
	if(_hades == NULL)
	{
		_hadesFile = 	FileUtil::GetTempFolder() + QUuid::createUuid().toString();
		_hades = new Hades();
		_hades->SignalRecordData.connect(this, &KeWidget::OnAudioVolume);
		_hades->RecordTimeUpdate.connect(this, &KeWidget::OnRecordTime);
		_hades->InitRecorder(_hadesFile.toUtf8().data(), -1, 0);
	}

	if(!_hades->HasMicrophone())
	{
		ChineseMessageBox::Show(tr("没有检测到麦克风，不能录课"), tr("错误"), ChineseMessageBox::OK, ChineseMessageBox::Error); 
		_keTime->Pause();
		return;
	}

	if(!_hasStarted)
	{
		_hades->StartRecording();
		_hasStarted = true;
	}
	else
	{
		_hades->ContinueRecording();
	}

	_dirty = true;
	QString title = FileUtil::GetFilename(_filepath, _filepath);
	Q_EMIT OnSaveFileNameChanged(title, true);
}

void KeWidget::OnPauseTriggered()
{
//	_currentPage->DisableEdit(true);
	if(_hades != NULL)
	{
		_hades->PauseRecording();
	}
}

void KeWidget::OnInsertBanTriggered(QString filepath)
{
//	_currentPage->InsertScAsImage(filepath);
	_currentPage->OnInsertBanTriggered(filepath);
}

void KeWidget::OnAttachBanTriggered(QString filepath)
{
	_attachedTests.push_back(filepath);
}

void KeWidget::OnAddSlidePresenterTriggered(SlidePresenter* presenter)
{
	presenter->ChangeParentWidget(_currentPage);
	presenter->move(_currentPage->GetCursorPoint());
	presenter->BeginShow();
}

void KeWidget::OnNewPageTriggered()
{
	//if(_currentPage != NULL)
	//	_mainLayout->removeWidget(_currentPage);
	_currentPage = new BanBaseWidget(false);
	_currentPage->_canEditInside = false;
	AddPage(_currentPage);
	OnCurrentBanFileNameChanged(_filepath, true);
}

void KeWidget::AddPage(BanBaseWidget* page, bool hasAction)
{
	connect(page, &BanBaseWidget::OnAnimationAction, this, &KeWidget::AddActionTriggered);
	connect(page, &BanBaseWidget::OnBitmapSizeChanged, this, &KeWidget::OnPageSizeChange);
	connect(page, &BanBaseWidget::OnSaveFileNameChanged, this, &KeWidget::OnCurrentBanFileNameChanged);
	connect(page, &BanBaseWidget::OnHighlightElementChanged, this, &KeWidget::OnHighlightElementChanged);
	page->SetMode(_cachedMode);
	page->LineWidthChanged(_cachedWidth);
	page->LineStyleChanged(_cachedStyle);
	page->LineColorChanged(_cachedColor);
	page->ReviseDrawTriggered(_cachedReviseDraw);

	_allPages.push_back(page);
	_currentPageIndex = _allPages.size() - 1;
	_mainLayout->addWidget(page);

	_mainLayout->setCurrentWidget(page);
	SlidePresenterManager* slideManager = SlidePresenterManager::Instance();
	slideManager->ChangeParent(page);

	KeTime* keTime = _keTime;
	if(keTime->IsPausing())
	{
//		_currentPage->DisableEdit(true);
	}
	if(hasAction)
		SendSwitchPageAnimation(page);

	NotifyWindow* notify = NotifyWindow::Instance();
	QString message = tr("第") + QString::number(_allPages.size()) + tr("块黑板");
	notify->ShowNotify(message);
}

void KeWidget::OnNewPageWithBackgroundTriggered(QImage& image)
{
	_currentPage = new BanBaseWidget(false);
	_currentPage->_canEditInside = false;
	_currentPage->_cachePixmap = QPixmap::fromImage(image);
	_currentPage->_backgroundImage.SetImage(_currentPage->_cachePixmap);

	AddPage(_currentPage);
}

void KeWidget::OnSwitchPageTriggered()
{
	KeTime* keTime = _keTime;
	bool needContinue = !keTime->IsPausing();
	keTime->Pause();
	KePages* pages = KePages::Instance();
	pages->ShowModal(_allPages);
	if(needContinue)
		keTime->Start();
}

void KeWidget::OnPreviousPageTriggered()
{
	if(_currentPageIndex <= 0)
		return;
	OnSelectPageTriggered(--_currentPageIndex);
}

void KeWidget::OnNextPageTriggered()
{
	if(_currentPageIndex >= _allPages.size() -1)
		return;
	OnSelectPageTriggered(++_currentPageIndex);
}

void KeWidget::OnSelectPageTriggered(int page)
{
	_currentPageIndex = page;
	_currentPage = _allPages[page];
	_currentPage->SetMode(_cachedMode);
	_currentPage->LineWidthChanged(_cachedWidth);
	_currentPage->LineStyleChanged(_cachedStyle);
	_currentPage->LineColorChanged(_cachedColor);
	_currentPage->ReviseDrawTriggered(_cachedReviseDraw);

	_mainLayout->setCurrentWidget(_currentPage);
	SlidePresenterManager* slideManager = SlidePresenterManager::Instance();
	slideManager->ChangeParent(_currentPage);

	SendSwitchPageAnimation(_currentPage);

	NotifyWindow* notify = NotifyWindow::Instance();
	QString message = tr("第") + QString::number(page + 1) + tr("块黑板");
	notify->ShowNotify(message);
}

void KeWidget::SendSwitchPageAnimation(BanBaseWidget* widget)
{
	SwitchPageAction action;
	action.SetTartgetBanUuid(widget->GetObjectUuid().toStdString());
	AddActionTriggered(action, NULL);
}

void KeWidget::AddActionTriggered(AnimationAction& action, Element* element)
{
	KeTime* keTime = _keTime;
	int time = keTime->GetAnimatedTime();
	action.SetTime(time);

	AnimationAction* a = action.CreateFromThis();
	_actions.push_back(a);
}

void KeWidget::OnCurrentBanFileNameChanged(QString filename, bool modifysign)
{
	//只关心第二个参数
	if(modifysign)
	{
		_dirty = true;
		QString title = FileUtil::GetFilename(_filepath, _filepath);
		Q_EMIT OnSaveFileNameChanged(title, true);
	}
}

void KeWidget::OnPageSizeChange(QSize size)
{
	_pageWidth = size.width();
	_pageHeight = size.height();
}