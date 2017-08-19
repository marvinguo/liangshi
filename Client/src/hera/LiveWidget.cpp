#include "LiveWidget.h"
#include "VideoWidget.h"
#include "../common/library.h"
#include "../common/FileUtil.h"
#include "../common/LokiClient.h"
#include "../common/Settings.h"

#include "KePages.h"

#include "DrawElement.h"
#include "LineElement.h"
#include "RectElement.h"
#include "EllipseElement.h"
#include "ImageElement.h"
#include "TextElement.h"
#include "HtmlTextElement.h"
#include "BanElement.h"

#include "NotifyWindow.h"
#include "ChineseMessageBox.h"
#include "SlidePresenterManager.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

LiveWidget::LiveWidget(QString session, QWidget *parent):
QWidget(parent),
_currentPage(NULL),
_cachedMode(DrawMode_Draw),
_cachedWidth(2),
_cachedStyle(Qt::SolidLine),
_cachedColor(QColor(Qt::white)),
_cachedReviseDraw(false),
_controlingBoard(false),
_session(session)
#ifndef _LiangshiDesktop
,_videoWidget(NULL)
#endif
{
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	_mainLayout = new QStackedLayout();
	_mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(_mainLayout);

	KePages* pages = KePages::Instance();
	connect(pages, &KePages::OnSelectPage, this, &LiveWidget::OnSelectPageTriggered);

	NewFirstPage();

	Settings settings;
	connect(&_lokiClient, &LokiClient::OnConnected, this, &LiveWidget::OnConnected);
	connect(&_lokiClient, &LokiClient::OnBoardLogin, this, &LiveWidget::OnBoardLoginTriggered);
	connect(&_lokiClient, &LokiClient::OnHadesData, this, &LiveWidget::OnHadesDataTriggered);
	connect(&_lokiClient, &LokiClient::OnBoardData, this, &LiveWidget::OnBoardDataTriggered);
	connect(&_lokiClient, &LokiClient::OnApplyBoard, this, &LiveWidget::OnStudentApplyBoardTriggered);
	connect(&_lokiClient, &LokiClient::OnTeacherAgreeBoard, this, &LiveWidget::OnTeacherAgreeBoardTriggered);
	connect(&_lokiClient, &LokiClient::OnTakeBoard, this, &LiveWidget::OnTeacherTakeBoardTriggered);
	connect(&_lokiClient, &LokiClient::OnSocketError, this, &LiveWidget::OnSocketErrorTriggered);

	_lokiClient.Init(settings.GetP2PUrl(), LOKI_LISTEN_PORT);

	NotifyWindow* notify = NotifyWindow::Instance();
	notify->ShowNotify(tr("正在连接服务器"));
}

LiveWidget::~LiveWidget()
{
	for(map<QString, Hades*>::iterator i = _n2nHades.begin(); i != _n2nHades.end(); ++i)
	{
		Hades* hades = i->second;
		hades->SignalCandidateSdp.disconnect_all();
		hades->SignalSdp.disconnect_all();
		hades->SignalLocalVideoImage.disconnect_all();
		hades->SignalRemoteVideoImage.disconnect_all();
		hades->Release();
		delete hades;
	}
	_n2nHades.clear();

#ifdef _LiangshiDesktop
	for(map<QString, VideoWidgets>::iterator i = _n2nVideoWidgets.begin(); i != _n2nVideoWidgets.end(); ++i)
	{
		VideoWidgets widgets = i->second;
		widgets.local->close();
		widgets.remote->close();
		widgets.local->deleteLater();
		widgets.remote->deleteLater();
	}
	_n2nVideoWidgets.clear();
	//这里有个bug,还是不能退出程序
	qApp->quit();
#else
	if(_videoWidget != NULL)
	{
		_videoWidget->deleteLater();
		_videoWidget = NULL;
	}
#endif
}

//from medial.dll
void LiveWidget::OnAudioVolume(int percent)
{
	//qDebug() << percent;
	Q_EMIT OnAudioVolumeChanged(percent);
}

bool LiveWidget::CanQuit()
{
	return true;
}

QString LiveWidget::GetSaveFileDialongExtension()
{
	return tr("课件文件(*.ke)");
}

void LiveWidget::UpdateFilepath(QString filepath)
{
}

bool LiveWidget::HasSavedPath()
{
	return false;
}

bool LiveWidget::SaveDocument(QString filepath)
{
	return false;
}

bool LiveWidget::SaveAsDocument(QString filepath)
{
	return false;
}

void LiveWidget::DrawModeTriggered()
{
	_currentPage->DrawModeTriggered();
	_cachedMode = DrawMode_Draw;
}

void LiveWidget::LineModeTriggered()
{
	_currentPage->LineModeTriggered();
	_cachedMode = DrawMode_Line;
}

