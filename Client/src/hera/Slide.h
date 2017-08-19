#pragma once

#include "../common/library.h"
#include "../common/Qt.h"
#include "ImageElement.h"
#include "SizeAction.h"

class Slide : public QObject
{
	Q_OBJECT
public:
	Slide(QSize size);
	virtual ~Slide();

	//ppt的
	void AddShape(int id, QString filepath);
	void AddAction(int id, SizeAction* action);

	void SetBanBase(BanBaseWidget* banBase);
	void SetBackgroundPixmap(QPixmap& pixmap);
	void SetNewSize(QPoint& leftTop, const QSize& size, bool withAction = true, bool withShow = false);
	void GetCurrentShapeSize(int id, QPoint& leftTop, QSize& size);
	void ToCurrentAction(QPixmap& pixmap);
	bool NextAction(QPixmap& pixmap);
	void ClearActions();
	void Hide();
	void HideImageElement(ImageElement* element);
	void UpdateBanBase();

Q_SIGNALS:
	void OnAnimationAction(AnimationAction& action, Element* element);

private:
	bool AddToBanBase(ImageElement* element);
	void AddShowAction(ImageElement* element);
	void AddSizeAction(ImageElement* element, QPoint leftTop, QSize size);
	bool HasMoreMoveAction(int start, int end, int id);

private:
	//从Pixmap变成ImageElement,因为这样uuid保持一样
	ImageElement* _background;
	map<int, ImageElement*> _shapes;
	//动画，对应一次点击
	vector<ShapeAction> _shapeActions;
	int _actionIndex;
	map<QString, map<ImageElement*, bool>> _hasAddedToBanBase; 
	BanBaseWidget* _banBase;

	QPoint _leftTop;
	QSize _originalSize;
	QSize _currentSize;
};

