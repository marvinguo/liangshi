#include "widget.h"
#include <QIcon>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QLabel>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_mode = new QComboBox;
    layout->addWidget(m_mode);
    m_mode->addItem("List");
    m_mode->addItem("Icon");
    connect(m_mode, SIGNAL(currentTextChanged(QString)),
            this, SLOT(onListModeChanged(QString)));

    m_list = new QListWidget;
    layout->addWidget(m_list, 1);
    QListWidgetItem *item = new QListWidgetItem(QIcon(":/audioFile.png"), "audio file");
    m_list->addItem(item);
    item = new QListWidgetItem(QIcon(":/videoFile.png"), "video file");
    m_list->addItem(item);
    item = new QListWidgetItem(QIcon(":/imageFile.png"), "image file");
    m_list->addItem(item);
    item = new QListWidgetItem(QIcon(":/folder.png"), "folder");
    m_list->addItem(item);
    m_list->addItem("text file");

    QLabel *label = new QLabel;
    layout->addWidget(label);
    connect(m_list, SIGNAL(currentTextChanged(QString)),
            label, SLOT(setText(QString)));
}

Widget::~Widget()
{

}

void Widget::onListModeChanged(const QString &mode)
{
    if(mode == "List")
    {
        m_list->setViewMode(QListView::ListMode);
    }
    else
    {
        m_list->setViewMode(QListView::IconMode);
    }
}
