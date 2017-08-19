#pragma once

#include "../common/Qt.h"
#include "../common/library.h"
#include "HtmlElementPainter.h"

class LIANGSHIEXPORT HtmlElementViewer : public QWidget
{
	Q_OBJECT
	friend class HtmlTextElementHelper;
public:
	HtmlElementViewer();
	~HtmlElementViewer();

	void SetHtmlText(QString& text, int size, QColor color);
	virtual void paintEvent(QPaintEvent *e);
	virtual QSize sizeHint() const;

private:
	HtmlElementPainter* _painter;
	QColor _color;
};