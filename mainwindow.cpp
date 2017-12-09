#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qmainglwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint);

    QMenu *a = new QMenu();
    a->addAction("Plik");


    ui->mainMenu->setMenu(a);
}

MainWindow::~MainWindow()
{
    delete ui;
}