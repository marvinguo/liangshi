#include "SizeAction.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

SizeAction::SizeAction():
AnimationAction()
{
}

SizeAction::~SizeAction()
{
}

void SizeAction::Update(QPoint leftTop, int width, int height)
{
	_leftTop = leftTop;
	_width = width;
	_height = height;
}

LokiBuffer SizeAction::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

AnimationType SizeAction::GetType()
{
	return AnimationType_Size;
}

AnimationAction* SizeAction::CreateFromThis()
{
	SizeAction* action = new SizeAction();
	*action = *this;
	return action;
}