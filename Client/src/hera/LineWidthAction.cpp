#include "LineWidthAction.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

LineWidthAction::LineWidthAction():
AnimationAction()
{
}

LineWidthAction::~LineWidthAction()
{
}

LokiBuffer LineWidthAction::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

AnimationType LineWidthAction::GetType()
{
	return AnimationType_LineWidth;
}

void LineWidthAction::Update(int width)
{
	_width = width;
}

AnimationAction* LineWidthAction::CreateFromThis()
{
	LineWidthAction* action = new LineWidthAction();
	*action = *this;
	return action;
}