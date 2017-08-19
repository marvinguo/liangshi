#include "widget.h"
#include <QVBoxLayout>
#include <QString>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QGroupBox *groupBox = new QGroupBox;
    groupBox->setTitle("Select Program Language");
    mainLayout->addWidget(groupBox);

    QRadioButton *language = new QRadioButton("C++");
    QVBoxLayout *groupLayout = new QVBoxLayout(groupBox);
    groupLayout->addWidget(language);
    connect(language, SIGNAL(toggled(bool)), this, SLOT(onToggled()));

    language = new QRadioButton("Java");
    groupLayout->addWidget(language);
    connect(language, SIGNAL(toggled(bool)), this, SLOT(onToggled()));

    language = new QRadioButton("Objective-C");
    groupLayout->addWidget(language);
    connect(language, SIGNAL(toggled(bool)), this, SLOT(onToggled()));

    groupBox = new QGroupBox;
    groupBox->setTitle("Select Mobile OS");
    groupBox->setCheckable(true);
    mainLayout->addWidget(groupBox);

    QCheckBox *os = new QCheckBox("iOS");
    groupLayout = new QVBoxLayout(groupBox);
    groupLayout->addWidget(os);
    connect(os, SIGNAL(stateChanged(int)), this, SLOT(onCheckState(int)));

    os = new QCheckBox("Android");
    groupLayout->addWidget(os);
    connect(os, SIGNAL(stateChanged(int)), this, SLOT(onCheckState(int)));
    os = new QCheckBox("Windows Phone");
    groupLayout->addWidget(os);
    connect(os, SIGNAL(stateChanged(int)), this, SLOT(onCheckState(int)));

    m_stateLabel = new QLabel();
    mainLayout->addWidget(m_stateLabel);
    mainLayout->addStretch(1);
}

Widget::~Widget()
{

}

void Widget::onToggled()
{
    QRadioButton *btn = qobject_cast<QRadioButton*>(sender());
    m_stateLabel->setText(btn->text());
    m_stateLabel->adjustSize();
}

void Widget::onCheckState(int state)
{
    QCheckBox *btn = qobject_cast<QCheckBox*>(sender());
    QString stats = QString("%1 - %2").arg(btn->text()).arg(state);
    m_stateLabel->setText(stats);
    m_stateLabel->adjustSize();
}
