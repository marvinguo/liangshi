#include "HtmlElementViewer.h"
#include "../common/config.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

HtmlElementViewer::HtmlElementViewer():
_painter(new HtmlElementPainter(this))
{
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	//updateGeometry();
	//setMinimumHeight(100);
	//setMaximumHeight(800);
}

HtmlElementViewer::~HtmlElementViewer()
{
}

void HtmlElementViewer::SetHtmlText(QString& text, int size, QColor color)
{
	_painter->SetHtmlText(text, size);
	_color = color;
	QSizeF sizeF = _painter->GetTextSize();
	resize(sizeF.toSize());
	update();
}

void HtmlElementViewer::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	painter.fillRect(rect(), QColor(BLACKBOARD_BACKGROUND_COLOR));
	_painter->Draw(&painter, _color);
	QWidget::paintEvent(e);
}

QSize HtmlElementViewer::sizeHint() const
{
	QSizeF sizeF = _painter->GetTextSize();
	return sizeF.toSize();
}

