#include "widget.h"
#include <QVBoxLayout>
#include <QComboBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_table = new QTableWidget;
    layout->addWidget(m_table);
    m_table->setColumnCount(3);
    m_table->setRowCount(3);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(m_table, SIGNAL(itemChanged(QTableWidgetItem*)),
            this, SLOT(onItemChanged(QTableWidgetItem*)));
    QStringList horzHeaders("语文");
    horzHeaders << "数学" << "评价";
    m_table->setHorizontalHeaderLabels(horzHeaders);
    QStringList vertHeaders("张三");
    vertHeaders << "李四" << "王五";
    m_table->setVerticalHeaderLabels(vertHeaders);
    QComboBox *eval0 = new QComboBox;
    eval0->addItem("通过");
    eval0->addItem("毙掉");
    eval0->setCurrentIndex(-1);
    connect(eval0, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onCurrentIndexChanged(int)));
    m_table->setCellWidget(0, 2, eval0);
    QComboBox *eval1 = new QComboBox;
    eval1->addItem("通过");
    eval1->addItem("毙掉");
    eval1->setCurrentIndex(-1);
    connect(eval1, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onCurrentIndexChanged(int)));
    m_table->setCellWidget(1, 2, eval1);
    QComboBox *eval2 = new QComboBox;
    eval2->addItem("通过");
    eval2->addItem("毙掉");
    eval2->setCurrentIndex(-1);
    connect(eval2, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onCurrentIndexChanged(int)));
    m_table->setCellWidget(2, 2, eval2);
}

Widget::~Widget()
{

}

void Widget::onItemChanged(QTableWidgetItem *item)
{
    if(item->text().toInt() < 60)
    {
        item->setTextColor(Qt::red);
    }
}

void Widget::onCurrentIndexChanged(int index)
{
    int row = m_table->currentRow();
    m_table->verticalHeaderItem(row)->setTextColor(index == 0 ? Qt::green : Qt::red);
}
