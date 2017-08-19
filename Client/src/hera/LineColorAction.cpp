#include "LineColorAction.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

LineColorAction::LineColorAction():
AnimationAction()
{
}

LineColorAction::~LineColorAction()
{
}

LokiBuffer LineColorAction::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

AnimationType LineColorAction::GetType()
{
	return AnimationType_LineColor;
}

void LineColorAction::Update(QColor color)
{
	_color = color.rgba();
}

AnimationAction* LineColorAction::CreateFromThis()
{
	LineColorAction* action = new LineColorAction();
	*action = *this;
	return action;
}