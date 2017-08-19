#pragma once

#include "../common/stl.h"
#include "../common/library.h"
#include "../common/config.h"
#include "../../../SharedCode/LokiBuffer.h"
#include "../common/SColor.h"
#include "../common/SPoint.h"
#include "../common/SMatrix.h"
#include "../common/Util.h"
#include "../common/FontUtil.h"

enum ElementType
{
	ElementType_Draw = 100,
	ElementType_Line = 101,
	ElementType_Rect = 102,
	ElementType_Ellipse = 103,
	ElementType_Text = 104,
	ElementType_Image = 105,
	ElementType_Ban = 106,
	ElementType_BanWithLabel = 107,
	ElementType_TestBan = 108,
	ElementType_TestBanWithLabel = 109,
	//想想以后会加入media板书，应该是这个，media可以嵌入到test, sc里，也可以自己单独加入到ke里，如果sc里加入多个
	//就顺序播放
	ElementType_Media = 110,
	ElementType_CompressedImage = 111,
	ElementType_HtmlText = 112,
	ElementType_OriginalDraw = 113,
	ElementType_BanState = 114,
	ElementType_ImageState = 115,
};

class LIANGSHIEXPORT Element
{
	friend class BanBaseWidget;
protected:
	Element();
public:
	virtual ~Element();

public:
	static Element* CreateFromType(ElementType type, LokiBuffer& buffer);
	static Element* CreateFromCopyableData(LokiBuffer& buffer);

public:
	void Reset(bool hasMatrix = false);
	string GetObjectUuid();
	int GetLineWidth();
	int GetLineStyle();
	SMatrix GetMatrix();
	QColor GetLineColor();
	bool IsHightlighting();
	void SetCanDelete(bool can);
	bool CanDelete();
	void SetCanPaint(bool can);
	bool CanPaint();
	void SetCanPreview(bool can);
	bool CanPreview();

	LokiBuffer GetCopyableData();

//real interface
public:
	virtual void SetMatrix(QMatrix matrix);
	virtual void SetLineWidth(int width);
	virtual void SetLineStyle(int style);
	virtual void SetLineColor(QColor color);
	virtual void UpdateSize(QPoint point, int width = -1, int height = -1);
	virtual ElementType GetType() = 0;
	virtual bool IsPointIn(QPoint point) = 0;
	virtual QRect GetActualRect() = 0;
	virtual void Highlight(QWidget* parent, bool high = true);
	virtual void DrawOnQtPainter(QPainter* painter) = 0;
	virtual LokiBuffer GetMsgPackData() = 0;
	virtual void UpdateDataFromMsgPack();

	//2014.10.29,改设计了
	virtual void SaveInitState() = 0;
	virtual void ToInitState() = 0;
	virtual void ToCurrentState() = 0;

protected:
	string _objUuid;
	int _lineWidth;
	int _lineStyle;
	SColor _lineColor;
	QColor _lineQColor;
	SMatrix _matrix;

private:
	bool _highlight;
	bool _canDelete;
	bool _canPaint;
	bool _canPreview;
};

