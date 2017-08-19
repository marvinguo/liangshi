#include "NetworkShowAction.h"
#include "../common/FileUtil.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

NetworkShowAction::NetworkShowAction():
AnimationAction()
{
}

NetworkShowAction::~NetworkShowAction()
{
}

LokiBuffer NetworkShowAction::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

AnimationType NetworkShowAction::GetType()
{
	return AnimationType_NetworkShow;
}

AnimationAction* NetworkShowAction::CreateFromThis()
{
	NetworkShowAction* action = new NetworkShowAction();
	*action = *this;
	return action;
}

void NetworkShowAction::Update(int type, LokiBuffer& data)
{
	_elementType = type;
	//数据启用压缩
	_elementData = FileUtil::ZipLokiBuffer(data);
}

int NetworkShowAction::GetElementType()
{
	return _elementType;
}

LokiBuffer NetworkShowAction::GetElementBuffer()
{
	return FileUtil::UnZipLokiBuffer(_elementData);
}
