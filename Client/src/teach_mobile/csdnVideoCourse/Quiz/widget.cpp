#include "widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

static const char *g_questions[] = {
    "“床前明月光”中的床是什么？",
    "C语言之父？",
    "foruok的博客地址？"
};

static const char *g_answers[] = {
    "井台上的围栏，即井床",
    "Dennis Ritchie",
    "http://blog.csdn.net/foruok"
};

Widget::Widget(QWidget *parent)
    : QWidget(parent), m_questionIndex(0)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch(1);
    m_label = new QLabel(QString::fromUtf8(g_questions[0]));
    layout->addWidget(m_label, 0, Qt::AlignCenter);
    layout->addSpacing(10);
    QHBoxLayout *actionLayout = new QHBoxLayout;
    layout->addLayout(actionLayout);
    actionLayout->addStretch(1);
    m_questionButton = new QPushButton("Question");
    QObject::connect(m_questionButton, SIGNAL(clicked()),
                     this, SLOT(onQuestionClicked()));
    actionLayout->addWidget(m_questionButton);
    m_answerButton = new QPushButton("Answer");
    actionLayout->addWidget(m_answerButton);
    QObject::connect(m_answerButton, SIGNAL(clicked()),
                     this, SLOT(onAnswerClicked()));
    actionLayout->addStretch(1);
}

Widget::~Widget()
{

}

void Widget::onQuestionClicked()
{
    int count = sizeof(g_questions) / sizeof(g_questions[0]);
    m_questionIndex = (m_questionIndex + 1) % count;
    m_label->setText(QString::fromUtf8(g_questions[m_questionIndex]));
}

void Widget::onAnswerClicked()
{
    m_label->setText(QString::fromUtf8(g_answers[m_questionIndex]));
}
