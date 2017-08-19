#pragma once

#include "AnimationAction.h"
#include "ElementLabelBorder.h"

class BanElement;

class LIANGSHIEXPORT BanElementHelper : public ElementLabelBorder
{
	Q_OBJECT
	friend class BanElement;
public:
	BanElementHelper(BanElement* materialElement, QWidget *parent = 0);
	virtual ~BanElementHelper();

Q_SIGNALS:

private Q_SLOTS:
	void OnDeleteTriggered();
	void OnEditTriggered();
	void OnTitleTriggered();
	void OnTopMostTriggered();
	void OnBottomMostTriggered();

//QLable virtual
private:
	virtual void showEvent(QShowEvent * e);
	virtual void hideEvent(QHideEvent * e);

//ResizableElementHelperBase virtual
private:
	virtual string GetObjUuid();
	virtual void UpdateImage();
	virtual void SendSizeAnimation(const QSize& size);

private:
	BanElement* _materialElement;
};

