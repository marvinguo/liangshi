#pragma once

#include "AnimationAction.h"
#include "Element.h"
#include "ElementLabel.h"
#include "ElementResizableTitlebar.h"

enum ResizableElementDirection
{
	ResizableElementDirection_UP = 1,
	ResizableElementDirection_DOWN,
	ResizableElementDirection_LEFT,
	ResizableElementDirection_RIGHT,
	ResizableElementDirection_LEFTTOP,
	ResizableElementDirection_LEFTBOTTOM,
	ResizableElementDirection_RIGHTBOTTOM,
	ResizableElementDirection_RIGHTTOP,
	ResizableElementDirection_NONE,
};

class BanBaseWidget;

class LIANGSHIEXPORT ElementLabelBorder : public QWidget
{
	Q_OBJECT
public:
	ElementLabelBorder(QWidget *parent = 0, bool canResize = true);
	virtual ~ElementLabelBorder();

public:
	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);
	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	virtual void resizeEvent(QResizeEvent *e);
	virtual void moveEvent(QMoveEvent *e);
	virtual void showEvent(QShowEvent * e);
	virtual void hideEvent(QHideEvent * e);
	virtual void paintEvent(QPaintEvent *e);
	virtual void wheelEvent(QWheelEvent *e);

Q_SIGNALS:
	void OnAnimationAction(AnimationAction& action, Element* element);
	void OnDeleteElement(Element* element);
	void OnTopMostElement(Element* element);
	void OnBottomMostElement(Element* element);
	void OnDirty();

protected:
	virtual string GetObjUuid() = 0;
	void SetPixmap(QPixmap& pixmap, bool needResize = false);
	void Move(int x, int y);
	virtual void SendSizeAnimation(const QSize& size);
	virtual void ChangeParentWidget(QWidget* parent);

private:
	void DetectNeedResize(const QPoint &point);

protected:
	bool _isLeftPressDown;
	bool _canResize;
	QPoint _dragPosition;
	QPoint _clickPoint;
	ResizableElementDirection _direction;

	ElementResizableTitlebar* _titleBar;
	ElementLabel* _label;
	BanBaseWidget* _banBase;

	QColor _borderColor;
};

