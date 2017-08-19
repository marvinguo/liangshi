#include "TopMostAction.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

TopMostAction::TopMostAction():
AnimationAction()
{
}

TopMostAction::~TopMostAction()
{
}

LokiBuffer TopMostAction::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

AnimationType TopMostAction::GetType()
{
	return AnimationType_TopMost;
}

AnimationAction* TopMostAction::CreateFromThis()
{
	TopMostAction* action = new TopMostAction();
	*action = *this;
	return action;
}