#include "SlidePresenter.h"
#include "ImageElement.h"
#include "NotifyWindow.h"
#include "SlidePresenterManager.h"
#include "BanBaseWidget.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

SlidePresenter::SlidePresenter(QWidget *parent):
ElementLabelBorder(parent, true),
_hasBeenSetSize(false)
{
	_borderColor = Qt::magenta;
//	_titleBar->GetDeleteButton()->show();
//	connect(_titleBar->GetDeleteButton(), &QPushButton::clicked, this, &SlidePresenter::OnDeleteTriggered);
	SlidePresenterManager* manager = SlidePresenterManager::Instance();
	manager->AddPresenter(this);
}

SlidePresenter::~SlidePresenter()
{
	SlidePresenterManager* manager = SlidePresenterManager::Instance();
	manager->RemotePresenter(this);

	for(auto slide : _slides)
	{
		delete slide;
	}
	_slides.clear();
}

string SlidePresenter::GetObjUuid()
{
	return "";
}

//从基类继承的
//这里的逻辑需要重点理清
//当size改变时，以前的所有的shape,需要重新resize!!
void SlidePresenter::SendSizeAnimation(const QSize& size)
{
	QRect rect = _label->rect();
	QPoint global = _label->mapToGlobal(_label->rect().topLeft());

	int x = parentWidget()->mapFromGlobal(global).x();
	int y = parentWidget()->mapFromGlobal(global).y();

	QPoint leftTop(x, y);
	_leftTop = leftTop;
	_size = size;
	_hasBeenSetSize = true;

	//必须是所有的slide,因为next action会基于currentsize算
	int i = 0;
	for(auto slide : _slides)
	{
		slide->SetNewSize(leftTop, size, i == _currentSlideIndex);
		i++;
	}
}

void SlidePresenter::ChangeParentWidget(QWidget* parent)
{
	ElementLabelBorder::ChangeParentWidget(parent);
	//在改变之前，先hide
	//hide也不行，不能预览了
	/*
	if(_currentSlideIndex >= 0 && _currentSlideIndex < _slides.size())
	{
		Slide* slide = _slides[_currentSlideIndex];
		slide->Hide();
	}
	*/
	SetAllSlidesBanBase();
	if(_hasBeenSetSize)
	{
		_label->resize(_size);
		Move(_leftTop.x(), _leftTop.y());

		if(_currentSlideIndex >= 0 && _currentSlideIndex < _slides.size())
		{
			Slide* slide = _slides[_currentSlideIndex];
			slide->UpdateBanBase();
		}
	}
}

void SlidePresenter::mousePressEvent(QMouseEvent *e)
{
	//想了想，鼠标还点管移动，所以这个地方不能用click
	setFocus();
	return ElementLabelBorder::mousePressEvent(e);
}

void SlidePresenter::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Space)
	{
		OnNextAction();
	}

	if (e->key() == Qt::Key_Left)
	{
		OnPreviousAction();
	}

	if (e->key() == Qt::Key_Right)
	{
		OnNextAction();
	}

	return ElementLabelBorder::keyPressEvent(e);
}

Slide* SlidePresenter::AddSlideFromImage(QImage& image, QSize size)
{
	Slide* slide = new Slide(size);
	QPixmap pixmap = QPixmap::fromImage(image);
	slide->SetBackgroundPixmap(pixmap);
	_slides.push_back(slide);
	SetAllSlidesBanBase();
	return slide;
}

void SlidePresenter::AddSlide(Slide* slide)
{
	_slides.push_back(slide);
	SetAllSlidesBanBase();
}

void SlidePresenter::BeginShow()
{
	if(_slides.size() > 0)
	{
		_currentSlideIndex = 0;
		for (int i = _currentSlideIndex; i < _slides.size(); ++i)
		{
			Slide* slide = _slides[i];
			slide->ClearActions();
		}
		OnNextAction();
	}
	setFocus();
	show();
}

void SlidePresenter::ShowCurrentSlide()
{
	if(_currentSlideIndex >= 0 && _currentSlideIndex < _slides.size())
	{
		Slide* slide = _slides[_currentSlideIndex];
		QPixmap pixmap;
		slide->ToCurrentAction(pixmap);
		SetPixmap(pixmap);
	}
	setFocus();
	show();
}

void SlidePresenter::OnPreviousAction()
{
	if(_currentSlideIndex <= 0)
		return;

	if(_currentSlideIndex >= 1 && _currentSlideIndex < _slides.size())
	{
		_currentSlideIndex--;
		for(int i = _currentSlideIndex; i < _slides.size(); ++i)
		{
			Slide* slide = _slides[i];
			slide->ClearActions();
		}
		OnNextAction();
	}
}

void SlidePresenter::OnNextAction()
{
	if(_currentSlideIndex >= _slides.size())
		return;

	Slide* slide = _slides[_currentSlideIndex];

	QPixmap pixmap;
	if(slide->NextAction(pixmap))
	{
		if(_currentSlideIndex - 1 >= 0)
		{
			Slide* previousSlide = _slides[_currentSlideIndex - 1];
			previousSlide->Hide();
		}
		if(_currentSlideIndex + 1 <  _slides.size())
		{
			Slide* nextSlide = _slides[_currentSlideIndex + 1];
			nextSlide->Hide();
		}

		SetPixmap(pixmap);
		NotifyWindow* notify = NotifyWindow::Instance();
		notify->ShowNotify(QString(tr("幻灯片第%1页")).arg(_currentSlideIndex + 1));
	}
	else
	{
		if(_currentSlideIndex + 1 < _slides.size())
		{
			_currentSlideIndex++;
			OnNextAction();
		}
	}
	//全部不画，这个也不需要
	//_banBase->Repaint();
}

void SlidePresenter::SetAllSlidesBanBase()
{
	if(_banBase != NULL)
	{
		for(auto slide : _slides)
		{
			slide->SetBanBase(_banBase);
			slide->Hide();
		}
	}
}
