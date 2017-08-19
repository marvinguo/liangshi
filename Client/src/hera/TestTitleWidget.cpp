#include "TestTitleWidget.h"
#include "AddCorrectDialog.h"
#include "HeraMainWidget.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

TestTitleWidget::TestTitleWidget(QWidget *parent):
QWidget(parent)
{
	_addQuestion = new TitleButton(tr("加入题目"));
	_addQuestion->setToolTip(tr("加入题目板书"));
	connect(_addQuestion, &TitleButton::clicked, this, &TestTitleWidget::OnAddQuestionTriggered);

	_addAnswer = new TitleButton(tr("加入答案"));
	_addAnswer->setToolTip(tr("加入答案板书"));
	//_addAnswer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	connect(_addAnswer, &TitleButton::clicked, this, &TestTitleWidget::OnAddAnswerTriggered);

	_addCorrect = new TitleButton(tr("设置正确答案"));
	_addCorrect->setVisible(false);
	connect(_addCorrect, &TitleButton::clicked, this, &TestTitleWidget::OnAddCorrectTriggered);

	_correct = new QLabel();
	//QSpacerItem * spacer = new QSpacerItem(600, 600, QSizePolicy::Maximum, QSizePolicy::Ignored);
	_lineColor = new ColorButton(tr("笔的颜色"));
	_lineColor->setText(tr("标签色"));

	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->addWidget(_addQuestion);
	mainLayout->addWidget(_addAnswer);
	mainLayout->addWidget(_addCorrect);
	mainLayout->addWidget(_correct);
	mainLayout->addWidget(_lineColor);
	mainLayout->addStretch();
	//mainLayout->addItem(spacer);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	//mainLayout->setSizeConstraint(QLayout::SetMinimumSize);

	//setFixedWidth(300);
	setFixedHeight(22);
	setLayout(mainLayout);
}

TestTitleWidget::~TestTitleWidget()
{

}

void TestTitleWidget::InitWithTestWidget(TestBaseWidget* testWidget)
{
	_testWidget = testWidget;
	connect(this, &TestTitleWidget::OnGetCorrect, _testWidget, &TestBaseWidget::OnGetCorrectTriggered);
	connect(_testWidget, &TestBaseWidget::OnCanShowQuestion, this, &TestTitleWidget::OnShowQuestionTriggered);
	connect(_testWidget, &TestBaseWidget::OnCanShowAnswer, this, &TestTitleWidget::OnShowAnswerTriggered);
}


void TestTitleWidget::OnAddQuestionTriggered()
{
	//换方式，还是用以前的那种
#if 0
	QString path = QFileDialog::getOpenFileName(this, QObject::tr("打开题目板书"), ".", QObject::tr("板书文件(*.ban)"));
	if(!path.isEmpty())
	{
		_testWidget->AddQuestionSc(path);
	}
#endif
	QWidget* window = topLevelWidget();
	if(window != NULL)
	{
		window->hide();
		BanElement* element = _testWidget->NeedQuestionElement();
		QVariant variant = qVariantFromValue((void*)element);
		HeraMainWidget* hera = HeraMainWidget::LoadFromLokiBuffer(NewableDocType_BanEmbedded, element->_data, variant, tr("编辑试题题目"));
		if(hera != NULL)
		{
			connect(hera, &HeraMainWidget::OnGetScBuffer, _testWidget, &TestBaseWidget::OnGetScBufferTriggered);
			connect(hera, &HeraMainWidget::OnHavenotSaveRelatedElement, _testWidget, &TestBaseWidget::OnHavenotSaveRelatedElementTriggered);
			connect(hera, SIGNAL(OnClosed()), window, SLOT(OnShowTriggered()));
			hera->ShowModal();
		}
	}
}

void TestTitleWidget::OnAddAnswerTriggered()
{
#if 0
	QString path = QFileDialog::getOpenFileName(this, QObject::tr("打开答案板书"), ".", QObject::tr("板书文件(*.ban)"));
	if(!path.isEmpty())
	{
		_testWidget->AddAnswerSc(path);
	}
#endif
	QWidget* window = topLevelWidget();
	if(window != NULL)
	{
		window->hide();
		BanElement* element = _testWidget->AddAnswerElement();
		QVariant variant = qVariantFromValue((void*)element);
		HeraMainWidget* hera = HeraMainWidget::LoadFromLokiBuffer(NewableDocType_BanEmbedded, element->_data, variant, tr("编辑试题答案"));
		if(hera != NULL)
		{
			connect(hera, &HeraMainWidget::OnGetScBuffer, _testWidget, &TestBaseWidget::OnGetScBufferTriggered);
			connect(hera, &HeraMainWidget::OnHavenotSaveRelatedElement, _testWidget, &TestBaseWidget::OnHavenotSaveRelatedElementTriggered);
			connect(hera, SIGNAL(OnClosed()), window, SLOT(OnShowTriggered()));
			hera->ShowModal();
		}
	}
}

void TestTitleWidget::OnAddCorrectTriggered()
{
	AddCorrectDialog* dialog= AddCorrectDialog::Instance();
	connect(dialog, &AddCorrectDialog::OnGetCorrect, this, &TestTitleWidget::OnGetCorrect);

	vector<string> allAnswers;
	vector<string> correctLabels;
	_testWidget->GetAnswerLabels(allAnswers, correctLabels);
	dialog->ShowModal(allAnswers, correctLabels);
}

void TestTitleWidget::OnShowQuestionTriggered(bool show)
{
	_addQuestion->setVisible(show);
}

void TestTitleWidget::OnShowAnswerTriggered(bool show)
{
	_addCorrect->setVisible(show);
}