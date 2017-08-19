#include "ShowAction.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

ShowAction::ShowAction():
AnimationAction()
{
}

ShowAction::~ShowAction()
{
}

LokiBuffer ShowAction::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

AnimationType ShowAction::GetType()
{
	return AnimationType_Show;
}

AnimationAction* ShowAction::CreateFromThis()
{
	ShowAction* action = new ShowAction();
	*action = *this;
	return action;
}