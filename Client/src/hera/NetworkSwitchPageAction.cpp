#include "NetworkSwitchPageAction.h"
#include "../common/FileUtil.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

NetworkSwitchPageAction::NetworkSwitchPageAction():
AnimationAction()
{
}

NetworkSwitchPageAction::~NetworkSwitchPageAction()
{
}

LokiBuffer NetworkSwitchPageAction::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

AnimationType NetworkSwitchPageAction::GetType()
{
	return AnimationType_NetworkSwitchPage;
}

AnimationAction* NetworkSwitchPageAction::CreateFromThis()
{
	NetworkSwitchPageAction* action = new NetworkSwitchPageAction();
	*action = *this;
	return action;
}

void NetworkSwitchPageAction::Update(QString uuid, LokiBuffer& data)
{
	_targetScUuid = uuid.toStdString();
	_pageData = FileUtil::ZipLokiBuffer(data);
}

QString NetworkSwitchPageAction::GetUuid()
{
	return QString(_targetScUuid.c_str());
}

LokiBuffer NetworkSwitchPageAction::GetPageBuffer()
{
	return FileUtil::UnZipLokiBuffer(_pageData);;
}