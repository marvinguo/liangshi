#include "LineStyleAction.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

LineStyleAction::LineStyleAction():
AnimationAction()
{
}

LineStyleAction::~LineStyleAction()
{
}

LokiBuffer LineStyleAction::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

AnimationType LineStyleAction::GetType()
{
	return AnimationType_LineStyle;
}

void LineStyleAction::Update(Qt::PenStyle style)
{
	_style = (int)style;
}

AnimationAction* LineStyleAction::CreateFromThis()
{
	LineStyleAction* action = new LineStyleAction();
	*action = *this;
	return action;
}