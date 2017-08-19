#include "TestBanElement.h"
#include "BanFile.h"
#include "../common/Util.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

TestBanElement::TestBanElement():
BanElement()
{
}

TestBanElement::TestBanElement(TestBaseWidget* viwer):
BanElement(viwer)
{
}

TestBanElement::~TestBanElement()
{
}

ElementType TestBanElement::GetType()
{
	return ElementType_TestBan;
}

LokiBuffer TestBanElement::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

void TestBanElement::UpdateDataFromMsgPack()
{
	Element::UpdateDataFromMsgPack();
	_viewer = new TestBaseWidget();
	_viewer->	SetMsgPackData(_data);
	InitViewer();
}
