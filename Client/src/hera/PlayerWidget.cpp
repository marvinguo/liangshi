#include "PlayerWidget.h"
#include "AnimationThread.h"
#include "../common/library.h"
#include "../common/FileUtil.h"
#include "DrawElement.h"
#include "LineElement.h"
#include "RectElement.h"
#include "EllipseElement.h"
#include "KeTime.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

PlayerWidget::PlayerWidget(QWidget *parent):
QWidget(parent),
_keTime(NULL),
_hades(NULL),
_currentPage(NULL),
_isMouseDown(false),
_moveDirection(0)
{
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	_mainLayout = new QStackedLayout();
	_mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(_mainLayout);

	//_thread = new AnimationThread();
	//connect(_thread, &AnimationThread::OnAnimated, this, &PlayerWidget::AnimatedTriggered);

	setAttribute(Qt::WA_ContentsPropagated);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_NoSystemBackground);

	_timer = new QTimer(this);
	connect(_timer, &QTimer::timeout, this, &PlayerWidget::AnimatedTriggered);
	_timer->stop();
}

PlayerWidget::~PlayerWidget()
{
	ReleaseMedia();
	_timer->stop();
	//这里有个crash,不知怎么解决
/*
	_thread->Stop(true);
	QThread::msleep(200);
	_thread->deleteLater();
*/
	for(vector<AnimationAction*>::iterator i = _actions.begin(); i != _actions.end(); ++i)
	{
		delete *i;
	}
	_actions.clear();

	for(map<string, PlayerPage*>::iterator i = _allPages.begin(); i != _allPages.end(); ++i)
	{
		PlayerPage* page = i->second;
		//page->deleteLater();
		delete page;
	}
	_allPages.clear();
}

LokiBuffer PlayerWidget::GetMsgPackData()
{
	//返回当前page的MsgPack
	return _currentPage->_viewer->GetMsgPackData(false);
}

void PlayerWidget::DrawBackground(QPixmap& pixmap)
{
	return _currentPage->DrawBackground(pixmap);
}

QPixmap PlayerWidget::GetBackground()
{
	return _currentPage->_backgroudPixmap;
}

void PlayerWidget::SetMsgPackData(LokiBuffer& buffer)
{
	msgpack::unpacked msg;
	msgpack::unpack(&msg, buffer.Data(), buffer.Size());
	msg.get().convert(this);
}

void PlayerWidget::UpdateDataFromMsgPack()
{
	_pageWidth = _pageWidth > 0 ? _pageWidth : 1;
	_pageHeight = _pageHeight > 0 ? _pageHeight : 1;
}

void PlayerWidget::UpdateFilepath(QString filepath)
{
	QString title = FileUtil::GetFilename(filepath, filepath);
	Q_EMIT OnSaveFileNameChanged(title, false);
}

bool PlayerWidget::SaveDocument(QString filepath)
{
	return true;
}

void PlayerWidget::AddPage(BanBaseWidget* page)
{
	_allPages.insert(pair<string, PlayerPage*>(page->GetObjectUuid().toStdString(), new PlayerPage(_pageWidth, _pageHeight, page)));
}

void PlayerWidget::AddAction(AnimationAction* action)
{
	_actions.push_back(action);
}

void PlayerWidget::ReleaseMedia()
{
	if(_hades != NULL)
	{
		_hades->StopPlaying();
		_hades->Release();
		delete _hades;
		_hades = NULL;
		QFile::remove(_hadesFile);
		_hadesFile = "";
	}
}

void PlayerWidget::InitMedia(QString filepath)
{
	ReleaseMedia();
	_hadesFile = filepath;
	_hades = new Hades();
	//wstring filename = _hadesFile.toStdWString();

	//wchar_t* wPtr = const_cast<wchar_t*>(filename.c_str());

	//	_hades->InitPlayer(reinterpret_cast<char*>(wPtr));
	_hades->InitPlayer(_hadesFile.toUtf8().data());


	//还是以媒体文件中的为主
	//2014.11.03,这里开始应用msgpack time, 因为现在ke时间同步了
	//没有麦克风，也没有时间，这个取不出来
	_totalTime = _hades->GetPlayFileDuration();
}

void PlayerWidget::OnPlayTriggered()
{
	KeTime* keTime = _keTime;
	_timer->start(100);
	if(_hades != NULL)
	{
		_hades->StopPlaying();
		int time = keTime->GetAnimatedTime();
		//说的声音比快一点，不知为啥
		//2014.04.30,发现现在这个版本好像没有这个问题了
		//time = time - 1000;
		if (time < 0)
			time = 0;
		_hades->GotoTime(time);
		_hades->StartPlaying();
	}
}

void PlayerWidget::OnPauseTriggered()
{
	_timer->stop();
	if(_hades != NULL)
	{
		_hades->PausePlaying();
	}
}

//倒的这个，需要reset,必须
void PlayerWidget::OnGotoPositionTriggered(double value)
{
	int time = value * _totalTime;
	OnGotoPosition(time);
}

void PlayerWidget::OnGotoPosition(int time)
{
	KeTime* keTime = _keTime;
	{
		//QMutexLocker lock(&_animateMutex);

		//这个reset有没有可能优化的地方?
		//2014.0515,我想到一个办法，play switch board的时候在reset element
		ResetPages();
		keTime->SetAnimatedTime(time);
	}
	if(!keTime->IsPausing())
	{
		//这样做主要是让声音同步
		OnPlayTriggered();
	}
	else
	{
		//让画面同步
		AnimatedTriggered();
	}
}