void LiveWidget::RectModeTriggered()
{
	_currentPage->RectModeTriggered();
	_cachedMode = DrawMode_Rect;
}

void LiveWidget::PerectRectModeTriggered()
{
	_currentPage->PerectRectModeTriggered();
	_cachedMode = DrawMode_PerfectRect;
}

void LiveWidget::EclipseModeTriggered()
{
	_currentPage->EclipseModeTriggered();
	_cachedMode = DrawMode_Ellipse;
}

void LiveWidget::PerfectEclipseModeTriggered()
{
	_currentPage->PerfectEclipseModeTriggered();
	_cachedMode = DrawMode_PerfectEllipse;
}

void LiveWidget::SelectModeTriggered()
{
	_currentPage->SelectModeTriggered();
	_cachedMode = DrawMode_Select;
}

void LiveWidget::LineWidthTriggered(int width)
{
	_currentPage->LineWidthChanged(width);
	_cachedWidth = width;
}

void LiveWidget::LineStyleTriggered(int style)
{
	_currentPage->LineStyleChanged(style);
	_cachedStyle = style;
}

void LiveWidget::LineColorTriggered(QColor color)
{
	_currentPage->LineColorChanged(color);
	_cachedColor = color;
}

void LiveWidget::ReviseDrawTriggered(bool ok)
{
	_currentPage->ReviseDrawTriggered(ok);
	_cachedReviseDraw = ok;
}

void LiveWidget::OnPaste()
{
	_currentPage->OnPasteTriggered();
}

void LiveWidget::OnUndo()
{
	_currentPage->OnUndoTriggered();
}

void LiveWidget::OnRedo()
{
	_currentPage->OnRedoTriggered();
}

void LiveWidget::OnInsertPixmapTriggered(QPixmap& pixmap)
{
	if(_controlingBoard)
		_currentPage->OnInsertPixmapTriggered(pixmap);
}

void LiveWidget::OnInsertPixmapBackgroundTriggered(QPixmap& pixmap)
{
	if(_controlingBoard)
	{
		_currentPage->OnInsertPixmapBackgroundTriggered(pixmap);
		//这种情况，应该发switch board了
		SendSwitchPageAnimation(_currentPage);
	}
}

bool LiveWidget::OnGetLocalCameraTriggered()
{
#ifdef _LiangshiDesktop
	for(map<QString, VideoWidgets>::iterator i = _n2nVideoWidgets.begin(); i != _n2nVideoWidgets.end(); ++i)
	{
		VideoWidgets widgets = i->second;
		QPixmap pixmap = widgets.local->GetPixmap();
		OnInsertPixmapTriggered(pixmap);
		return true;
	}
#else
	//手机上的时候，只用做背景
	if(_videoWidget != NULL)
	{
		QPixmap pixmap = _videoWidget->GetPixmap();
		OnInsertPixmapBackgroundTriggered(pixmap);
		return true;
	}
#endif
	return false;
}

void LiveWidget::OnInsertBanTriggered(QString filepath)
{
//	_currentPage->InsertScAsImage(filepath);
	_currentPage->OnInsertBanTriggered(filepath);
}

void LiveWidget::OnAddSlidePresenterTriggered(SlidePresenter* presenter)
{
	presenter->ChangeParentWidget(_currentPage);
	presenter->move(_currentPage->GetCursorPoint());
	presenter->BeginShow();
}

void LiveWidget::SetEnableCurrentPage(bool enable)
{
	_currentPage->_disableHighlight = !enable;
	_currentPage->_disableDraw = !enable;
	_currentPage->_disablePaste = !enable;
	_currentPage->_disableDrop = !enable;
	_currentPage->DeHighlightElement();
	_controlingBoard = enable;
}

void LiveWidget::NewFirstPage()
{
	_currentPage = new BanBaseWidget(false);
	_currentPage->SetElementMatrix(_elementMatrix);
	_currentPage->SetWorldMatrix(_worldMatrix);
	_currentPage->_canEditInside = false;
	_currentPage->_emitAdditionAction = false;
	connect(_currentPage, &BanBaseWidget::OnAnimationAction, this, &LiveWidget::AddActionTriggered);
	connect(_currentPage, &BanBaseWidget::OnBitmapSizeChanged, this, &LiveWidget::OnPageSizeChange);
	connect(_currentPage, &BanBaseWidget::OnHighlightElementChanged, this, &LiveWidget::OnHighlightElementChanged);
	_currentPage->SetMode(_cachedMode);
	_currentPage->LineWidthChanged(_cachedWidth);
	_currentPage->LineStyleChanged(_cachedStyle);
	_currentPage->LineColorChanged(_cachedColor);
	_currentPage->ReviseDrawTriggered(_cachedReviseDraw);

	_allPages.push_back(_currentPage);
	_currentPageIndex = _allPages.size() - 1;
	_mainLayout->addWidget(_currentPage);
	_mainLayout->setCurrentWidget(_currentPage);
	SetEnableCurrentPage(false);
}

