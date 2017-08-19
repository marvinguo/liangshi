#include "HideAction.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

HideAction::HideAction():
AnimationAction()
{
}

HideAction::~HideAction()
{
}

LokiBuffer HideAction::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

AnimationType HideAction::GetType()
{
	return AnimationType_Hide;
}

AnimationAction* HideAction::CreateFromThis()
{
	HideAction* action = new HideAction();
	*action = *this;
	return action;
}