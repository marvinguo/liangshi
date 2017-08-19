#include "SwitchPageAction.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

SwitchPageAction::SwitchPageAction():
AnimationAction()
{
}

SwitchPageAction::~SwitchPageAction()
{
}

LokiBuffer SwitchPageAction::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

AnimationType SwitchPageAction::GetType()
{
	return AnimationType_SwitchPage;
}

AnimationAction* SwitchPageAction::CreateFromThis()
{
	SwitchPageAction* action = new SwitchPageAction();
	*action = *this;
	return action;
}