void LiveWidget::OnNewPageTriggered()
{
	_currentPage = new BanBaseWidget(false);
	_currentPage->SetElementMatrix(_elementMatrix);
	_currentPage->SetWorldMatrix(_worldMatrix);
	_currentPage->_canEditInside = false;
	_currentPage->_emitAdditionAction = false;
	connect(_currentPage, &BanBaseWidget::OnAnimationAction, this, &LiveWidget::AddActionTriggered);
	connect(_currentPage, &BanBaseWidget::OnBitmapSizeChanged, this, &LiveWidget::OnPageSizeChange);
	connect(_currentPage, &BanBaseWidget::OnHighlightElementChanged, this, &LiveWidget::OnHighlightElementChanged);
	_currentPage->SetMode(_cachedMode);
	_currentPage->LineWidthChanged(_cachedWidth);
	_currentPage->LineStyleChanged(_cachedStyle);
	_currentPage->LineColorChanged(_cachedColor);
	_currentPage->ReviseDrawTriggered(_cachedReviseDraw);

	_allPages.push_back(_currentPage);
	_currentPageIndex = _allPages.size() - 1;
	_mainLayout->addWidget(_currentPage);

	_mainLayout->setCurrentWidget(_currentPage);
	SlidePresenterManager* slideManager = SlidePresenterManager::Instance();
	slideManager->ChangeParent(_currentPage);

	SendSwitchPageAnimation(_currentPage);
	NotifyWindow* notify = NotifyWindow::Instance();
	QString message = tr("第") + QString::number(_allPages.size()) + tr("块黑板");
	notify->ShowNotify(message);
}

void LiveWidget::OnPreviousPageTriggered()
{
	if(_currentPageIndex <= 0)
		return;
	OnSelectPageTriggered(--_currentPageIndex);
}

void LiveWidget::OnNextPageTriggered()
{
	if(_currentPageIndex >= _allPages.size() - 1)
		return;
	OnSelectPageTriggered(++_currentPageIndex);
}

void LiveWidget::OnNewPageWithBackgroundTriggered(QImage& image)
{
	_currentPage = new BanBaseWidget(false);
	_currentPage->SetElementMatrix(_elementMatrix);
	_currentPage->SetWorldMatrix(_worldMatrix);
	_currentPage->_canEditInside = false;
	_currentPage->_emitAdditionAction = false;
	_currentPage->_cachePixmap = QPixmap::fromImage(image);
	_currentPage->_backgroundImage.SetImage(_currentPage->_cachePixmap);

	connect(_currentPage, &BanBaseWidget::OnAnimationAction, this, &LiveWidget::AddActionTriggered);
	_currentPage->SetMode(_cachedMode);
	_currentPage->LineWidthChanged(_cachedWidth);
	_currentPage->LineStyleChanged(_cachedStyle);
	_currentPage->LineColorChanged(_cachedColor);
	_currentPage->ReviseDrawTriggered(_cachedReviseDraw);

	_allPages.push_back(_currentPage);
	_currentPageIndex = _allPages.size() - 1;
	_mainLayout->addWidget(_currentPage);

	_mainLayout->setCurrentWidget(_currentPage);
	SlidePresenterManager* slideManager = SlidePresenterManager::Instance();
	slideManager->ChangeParent(_currentPage);
	
	//觉得导入的时候，不需要发送，只能通过切换黑板
	//SendSwitchPageAnimation(_currentPage);
}

void LiveWidget::OnSwitchPageTriggered()
{
	KePages* pages = KePages::Instance();
	pages->ShowModal(_allPages);
}

void LiveWidget::OnApplyBoardTriggered()
{
	_lokiClient.ApplyBoard();
}

void LiveWidget::OnTakeBoardTriggered()
{
	_lokiClient.TakeBoard();

	Q_EMIT OnLiveCanApplyAndTake(true, false, false);
	SetEnableCurrentPage(true);
}

void LiveWidget::OnSelectPageTriggered(int page)
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

void LiveWidget::SendSwitchPageAnimation(BanBaseWidget* widget)
{
	SwitchPageAction action;
	action.SetTartgetBanUuid(widget->GetObjectUuid().toStdString());
	AddActionTriggered(action, NULL);
}

void LiveWidget::OnConnected()
{
	QString session = _session;
#ifdef _LiangshiDesktop
	QStringList arguments = QCoreApplication::arguments();
	if(arguments.size() < 3)
	{
		qApp->quit();
		return;
	}
	session = arguments[1];
#endif

	_lokiClient.BoardLogin(session);
}

