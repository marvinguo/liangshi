#pragma once

#include "Element.h"
#include "BanElementHelper.h"
#include "BanBaseWidget.h"

class LIANGSHIEXPORT BanElement : public Element
{
	friend class BanElementHelper;
	friend class BanBaseWidget;
	friend class BanStateElement;
	friend class TestTitleWidget;
	friend class PlayerPage;
	friend class LiveWidget;
public:
	struct State
	{
		SPoint leftTop;
		int zoomWidth;
		int zoomHeight;
		SMatrix matrix;
	};
public:
	BanElement();
	BanElement(BanBaseWidget* viwer);
	virtual ~BanElement();
	void UpdateFromBuffer(LokiBuffer& buffer);
	bool IsEmptyInside();

public:
	virtual bool HasLabel(){return false;}
	virtual void SetLabel(QString title){}
	virtual QString GetLabel(){return "";}
	ScType GetScType();

public:
	virtual void SetLineWidth(int width);
	virtual void SetLineStyle(int style);
	virtual void SetLineColor(QColor color);
	virtual void UpdateSize(QPoint point, int width = -1, int height = -1);
	virtual ElementType GetType();
	virtual bool IsPointIn(QPoint point);
	virtual QRect GetActualRect();
	virtual void Highlight(QWidget* parent, bool high = true);
	virtual void DrawOnQtPainter(QPainter* painter);
	virtual LokiBuffer GetMsgPackData();
	virtual void UpdateDataFromMsgPack();
	virtual void SaveInitState();
	virtual void ToInitState();
	virtual void ToCurrentState();
	State GetState();
	void SetState(State state);

protected:
	void InitViewer();

private:
	void SetWidthHeight(int width, int height);
	void Init(QPoint leftTop, BanBaseWidget* viwer);

protected:
	BanElementHelper* _helper;

protected:
	BanBaseWidget* _viewer;
	//saved
	SPoint _leftTop;
	SPoint _leftTopInSc;
	int _width;
	int _height;
	double _xZoom;
	double _yZoom;
	int _zoomWidth;
	int _zoomHeight;
	LokiBuffer _data;

	State _initState;
	State _currentState;
public:
	MSGPACK_DEFINE(_objUuid, _matrix, _leftTop, _leftTopInSc, _width, _height, _xZoom, _yZoom, _zoomWidth, _zoomHeight, _data);
};