void PlayerWidget::OnResetTriggered()
{
	ResetPages();
}

void PlayerWidget::ResetPages()
{
	for(map<string, PlayerPage*>::iterator i = _allPages.begin(); i != _allPages.end(); ++i)
	{
		(i->second)->ResetElement();
	}

	for(vector<AnimationAction*>::iterator i = _actions.begin(); i != _actions.end(); ++i)
	{
		(*i)->SetPlayed(false);
	}
}

void PlayerWidget::AnimatedTriggered()
{
	KeTime* keTime = _keTime;
	QMutexLocker lock(&_animateMutex);

	//2014.0515,优化
	//1,得到最后的switch board action
	//2,只play那个board的action

	//还有一种方法，从声音里得到时间
//	vector<AnimationAction*> actions = GetUnAnimatedActions(keTime->GetAnimatedTime());
	vector<AnimationAction*> actions = GetLatestSwitchBoardActions(keTime->GetAnimatedTime());

	if(!actions.empty())
	{
		PlayActions(actions);
		//然后再一起repaint
		_currentPage->update();
	}
}

//应用GetUnAnimatedActions,所有的page都是好的
void PlayerWidget::GotoTime(int time)
{
	ResetPages();
	_keTime->SetAnimatedTime(time);
	vector<AnimationAction*> actions = GetUnAnimatedActions(_keTime->GetAnimatedTime());
	PlayActions(actions);
}

vector<AnimationAction*> PlayerWidget::GetLatestSwitchBoardActions(int time)
{
	vector<AnimationAction*> actions;
	AnimationAction* switchBoardaction = NULL;

	//现在我只能弄成2次循环，time是排序的，可以用二分法来查找，优化的地方挺多的
	//2014.10.31,多个switch action可能是同时的，这样想想，下面的逻辑也是对的
	for(auto i = _actions.begin(); i != _actions.end(); ++i)
	{
		AnimationAction* action = *i;
		if(action->GetTime() < time)
		{
			if(action->GetType() == AnimationType_SwitchPage)
			{
				switchBoardaction = action;
				continue;
			}
		}
		else
		{
			break;
		}
	}

	if(switchBoardaction != NULL)
	{
		if(!switchBoardaction->HasPlayed())
		{
			switchBoardaction->SetPlayed(true);
			actions.push_back(switchBoardaction);
		}

		string uuid = switchBoardaction->GetTargetScUuid();

		for(auto i = _actions.begin(); i != _actions.end(); ++i)
		{
			AnimationAction* action = *i;
			if(action->GetTime() < time)
			{
				if(action->HasPlayed())
					continue;

				if(action->GetType() != AnimationType_SwitchPage && action->GetTargetScUuid() == uuid)
				{
					actions.push_back(action);
					action->SetPlayed(true);
				}
			}
			else
			{
				break;
			}
		}
	}
	return actions;
}

vector<AnimationAction*> PlayerWidget::GetUnAnimatedActions(int time)
{
	vector<AnimationAction*> actions;

	for(vector<AnimationAction*>::iterator i = _actions.begin(); i != _actions.end(); ++i)
	{
		AnimationAction* action = *i;
		if(action->GetTime() < time)
		{
			if(!action->HasPlayed())
			{
				actions.push_back(action);
				action->SetPlayed(true);
			}
		}
		else
		{
			break;
		}
	}
	return actions;
}

void PlayerWidget::PlayActions(vector<AnimationAction*> actions)
{
	for(vector<AnimationAction*>::iterator i = actions.begin(); i != actions.end(); ++i)
	{
		AnimationAction* action = *i;

		switch(action->GetType())
		{
		case AnimationType_SwitchPage:
			PlaySwitchPageAction((SwitchPageAction*)action);
			break;
		default:
			string uuid = action->GetTargetScUuid();
			PlayerPage* page = _allPages[uuid];
			if(page != NULL)
				page->PlayAction(action);
			break;
		}
	}
}

void PlayerWidget::PlaySwitchPageAction(SwitchPageAction* action)
{
	string uuid = action->GetTargetScUuid();
	_currentPage = _allPages[uuid];
	
	//2014.11.06,这里可能是bug的原因
	//我想在这里reset element
	//_currentPage->ResetElement();
	if(_mainLayout->indexOf(_currentPage) < 0)
		_mainLayout->addWidget(_currentPage);
	_mainLayout->setCurrentWidget(_currentPage);
}

void PlayerWidget::mousePressEvent(QMouseEvent *e)
{
	_isMouseDown = true;
	_downPoint = e->pos();
}

void PlayerWidget::mouseMoveEvent(QMouseEvent *e)
{
	if(_isMouseDown)
	{
		_moveDirection = e->pos().x() - _downPoint.x();
	}
}

void PlayerWidget::mouseReleaseEvent(QMouseEvent *e)
{
	if(_isMouseDown && _keTime != NULL)
	{
		int time = _keTime->GetAnimatedTime();

		if(_moveDirection > 0)
			time += 5000;
		else
			time -= 5000;

		if(time < 0)
			time = 0;
		OnGotoPosition(time);
	}
	_isMouseDown = false;
	_moveDirection = 0;
}

void PlayerWidget::keyPressEvent(QKeyEvent * e)
{
	if (e->key() == Qt::Key_Space)
	{
		if (_keTime->IsPausing())
		{
			_keTime->Start();
		}
		else
		{
			_keTime->Pause();
		}
	}
}