//定义hades结构
struct HadesData
{
	//1, sdp, 2 candidate
	int sdptype;
	string type;
	string sdp_mid;
	int stp_mline;
	string sdp;
	MSGPACK_DEFINE(sdptype, type, sdp_mid, stp_mline, sdp);

	LokiBuffer GetMsgPackData();
	static HadesData* FromLokiBuffer(LokiBuffer& buffer);
};

LokiBuffer HadesData::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

HadesData* HadesData::FromLokiBuffer(LokiBuffer& buffer)
{
	msgpack::unpacked msg;
	msgpack::unpack(&msg, buffer.Data(), buffer.Size());
	HadesData* hades = new HadesData();
	msg.get().convert(hades);
	return hades;
}

void LiveWidget::OnSdp(const char* identifier, const char* type, const char* sdp)
{
	HadesData data;
	data.sdptype = 1;
	data.type = type;
	data.sdp = sdp;
	LokiBuffer buffer = data.GetMsgPackData();
	_lokiClient.HadesData(QString(identifier), buffer);
}

void LiveWidget::OnCandidateSdp(const char* identifier, const char* sdp_mid, int stp_mline, const char* sdp)
{
	HadesData data;
	data.sdptype = 2;
	data.sdp_mid = sdp_mid;
	data.stp_mline = stp_mline;
	data.sdp = sdp;
	LokiBuffer buffer = data.GetMsgPackData();
	_lokiClient.HadesData(QString(identifier), buffer);
}

void LiveWidget::OnHadesDataTriggered(QString peer_id, QString nickname, LokiBuffer& data)
{
	HadesData* hadesData = HadesData::FromLokiBuffer(data);

	NotifyWindow* notify = NotifyWindow::Instance();
	notify->ShowNotify(QString(tr("收到%1语音请求，正在连接")).arg(nickname));

	Hades* hades = GetHades(hadesData->sdptype, peer_id, nickname, _lokiClient._has_video, false);

	if(hadesData->sdptype == 1)
	{
		hades->OnRemoteSdp(const_cast<char*>(hadesData->type.c_str()), const_cast<char*>(hadesData->sdp.c_str()));
	}
	if(hadesData->sdptype == 2)
	{
		hades->OnRemoteCandidate(const_cast<char*>(hadesData->sdp_mid.c_str()), hadesData->stp_mline, const_cast<char*>(hadesData->sdp.c_str()));
	}
	
	delete hadesData;
}

#ifndef _LiangshiDesktop
void LiveWidget::OnVideoWidget(bool show)
{
	if(_videoWidget != NULL)
	{
		if(show)
			_videoWidget->show();
		else
			_videoWidget->hide();
	}
}
#endif


//对于收到对方的，是不是可以不析构，重用，这里有问题，我想
//这里一定要加掉线逻辑？还有要不要告诉初始化完成的事件？
Hades* LiveWidget::GetHades(int sdptype, QString peer_id, QString nickname, bool has_video, bool caller)
{
	Hades* hades = NULL;
	map<QString, Hades*>::iterator find = _n2nHades.find(peer_id);
	if(find != _n2nHades.end())
	{
		//我现在想的是，发起，必须删除
		//被动的，有stream的，必须删除
		/*
		if(sdptype == 1 && find->second->HasStream() || sdptype == 0)
		{
			find->second->Release();
			delete find->second;
			_n2nHades.erase(find);
		}
		else
		{
			hades = find->second;
		}
		*/

		//2014..05.20
		//想了一个晚上，发现挺简单的，只需要删除发起的，被发起的，是新上线， 不可能有hades
		//即使有，我想也可以重用，hasstream函数现在没用
		if(caller)
		{
			find->second->SignalCandidateSdp.disconnect_all();
			find->second->SignalSdp.disconnect_all();
			find->second->SignalLocalVideoImage.disconnect_all();
			find->second->SignalRemoteVideoImage.disconnect_all();
			find->second->Release();
			delete find->second;
			_n2nHades.erase(find);

#ifdef _LiangshiDesktop
			//找到对应的widget，然后删除
			map<QString, VideoWidgets>::iterator it = _n2nVideoWidgets.find(peer_id);
			if(it != _n2nVideoWidgets.end())
			{
				VideoWidgets widgets = it->second;
				widgets.local->close();
				widgets.local->close();
				widgets.local->deleteLater();
				widgets.remote->deleteLater();
				_n2nVideoWidgets.erase(it);
			}
#else
			//这个地方需要估什么吗？
			if(_videoWidget != NULL)
			{
				_videoWidget->show();
			}
#endif
		}
		else
		{
			hades = find->second;
		}
	}

	if(hades == NULL)
	{
		hades = new Hades();
		hades->SignalCandidateSdp.connect(this, &LiveWidget::OnCandidateSdp);
		hades->SignalSdp.connect(this, &LiveWidget::OnSdp);
		_n2nHades.insert(pair<QString, Hades*>(peer_id, hades));

		if(has_video)
		{
#ifdef _LiangshiDesktop
			VideoWidget* local = new VideoWidget(tr("本地视频"));
			VideoWidget* remote = new VideoWidget(nickname + tr("的视频"));
			hades->SignalLocalVideoImage.connect(local, &VideoWidget::OnVideoImageUpdate);
			hades->SignalRemoteVideoImage.connect(remote, &VideoWidget::OnVideoImageUpdate);
			local->show();
			remote->show();

			VideoWidgets widgets;
			widgets.local = local;
			widgets.remote = remote;
			_n2nVideoWidgets.insert(pair<QString, VideoWidgets>(peer_id, widgets));
#else
			if(_videoWidget == NULL)
				_videoWidget = new MobileVideoWidget();
			hades->SignalLocalVideoImage.connect(_videoWidget, &MobileVideoWidget::OnLocalVideoImageUpdate);
			hades->SignalRemoteVideoImage.connect(_videoWidget, &MobileVideoWidget::OnRemotelVideoImageUpdate);
			_videoWidget->show();
#endif
		}
		string identifier = peer_id.toStdString();

		qDebug() << "being init hades";

		StunServer servers[2];

		StunServer stun1;
		stun1.uri = "stun:stun.l.google.com:19302";
		stun1.username = "";
		stun1.password = "";

		StunServer stun2;
		stun2.uri = "turn:p2p.liangshiyy.com:33478?transport=udp";
		stun2.username = "marvin";
		stun2.password = "JuXubwdbby1qiEzrYkyY";

		servers[0] = stun1;
		servers[1] = stun2;
		//caonima, google stun会挂掉，真tm到现在才发现这个问题

		hades->InitP2P(servers, 2, const_cast<char*>(identifier.c_str()), caller, has_video, has_video);
		qDebug() << "end init hades";
	}

	return hades;
}

