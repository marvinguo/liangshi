#include "MarkdownWidget.h"
#include "BanFile.h"
#include "../common/Util.h"
#include "BanBaseWidget.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

MarkdownWidget::MarkdownWidget():
Element()
{
}

MarkdownWidget::~MarkdownWidget()
{
}

ElementType MarkdownWidget::GetType()
{
	return ElementType_Media;
}

LokiBuffer MarkdownWidget::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

bool MarkdownWidget::IsPointIn(QPoint point)
{
	return false;
}

QRect MarkdownWidget::GetActualRect()
{
	return QRect();
}

void MarkdownWidget::DrawOnQtPainter(QPainter* painter)
{

}
