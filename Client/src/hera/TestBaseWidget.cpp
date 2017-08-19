#include "TestBaseWidget.h"
#include "../common/FileUtil.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

TestBaseWidget::TestBaseWidget(bool embedded, QWidget *parent):
BanBaseWidget(embedded, parent)
{
	SetMode(DrawMode_Select);
	_disableDraw = true;
	_disablePaste = true;
}

TestBaseWidget::~TestBaseWidget()
{
}

void TestBaseWidget::AddQuestionSc(QString filepath)
{
	vector<BanElement*> elements  = BanFile::LoadAsScElements(filepath, true);

	for(vector<BanElement*>::iterator i = elements.begin(); i != elements.end(); ++i)
	{
		AddElement(*i);
		_questionUuid = (*i)->GetObjectUuid();
	}

	EmitQuestionChanged();
}

void TestBaseWidget::AddAnswerSc(QString filepath)
{
	vector<BanElement*> elements  = BanFile::LoadAsScElements(filepath, true);

	for(vector<BanElement*>::iterator i = elements.begin(); i != elements.end(); ++i)
	{
		BanElement* element = *i;
		AddElement(element);
		_answerElements.push_back(element);
		_answerUuids.push_back(element->GetObjectUuid());
	}

	EmitAnswerChanged();
}

BanElementWithLabel* TestBaseWidget::NeedQuestionElement()
{
	BanBaseWidget* viewer = new BanBaseWidget(true);
	BanElementWithLabel* element = new BanElementWithLabel(viewer);
	AddElement(element);
	_questionUuid = element->GetObjectUuid();
	EmitQuestionChanged();
	return element;
}

BanElementWithLabel* TestBaseWidget::AddAnswerElement()
{
	BanBaseWidget* viewer = new BanBaseWidget(true);
	BanElementWithLabel* element = new BanElementWithLabel(viewer);
	AddElement(element);
	_answerElements.push_back(element);
	_answerUuids.push_back(element->GetObjectUuid());
	EmitAnswerChanged();
	return element;
}

void TestBaseWidget::OnHavenotSaveRelatedElementTriggered(QVariant& relatedData)
{
	RemoveDrawingElementTriggered(VPtr<Element>::asPtr(relatedData));
}

void TestBaseWidget::RemoveDrawingElementTriggered(Element* element)
{
	string uuid = element->GetObjectUuid();
	if(_questionUuid == uuid)
	{
		_questionUuid = "";
	}

	_answerUuids.erase(remove(_answerUuids.begin(), _answerUuids.end(), uuid), _answerUuids.end());
	_answerElements.erase(remove(_answerElements.begin(), _answerElements.end(), element), _answerElements.end());
	
	EmitQuestionChanged();
	EmitAnswerChanged();
	BanBaseWidget::RemoveDrawingElementTriggered(element);
}

void TestBaseWidget::GetAnswerLabels(vector<string>& allAnswers, vector<string>&correctLabels)
{
	correctLabels = _correctLabels;

	for(vector<BanElement*>::iterator i = _answerElements.begin(); i != _answerElements.end(); ++i)
	{
		allAnswers.push_back((*i)->GetLabel().toStdString());
	}
}

void TestBaseWidget::OnGetCorrectTriggered(vector<string>& correct)
{
	SetModifiedTriggered(true);
	_correctLabels = correct;
}

LokiBuffer TestBaseWidget::GetMsgPackData(bool init)
{
	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);

	//pack this
	msgpack::sbuffer thisBuffer;
	msgpack::packer<msgpack::sbuffer> packerThis(&thisBuffer);
	packerThis.pack(*this);
	packer.pack_raw(thisBuffer.size());
	packer.pack_raw_body(thisBuffer.data(), thisBuffer.size());

	//pack parent
	LokiBuffer parentBuffer = BanBaseWidget::GetMsgPackData(init);
	packer.pack_raw(parentBuffer.Size());
	packer.pack_raw_body(parentBuffer.Data(), parentBuffer.Size());

	LokiBuffer buffer(sbuffer.data(), sbuffer.size());
	return buffer;
}

//变成试题做的时候 ，我想还是要变图片，以后优化这个吧
void TestBaseWidget::SetMsgPackData(LokiBuffer& buffer)
{
	msgpack::unpacker pac;
	pac.reserve_buffer(buffer.Size());
	memcpy(pac.buffer(), buffer.Data(), buffer.Size());
	pac.buffer_consumed(buffer.Size());

	bool getThis = false;

	msgpack::unpacked result;
	while(pac.next(&result))
	{
		msgpack::object object = result.get();

		if(object.type == msgpack::type::RAW)
		{
			if(!getThis)
			{
				msgpack::unpacked msg;
				msgpack::unpack(&msg, object.via.raw.ptr, object.via.raw.size);
				msg.get().convert(this);
				getThis = true;
			}
			else
			{
				LokiBuffer rawLokiBuffer(const_cast<char*>(object.via.raw.ptr), object.via.raw.size);
				BanBaseWidget::SetMsgPackData(rawLokiBuffer);
			}
		}
	}
}

void TestBaseWidget::UpdateDataFromMsgPack()
{
	_answerElements.clear();
	for(list<Element*>::iterator i = _drawingElements.begin(); i != _drawingElements.end(); ++i)
	{
		BanElement* scElement = dynamic_cast<BanElement*>(*i);
		if(scElement != NULL)
		{
			vector<string>::iterator thisone = std::find(_answerUuids.begin(), _answerUuids.end(), scElement->GetObjectUuid());
			if(thisone != _answerUuids.end())
			{
				_answerElements.push_back(scElement);
			}
		}
	}

	EmitQuestionChanged();
	EmitAnswerChanged();
	BanBaseWidget::UpdateDataFromMsgPack();
}

void TestBaseWidget::EmitQuestionChanged()
{
	Q_EMIT OnCanShowQuestion(_questionUuid.empty());
}

void TestBaseWidget::EmitAnswerChanged()
{
	Q_EMIT OnCanShowAnswer(_answerUuids.size() > 0);
}