void LiveWidget::OnBoardLoginTriggered(Loki::BoardLoginCommand command)
{
	if(command.result)
	{
		//自已登录
		if(!command.peer)
		{
			if(command.all_teachers)
			{
				Q_EMIT OnLiveCanApplyAndTake(false, false, false);
				SetEnableCurrentPage(false);
				NotifyWindow* notify = NotifyWindow::Instance();
				notify->ShowNotify(tr("已经连接到服务器,当前是直播演示课堂，等待其它人上线"));
			}
			else
			{
				if(command.is_teacher)
				{
					Q_EMIT OnLiveCanApplyAndTake(true, false, false);
					SetEnableCurrentPage(true);
					NotifyWindow* notify = NotifyWindow::Instance();
					notify->ShowNotify(tr("已经连接到服务器,等待学生上线"));
				}
				else
				{
					Q_EMIT OnLiveCanApplyAndTake(false, true, true);
					NotifyWindow* notify = NotifyWindow::Instance();
					notify->ShowNotify(tr("已经连接到服务器"));
				}
			}
		}
		else
		{
			//user_id 只用对peer有用
			//hades数据来的时候，表示我是申请方，apply可以亮了
			//放在这里不好，debug不测试语音

			//peer上线，开始发起hades
			GetHades(0, QString(command.user_id.c_str()), QString(command.nickname.c_str()), command.has_video);

			NotifyWindow* notify = NotifyWindow::Instance();
			notify->ShowNotify(QString(tr("%1已经上线,正在连接语音")).arg(QString(command.nickname.c_str())));

			//只有是自己是老师，向别人发送
			if(_lokiClient._is_teacher && !_lokiClient._all_teachers)
				SendSwitchPageAnimation(_currentPage);
		}
	}
	else
	{
		NotifyWindow* notify = NotifyWindow::Instance();
		notify->ShowNotify(tr("该课程不存在，登录失败"));
		Q_EMIT OnLiveCanApplyAndTake(false, true, false);
	}
}

struct BoardData
{
	int type;
	LokiBuffer data;
	MSGPACK_DEFINE(type, data);

	LokiBuffer GetMsgPackData();
	static BoardData* FromLokiBuffer(LokiBuffer& buffer);
};

LokiBuffer BoardData::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

BoardData* BoardData::FromLokiBuffer(LokiBuffer& buffer)
{
	msgpack::unpacked msg;
	msgpack::unpack(&msg, buffer.Data(), buffer.Size());
	BoardData* hades = new BoardData();
	msg.get().convert(hades);
	return hades;
}

