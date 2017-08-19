#pragma once

#include "AnimationAction.h"
#include "ElementLabelBorder.h"

class ImageBaseElement;

class LIANGSHIEXPORT ImageElementHelper : public ElementLabelBorder
{
	Q_OBJECT
public:
	ImageElementHelper(ImageBaseElement* imageElement, QWidget *parent = 0);
	virtual ~ImageElementHelper();

Q_SIGNALS:

private Q_SLOTS:
	void OnDeleteTriggered();
	void OnTopMostTriggered();
	void OnBottomMostTriggered();

//QLable virtual
private:
	virtual void showEvent(QShowEvent * e);
	virtual void hideEvent(QHideEvent * e);

//ResizableElementHelperBase virtual
private:
	virtual string GetObjUuid();
	virtual void SendSizeAnimation(const QSize& size);

private:
	ImageBaseElement* _imageElement;
};

