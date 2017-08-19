#pragma once

#include "Element.h"

class LIANGSHIEXPORT MarkdownWidget : public Element
{
public:
	MarkdownWidget();
	virtual ~MarkdownWidget();

public:
//Element virtual
	virtual ElementType GetType();
	virtual LokiBuffer GetMsgPackData();
	virtual bool IsPointIn(QPoint point);
	virtual QRect GetActualRect();
	virtual void DrawOnQtPainter(QPainter* painter);

protected:

public:
	MSGPACK_DEFINE();
};

