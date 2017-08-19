#include "register.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QKeyEvent>
#include <QCoreApplication>

Register::Register(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout(this);
    layout->setRowStretch(0, 1);
    layout->setRowStretch(4, 1);
    layout->setColumnStretch(1, 1);
    QLabel *nameLabel = new QLabel("姓名：");
    layout->addWidget(nameLabel, 1, 0, Qt::AlignRight | Qt::AlignVCenter);
    m_nameEdit = new QLineEdit();
    m_nameEdit->installEventFilter(this);
    layout->addWidget(m_nameEdit, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    QLabel *passwordLabel = new QLabel("密码：");
    layout->addWidget(passwordLabel, 2, 0, Qt::AlignRight | Qt::AlignVCenter);
    m_passwordEdit = new QLineEdit();
    layout->addWidget(m_passwordEdit, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);

    QHBoxLayout *actionLayout = new QHBoxLayout();
    layout->addLayout(actionLayout, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);

    m_confirmButton = new QPushButton("确定");
    connect(m_confirmButton, SIGNAL(clicked()), this, SLOT(onConfirmButton()));
    actionLayout->addWidget(m_confirmButton);
    m_cancelButton = new QPushButton("取消");
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(onCancelButton()));
    actionLayout->addWidget(m_cancelButton);
    actionLayout->addStretch(1);
}

Register::~Register()
{
}

void Register::onConfirmButton()
{

}

void Register::onCancelButton()
{

}

bool Register::eventFilter(QObject *watched, QEvent *e)
{
    if(e->type() == QEvent::KeyPress)
    {
        QKeyEvent *ke = (QKeyEvent*)e;
        if(ke->key() == Qt::Key_Left)
        {
            QKeyEvent backspace(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
            QCoreApplication::sendEvent(watched, &backspace);
            return true;
        }
    }
    return QWidget::eventFilter(watched, e);
}
