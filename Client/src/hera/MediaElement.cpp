#include "MediaElement.h"
#include "BanFile.h"
#include "../common/Util.h"
#include "BanBaseWidget.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

MediaElement::MediaElement():
Element()
{
}

MediaElement::~MediaElement()
{
}

ElementType MediaElement::GetType()
{
	return ElementType_Media;
}

LokiBuffer MediaElement::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

bool MediaElement::IsPointIn(QPoint point)
{
	return false;
}

QRect MediaElement::GetActualRect()
{
	return QRect();
}

void MediaElement::DrawOnQtPainter(QPainter* painter)
{

}
