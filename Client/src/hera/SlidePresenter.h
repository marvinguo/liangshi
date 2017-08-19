#pragma once

#include "AnimationAction.h"
#include "ElementLabelBorder.h"
#include "Slide.h"

class LIANGSHIEXPORT SlidePresenter : public ElementLabelBorder
{
	Q_OBJECT
public:
	SlidePresenter(QWidget *parent = 0);
	virtual ~SlidePresenter();

	Slide* AddSlideFromImage(QImage& image, QSize size);
	void AddSlide(Slide* slide);

	void BeginShow();
	void ShowCurrentSlide();
	void SetAllSlidesBanBase();
	int GetSlideCount(){ return _slides.size(); }

Q_SIGNALS:

private Q_SLOTS:
	void OnPreviousAction();
	void OnNextAction();

//ElementLabelBorder virtuals
public:
	virtual string GetObjUuid();
	virtual void SendSizeAnimation(const QSize& size);
	virtual void ChangeParentWidget(QWidget* parent);

//QWidget virtuals
private:
	virtual void mousePressEvent(QMouseEvent *e);
	virtual void keyPressEvent(QKeyEvent *e);

private:
	vector<Slide*> _slides;
	int _currentSlideIndex;

	bool _hasBeenSetSize;
	QPoint _leftTop;
	QSize _size;
};

