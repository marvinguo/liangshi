#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAndroidJniObject>
#include "CustomEvent.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    btn = new QPushButton("start", this);
    btn->setGeometry(QRect(10, 10, 100, 50));
    connect(btn, SIGNAL(clicked()), this, SLOT(startGps()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startGps()
{
    //QAndroidJniObject javaAction = QAndroidJniObject::fromString(url);
    QAndroidJniObject::callStaticMethod<void>(
        "com/mtn/mes/GpsService",
        "calledByCpp",
        "()V");

}

bool MainWindow::event(QEvent *e)
{
    if(e->type() == CustomEvent::eventType()){
        e->accept();
        CustomEvent *sce = (CustomEvent*)e;
       //_resultView->setText(sce->m_arg2);
        ui->textBrowser->setText(sce->m_arg2);
        return true;
    }
    return QWidget::event(e);
}
