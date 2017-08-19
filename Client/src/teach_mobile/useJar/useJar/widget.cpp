#include "widget.h"
#include <QVBoxLayout>
#include <QPushButton>
#include "../simpleCustomEvent.h"
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *getLayout = new QHBoxLayout();
    layout->addLayout(getLayout);
    m_urlEdit = new QLineEdit("http://blog.csdn.net/foruok");
    getLayout->addWidget(m_urlEdit, 1);
    QPushButton *getButton = new QPushButton("GET");
    getLayout->addWidget(getButton);
    connect(getButton, SIGNAL(clicked()), this, SLOT(onGet()));

    m_resultView = new QTextEdit();
    m_resultView->setReadOnly(true);
    layout->addWidget(m_resultView, 1);
    m_stateLabel = new QLabel();
    layout->addWidget(m_stateLabel);
}

Widget::~Widget()
{

}

bool Widget::event(QEvent *e)
{
    if(e->type() == SimpleCustomEvent::eventType())
    {
        e->accept();
        SimpleCustomEvent *sce = (SimpleCustomEvent*)e;
        switch(sce->m_arg1)
        {
        case 1:
            m_resultView->setText(sce->m_arg2);
            m_stateLabel->setText("Success!");
            break;
        case 0:
            m_resultView->setText(sce->m_arg2);
            m_stateLabel->setText("Failed!");
            break;
        case -1:
            m_stateLabel->setText(sce->m_arg2);
            break;
        }
        return true;
    }
    return QWidget::event(e);
}

void Widget::onGet()
{
#ifdef WIN32
    m_resultView->setText("Sorry, Just for Android!");
#elif defined(ANDROID)
    QString url = m_urlEdit->text();
    QAndroidJniObject javaAction = QAndroidJniObject::fromString(url);
    QAndroidJniObject::callStaticMethod<void>("an/qt/useJar/ExtendsQtWithJava",
                                       "downloadWebPage",
                                       "(Ljava/lang/String;)V",
                                       javaAction.object<jstring>());
    m_stateLabel->setText("Downloading...");
#endif
}
