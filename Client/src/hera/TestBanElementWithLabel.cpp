#include "TestBanElementWithLabel.h"
#include "BanFile.h"
#include "../common/Util.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

TestBanElementWithLabel::TestBanElementWithLabel():
BanElementWithLabel()
{
}

TestBanElementWithLabel::TestBanElementWithLabel(TestBaseWidget* viwer):
BanElementWithLabel(viwer)
{
}

TestBanElementWithLabel::~TestBanElementWithLabel()
{
}

ElementType TestBanElementWithLabel::GetType()
{
	return ElementType_TestBanWithLabel;
}

LokiBuffer TestBanElementWithLabel::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

void TestBanElementWithLabel::UpdateDataFromMsgPack()
{
	Element::UpdateDataFromMsgPack();
	_viewer = new TestBaseWidget();
	_viewer->	SetMsgPackData(_data);
	InitViewer();
}
