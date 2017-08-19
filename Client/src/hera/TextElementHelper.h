#pragma once

#include "../common/config.h"
#include "../common/Qt.h"
#include "AnimationAction.h"
#include "ElementResizableTitlebar.h"

class TextElement;
class Element;
class BanBaseWidget;

class LIANGSHIEXPORT TextElementHelper : public QTextEdit
{
	Q_OBJECT
	friend class TextElement;
public:
	TextElementHelper(TextElement* textElement, QWidget *parent = 0);
	~TextElementHelper();

//QTextEdit
public:
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);
	virtual void resizeEvent(QResizeEvent *e);
	virtual void moveEvent(QMoveEvent *e);
	virtual void wheelEvent(QWheelEvent *e);
	virtual void keyPressEvent(QKeyEvent *e);
	virtual void showEvent(QShowEvent * e);
	virtual void hideEvent(QHideEvent *e);
	virtual void contextMenuEvent(QContextMenuEvent *e);

Q_SIGNALS:
	void OnAnimationAction(AnimationAction& action, Element* element);
	void OnDeleteElement(Element* element);
	void OnTopMostElement(Element* element);
	void OnBottomMostElement(Element* element);
	void OnDirty();

private Q_SLOTS:
	void OnTextChanged();
	void OnDeleteTriggered();
	void OnTopMostTriggered();
	void OnBottomMostTriggered();

private:
	void SendSizeAnimation();
	void SetFontParameters(bool needQ_EMIT = true);
	void UpdateSize();

private:
	QPoint _dragPosition;
	TextElement* _textElement;

	ElementResizableTitlebar* _titleBar;
	BanBaseWidget* _scBase;
};