//还点用action,因为有切换黑板等功能，现在不只是有element,uuid可能也有用，因为两边的数据有可能重复
//2014.9.26,现在想想1,对于电脑和电脑，不需要调整坐标，对于mobile,发出的element需要调整成desktop的坐标
//2. 手机接受以后，也需要调整成自己？？
void LiveWidget::AddActionTriggered(AnimationAction& action, Element* element)
{
	if(!_controlingBoard)
		return;
	//move 消息发的太多，检测到move, 就不发送
/*
	Qt::MouseButtons buttons = QApplication::mouseButtons();
	if(buttons == Qt::LeftButton)
		return;
*/
	BoardData data;
	if(action.GetType() == AnimationType_Show)
	{
		//改成NetworkShow action
		if(element == NULL)
			return;
		NetworkShowAction actionDummy;
		LokiBuffer buffer = element->GetMsgPackData();
		actionDummy.Update(element->GetType(), buffer);
		data.type = actionDummy.GetType();
		data.data = actionDummy.GetMsgPackData();
	}
	else if(action.GetType() == AnimationType_SwitchPage)
	{
		NetworkSwitchPageAction actionDummy;
		LokiBuffer buffer = _currentPage->GetMsgPackData();
		actionDummy.Update(_currentPage->GetObjectUuid(), buffer);
		data.type = actionDummy.GetType();
		data.data = actionDummy.GetMsgPackData();
	}
	else
	{
		data.type = action.GetType();
		data.data = action.GetMsgPackData();
	}

	LokiBuffer buffer = data.GetMsgPackData();
	_lokiClient.BoardData(buffer);
}

void LiveWidget::OnBoardDataTriggered(LokiBuffer& data)
{
	BoardData* board = BoardData::FromLokiBuffer(data);

	if(board != NULL)
	{
		AnimationAction* action = AnimationAction::CreateFromType((AnimationType)(board->type), board->data);
		PlayAction(action);
		delete board;
		delete action;
	}
}

void LiveWidget::OnTeacherAgreeBoardTriggered(bool another, bool agree)
{
	if(another)
	{
		if(agree)
		{
			Q_EMIT OnLiveCanApplyAndTake(false, true, true);
			SetEnableCurrentPage(false);
			NotifyWindow* notify = NotifyWindow::Instance();
			notify->ShowNotify(tr("别的同学申请了黑板"));
			return;
		}
	}
	else
	{
		if(agree)
		{
			Q_EMIT OnLiveCanApplyAndTake(true, true, false);
			SetEnableCurrentPage(true);
		}
		else
		{
			Q_EMIT OnLiveCanApplyAndTake(false, true, true);
			SetEnableCurrentPage(false);
			NotifyWindow* notify = NotifyWindow::Instance();
			notify->ShowNotify(tr("老师拒绝你的黑板申请"));
		}

	}
}

void LiveWidget::OnStudentApplyBoardTriggered(QString student_id, QString nickname)
{
	int rb = ChineseMessageBox::Show(QString(tr("%1同学请求使用黑板?")).arg(nickname), tr("消息"), ChineseMessageBox::YesNo, ChineseMessageBox::Question);
	_lokiClient.AgreeBoard(student_id, rb == QMessageBox::Yes);

	if(rb == QMessageBox::Yes)
	{
		Q_EMIT OnLiveCanApplyAndTake(false, false, false);
		SetEnableCurrentPage(false);
	}
}

void LiveWidget::OnTeacherTakeBoardTriggered()
{
	if(_controlingBoard)
	{
		NotifyWindow* notify = NotifyWindow::Instance();
		
		if(_lokiClient._all_teachers)
		{
			SetEnableCurrentPage(false);
			Q_EMIT OnLiveCanApplyAndTake(false, true, true);
			notify->ShowNotify(tr("别的同学使用黑板"));
		}
		else
		{
			SetEnableCurrentPage(false);
			Q_EMIT OnLiveCanApplyAndTake(false, _lokiClient._is_teacher, true);
			notify->ShowNotify(tr("老师收回黑板控制权"));
		}
	}
}

//这个Play还是比PlayerWidget里的简单
void LiveWidget::PlayAction(AnimationAction* action)
{
	switch(action->GetType())
	{
	case AnimationType_NetworkShow:
		PlayNetworkShowAction((NetworkShowAction*)action);
		break;
	case AnimationType_NetworkSwitchPage:
		PlayNetworkSwitchPageAction((NetworkSwitchPageAction*)action);
		break;
	case AnimationType_Size:
		PlaySizeAction((SizeAction*)action);
		break;
	case AnimationType_Hide:
		PlayHideAction((HideAction*)action);
		break;
	case AnimationType_LineWidth:
		PlayLineWidthAction((LineWidthAction*)action);
		break;
	case AnimationType_LineStyle:
		PlayLineStyleAction((LineStyleAction*)action);
		break;
	case AnimationType_LineColor:
		PlayLineColorAction((LineColorAction*)action);
		break;
	case AnimationType_TopMost:
		PlayTopMostAction((TopMostAction*)action);
		break;
	case AnimationType_BottomMost:
		PlayBottomMostAction((BottomMostAction*)action);
		break;
	case AnimationType_TextChanged:
		PlayTextChangedAction((TextChangedAction*)action);
		break;
	}
}

