#pragma once

#include "../common/library.h"
#include "../common/Qt.h"
#include "HeraDocument.h"
#include "BanFile.h"
#include "BanDocument.h"
#include "BanElementWithLabel.h"

class LIANGSHIEXPORT TestBaseWidget : public BanBaseWidget
{
	Q_OBJECT
public:
	TestBaseWidget(bool embedded = false, QWidget *parent = 0);
	virtual ~TestBaseWidget();

	void AddQuestionSc(QString filepath);
	void AddAnswerSc(QString filepath);
	BanElementWithLabel* NeedQuestionElement();
	BanElementWithLabel* AddAnswerElement();
	void GetAnswerLabels(vector<string>& allAnswers, vector<string>&correctLabels);
	void SetCorrectLabel(vector<string>& lables){_correctLabels = lables;}

//HeraDocument virtual
public:
	virtual QWidget* Convert2QWidget(){return static_cast<QWidget*>(this);}

//MaterailDoc virtual
public:
	virtual ScType GetType(){return ScType_Test;}
	virtual LokiBuffer GetMsgPackData(bool init = false);
	virtual void SetMsgPackData(LokiBuffer& buffer);
	virtual void UpdateDataFromMsgPack();

Q_SIGNALS:
	void OnCanShowQuestion(bool show);
	void OnCanShowAnswer(bool show);

public Q_SLOTS:
	void OnHavenotSaveRelatedElementTriggered(QVariant& relatedData);
	virtual void RemoveDrawingElementTriggered(Element* element);
	void OnGetCorrectTriggered(vector<string>& correct);

private Q_SLOTS:

private:
	void EmitQuestionChanged();
	void EmitAnswerChanged();

private:

	string _questionUuid;
	vector<string> _answerUuids;
	vector<BanElement*> _answerElements;
	vector<string> _correctLabels;

public:
	MSGPACK_DEFINE(_questionUuid, _answerUuids, _correctLabels);
};

