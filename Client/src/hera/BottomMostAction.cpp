#include "BottomMostAction.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

BottomMostAction::BottomMostAction():
AnimationAction()
{
}

BottomMostAction::~BottomMostAction()
{
}

LokiBuffer BottomMostAction::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

AnimationType BottomMostAction::GetType()
{
	return AnimationType_BottomMost;
}

AnimationAction* BottomMostAction::CreateFromThis()
{
	BottomMostAction* action = new BottomMostAction();
	*action = *this;
	return action;
}