void LiveWidget::PlayNetworkShowAction(NetworkShowAction* action)
{
	//这个地方不能用鼠标的位置来算，有bug
	LokiBuffer buffer = action->GetElementBuffer();
	Element* element = Element::CreateFromType((ElementType)(action->GetElementType()), buffer);
	if(element != NULL)
	{
		element->SetCanDelete(false);
		//_currentPage->AddElement(element, false, false);
		//_currentPage->DeHighlightElement();
		//这个地方应该和从文件载入一个逻辑，和界面上的有问题
		_currentPage->_allElements.insert(element);
		//_currentPage->_initElements.insert(element->DuplicateElement());
		_currentPage->_drawingElements.push_back(element);
	}
	_currentPage->Repaint();
}

void LiveWidget::PlayNetworkSwitchPageAction(NetworkSwitchPageAction* action)
{
	//找到Uuid对应的page, 如果没有就新建，uuid和新建的一样. setmessagepack会更新uuid
	QString uuid = action->GetUuid();
	BanBaseWidget* switchPage = NULL;
	for(vector<BanBaseWidget*>::iterator i = _allPages.begin(); i != _allPages.end(); ++i)
	{
		BanBaseWidget* page = *i;
		if(page->GetObjectUuid() == uuid)
		{
			switchPage = page;
			break;
		}
	}

	if(switchPage == NULL)
	{
		_currentPage = new BanBaseWidget(false);
		_currentPage->SetElementMatrix(_elementMatrix);
		_currentPage->SetWorldMatrix(_worldMatrix);
		_currentPage->_canEditInside = false;
		_currentPage->_emitAdditionAction = false;
		connect(_currentPage, &BanBaseWidget::OnAnimationAction, this, &LiveWidget::AddActionTriggered);
		_currentPage->SetMode(_cachedMode);
		_currentPage->LineWidthChanged(_cachedWidth);
		_currentPage->LineStyleChanged(_cachedStyle);
		_currentPage->LineColorChanged(_cachedColor);
		_currentPage->ReviseDrawTriggered(_cachedReviseDraw);

		_allPages.push_back(_currentPage);
		_currentPageIndex = _allPages.size() - 1;
		_mainLayout->addWidget(_currentPage);

		_mainLayout->setCurrentWidget(_currentPage);
//		SlidePresenterManager* slideManager = SlidePresenterManager::Instance();
//		slideManager->ChangeParent(_currentPage);


		switchPage = _currentPage;
	}
	else
	{
		_currentPage = switchPage;
		_currentPage->SetMode(_cachedMode);
		_currentPage->LineWidthChanged(_cachedWidth);
		_currentPage->LineColorChanged(_cachedColor);
		_mainLayout->setCurrentWidget(_currentPage);
	}

	LokiBuffer buffer = action->GetPageBuffer();
	switchPage->SetMsgPackData(buffer);
	SetEnableCurrentPage(false);
	_currentPage->Repaint();
}

void LiveWidget::PlaySizeAction(SizeAction* action)
{
	Element* element = _currentPage->FindElementByUuid(action->GetTargetElementUuid());
	if(element != NULL)
	{
		QPoint point(action->GetLeftTop()._x, action->GetLeftTop()._y);
		element->UpdateSize(point, action->GetWidth(), action->GetHeight());
	}
	_currentPage->Repaint();
}

void LiveWidget::PlayHideAction(HideAction* action)
{
	Element* element = _currentPage->FindElementByUuid(action->GetTargetElementUuid());
	if(element != NULL)
	{
		_currentPage->_drawingElements.remove(element);
	}
	_currentPage->Repaint();
}

void LiveWidget::PlayLineWidthAction(LineWidthAction* action)
{
	Element* element = _currentPage->FindElementByUuid(action->GetTargetElementUuid());
	if(element != NULL)
	{
		element->SetLineWidth(action->GetWidth());
	}
	_currentPage->Repaint();
}

void LiveWidget::PlayLineStyleAction(LineStyleAction* action)
{
	Element* element = _currentPage->FindElementByUuid(action->GetTargetElementUuid());
	if(element != NULL)
	{
		element->SetLineStyle(action->GetStyle());
	}
	_currentPage->Repaint();
}

void LiveWidget::PlayLineColorAction(LineColorAction* action)
{
	Element* element = _currentPage->FindElementByUuid(action->GetTargetElementUuid());
	if(element != NULL)
	{
		element->SetLineColor(action->GetColor());
	}
	_currentPage->Repaint();
}

