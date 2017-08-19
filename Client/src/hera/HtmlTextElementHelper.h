#pragma once

#include "../common/config.h"
#include "../common/Qt.h"
#include "AnimationAction.h"
#include "ElementResizableTitlebar.h"
#include "HtmlElementViewer.h"
#include "ElementLabelBorder.h"

class HtmlTextElement;
class Element;
class BanBaseWidget;

class LIANGSHIEXPORT HtmlTextElementHelper : public ElementLabelBorder
{
	Q_OBJECT
	friend class HtmlTextElement;
public:
	HtmlTextElementHelper(HtmlTextElement* HtmlTextElement, QWidget *parent = 0);
	~HtmlTextElementHelper();

public:
	virtual void wheelEvent(QWheelEvent *e);
	virtual void showEvent(QShowEvent * e);
	virtual void hideEvent(QHideEvent *e);

Q_SIGNALS:
	void OnAnimationAction(AnimationAction& action, Element* element);
	void OnDeleteElement(Element* element);
	void OnTopMostElement(Element* element);
	void OnBottomMostElement(Element* element);
	void OnDirty();

private Q_SLOTS:
	void OnDeleteTriggered();
	void OnEditTriggered();
	void OnTopMostTriggered();
	void OnBottomMostTriggered();

private:
	void UpdatePixmap();
	virtual string GetObjUuid();
	virtual void SendSizeAnimation(const QSize& size);

private:
	QPoint _dragPosition;
	HtmlTextElement* _HtmlTextElement;
};

