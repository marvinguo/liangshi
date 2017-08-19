#include "widget.h"
#include <QVBoxLayout>
#include <QFile>
#include <QXmlStreamReader>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_contacts = new QTableWidget;
    layout->addWidget(m_contacts);
    m_contacts->setColumnCount(2);
    QStringList headerLabels("Name");
    headerLabels << "Mobile";
    m_contacts->setHorizontalHeaderLabels(headerLabels);
    readContacts();
}

Widget::~Widget()
{

}

void Widget::readContacts()
{
    QFile file(":/contacts.xml");
    file.open(QFile::ReadOnly);
    QXmlStreamReader reader;
    reader.setDevice(&file);
    QStringRef elementName;
    int row = 0;
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            elementName = reader.name();
            if(elementName == "contacts")
            {
                int count = reader.attributes().value("count").toInt();
                m_contacts->setRowCount(count);
            }
            else if(elementName == "people")
            {
                QString peopleName = reader.attributes().value("name").toString();
                m_contacts->setItem(row, 0, new QTableWidgetItem(peopleName));
            }
            else if(elementName == "mobile")
            {
                QString mobile = reader.readElementText();
                m_contacts->setItem(row, 1, new QTableWidgetItem(mobile));
                row++;
            }
        }
    }
}
