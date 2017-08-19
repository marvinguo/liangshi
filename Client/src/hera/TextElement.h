#pragma once

#include "Element.h"
#include "TextElementHelper.h"

class LIANGSHIEXPORT TextElement : public Element
{
	friend class BanBaseWidget;
	friend class TextElementHelper;
	friend class PlayerPage;
	friend class LiveWidget;
	friend class TextElementScriptValue;
	friend class DrawingToolsTitleWidget;

	struct State
	{
		QColor color;
		QPoint leftTop;
		int size;
		string text;
	};
public:
	TextElement();
	TextElement(QPoint leftTop, QString text, int fontSize = DEFAULT_FONT_SIZE);
	virtual ~TextElement();

public:
//Element virtual
	virtual void SetLineWidth(int width);
	virtual void SetLineColor(QColor color);
	virtual void UpdateSize(QPoint point, int width = -1, int height = -1);
	virtual ElementType GetType();
	virtual bool IsPointIn(QPoint point);
	virtual QRect GetActualRect();
	virtual void Highlight(QWidget* parent, bool high = true);
	virtual void DrawOnQtPainter(QPainter* painter);
	virtual LokiBuffer GetMsgPackData();
	virtual void SaveInitState();
	virtual void ToInitState();
	virtual void ToCurrentState();

	QPoint GetActualLeftTop();

private:
	void Init(QPoint leftTop, QString text, int fontSize);
	QRectF GetTextRect(bool withLeading = true);


private:
	TextElementHelper* _helper;

protected:
	//save part
	//SPoint _leftTop;
	SPoint _leftTop;
	double _fontSize;
	int _fontWeight;
	int _width;
	int _height;
	string _text;

	State _initState;
	State _currentState;
public:
	MSGPACK_DEFINE(_objUuid, _lineColor, _matrix, _leftTop, _width, _height, _fontSize, _fontWeight, _text);
};