void LiveWidget::PlayTopMostAction(TopMostAction* action)
{
	for(list<Element*>::iterator i = _currentPage->_drawingElements.begin(); i != _currentPage->_drawingElements.end(); ++i)
	{
		Element* e = *i;
		if(e->GetObjectUuid() == action->GetTargetElementUuid())
		{
			_currentPage->_drawingElements.erase(i);
			_currentPage->_drawingElements.push_back(e);
			break;
		}
	}
	_currentPage->Repaint();
}

void LiveWidget::PlayBottomMostAction(BottomMostAction* action)
{
	for(list<Element*>::iterator i = _currentPage->_drawingElements.begin(); i != _currentPage->_drawingElements.end(); ++i)
	{
		Element* e = *i;
		if(e->GetObjectUuid() == action->GetTargetElementUuid())
		{
			_currentPage->_drawingElements.erase(i);
			_currentPage->_drawingElements.push_front(e);
			break;
		}
	}
	_currentPage->Repaint();
}

void LiveWidget::PlayTextChangedAction(TextChangedAction* action)
{
	Element* element = _currentPage->FindElementByUuid(action->GetTargetElementUuid());
	if(element != NULL)
	{
		TextElement* text = dynamic_cast<TextElement*>(element);
		if(text != NULL)
		{
			text->_text = action->GetText();
			text->_fontSize = action->GetFontSize();
		}

		HtmlTextElement* html = dynamic_cast<HtmlTextElement*>(element);
		if(html != NULL)
		{
			html->_text = action->GetText();
			html->_fontSize = action->GetFontSize();
		}
	}
	_currentPage->Repaint();
}

//想想，还是在Widget里处理吧，除非以后能把错误抽象一下
void LiveWidget::OnSocketErrorTriggered(QAbstractSocket::SocketError socketError)
{
	NotifyWindow* notify = NotifyWindow::Instance();

	//网络上所有的异常，都应该在这里捕获，完善体验
	switch (socketError)
	{
	case QAbstractSocket::RemoteHostClosedError:
		{
			notify->ShowNotify(tr("与服务器断开"));
			ShowNetworkRetry();
			break;
		}
	case QAbstractSocket::HostNotFoundError:
		{
			notify->ShowNotify(tr("找不到服务器，请确定连接到网络"));
			ShowNetworkRetry();
			break;
		}
	case QAbstractSocket::ConnectionRefusedError:
		{
			notify->ShowNotify(tr("连接服务器失败，请联系客服"));
			ShowNetworkRetry();
			break;
		}
	case QAbstractSocket::SocketTimeoutError:
		{
			notify->ShowNotify(tr("连接服务器超时，请确定连接到网络"));
			ShowNetworkRetry();
			break;
		}
	default:
		{
			notify->ShowNotify(tr("网络错误，请检查网络"));
			ShowNetworkRetry();
			break;
		}
	}
}

void LiveWidget::ShowNetworkRetry()
{
	int rb = ChineseMessageBox::Show(tr("重新连接?"), tr("与服务器断开"), ChineseMessageBox::RetryCancel, ChineseMessageBox::Question);
	if(rb == QMessageBox::Retry)
	{
		Settings settings;
		_lokiClient.Init(settings.GetP2PUrl(), LOKI_LISTEN_PORT);
	}
}

void LiveWidget::resizeEvent(QResizeEvent *e)
{
	//这个是在手机里的，非常重要的东西
	QWidget::resizeEvent(e);

	QSize size = e->size();
}

//怎么算也有点拉伸，1280*760, 然后顶部27, 1278*732，笔记本标准的，其实，大屏幕和小的也是有40个像素的差距。

#define STANDRAD_WIDTH 1278
#define STANDRAD_HEIGHT 732

void LiveWidget::OnPageSizeChange(QSize size)
{
	qDebug() << "LiveWidget::OnPageSizeChange";
	if(size.width() == 0 || size.height() == 0)
	{
		qDebug() << "00000";
		return;
	}

	double pageWidth = size.width();
	double pageHeight = size.height();

	double mobileToDesktopX = pageWidth / STANDRAD_WIDTH;
	double mobileToDesktopY = pageHeight / STANDRAD_HEIGHT;

	double desktopToMobileX = (double)STANDRAD_WIDTH / pageWidth;
	double desktopToMobileY = (double)STANDRAD_HEIGHT / pageHeight;

//这里有个问题，如果这一个element能被2边修改，那么这个问题，需要修正element的matrix,这个设计就麻烦了
	_elementMatrix.reset();
	_elementMatrix.scale(desktopToMobileX, desktopToMobileY);
	_worldMatrix.reset();
	_worldMatrix.scale(mobileToDesktopX, mobileToDesktopY);

	//设置所有以有所有页面的world matrix

	for(vector<BanBaseWidget*>::iterator i = _allPages.begin(); i != _allPages.end(); ++i)
	{
		BanBaseWidget* page = *i;
		page->SetElementMatrix(_elementMatrix);
		page->SetWorldMatrix(_worldMatrix);
	}
}