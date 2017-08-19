#include "TextChangedAction.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

TextChangedAction::TextChangedAction():
AnimationAction()
{
}

TextChangedAction::~TextChangedAction()
{
}

LokiBuffer TextChangedAction::GetMsgPackData()
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(*this);
	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

AnimationType TextChangedAction::GetType()
{
	return AnimationType_TextChanged;
}

void TextChangedAction::Update(QString text, int fontSize)
{
	_text = text.toStdString();
	_fontSize = fontSize;
}

AnimationAction* TextChangedAction::CreateFromThis()
{
	TextChangedAction* action = new TextChangedAction();
	*action = *this;
	return action;
}