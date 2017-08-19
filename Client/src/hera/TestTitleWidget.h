#pragma once

#include "../common/Qt.h"
#include "ColorButton.h"
#include "TestBaseWidget.h"
#include "TitleButton.h"

//想把script里的run，先加入到这里吧，这样的设计简单一些
class LIANGSHIEXPORT TestTitleWidget : public QWidget
{
	Q_OBJECT
public:
	TestTitleWidget(QWidget *parent = 0);
	~TestTitleWidget();

	void InitWithTestWidget(TestBaseWidget* testWidget);
	ColorButton* GetColorButton(){return _lineColor;}

Q_SIGNALS:
	void OnGetCorrect(vector<string>& correct);

private Q_SLOTS:
	void OnAddQuestionTriggered();
	void OnAddCorrectTriggered();
	void OnAddAnswerTriggered();
	void OnShowQuestionTriggered(bool show);
	void OnShowAnswerTriggered(bool show);

protected:

private:
	TitleButton* _addQuestion;
	TitleButton* _addCorrect;
	QLabel* _correct;
	TitleButton* _addAnswer;
	ColorButton* _lineColor;

	TestBaseWidget* _testWidget;
};

