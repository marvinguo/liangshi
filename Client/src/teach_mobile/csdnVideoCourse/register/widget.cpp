#include "widget.h"
#include <QSize>
#include <QResizeEvent>
#include "register.h"
#include <QEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QPushButton * button = new QPushButton("注册", this);
    connect(button, SIGNAL(clicked()), this, SLOT(onRegisterButton()));
}

Widget::~Widget()
{
}

bool Widget::eventFilter(QObject *watched, QEvent *e)
{
    if(e->type() == QEvent::KeyPress)
    {
        if(((QKeyEvent*)e)->key() == Qt::Key_Home)
        {
            for(int i = 0; i < m_subwidgets.size(); i++)
            {
                m_subwidgets.at(i)->deleteLater();
            }
            m_subwidgets.clear();
            e->accept();
            return true;
        }
    }
    return QWidget::eventFilter(watched, e);
}

void Widget::onRegisterButton()
{
    Register *reg = new Register(this);
    reg->setAutoFillBackground(true);
    reg->setGeometry(rect());
    m_subwidgets.push_front(reg);
    reg->show();
}

void Widget::resizeEvent(QResizeEvent *e)
{
    if(m_subwidgets.size())
    {
        QSize size = e->size();
        m_subwidgets.front()->setGeometry(0, 0, size.width(), size.height());
    }
}
