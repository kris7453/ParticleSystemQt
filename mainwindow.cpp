#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmainglwidget.h"

QdrawableListWidget *MainWindow::drawableListWidget;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint);

    QMenu *a = new QMenu();
    a->addAction("Plik");

    ui->mainMenu->setMenu(a);

    //---------------------------------------------------------------------------------------

    drawableListWidget = new QdrawableListWidget(ui->resourcesList);
    drawableListWidget->setActiveWidgets(ui->activeLayerUpBtn,
                                         ui->activeLayerDownBtn,
                                         ui->activeVisibilityBtn,
                                         ui->activeCloseBtn,
                                         ui->activeName);

    connect( ui->activeVisibilityBtn, &QPushButton::clicked,
             [this](){emit drawableListWidget->changeVisibility();});
    connect( ui->activeCloseBtn, &QPushButton::clicked,
             [this](){emit drawableListWidget->close();});
    connect( ui->resourcesList,         &QListWidget::itemClicked,
             drawableListWidget, &QdrawableListWidget::setActiveItemFromList);
    connect( ui->activeLayerUpBtn, &QPushButton::clicked,
             [this](){emit drawableListWidget->layerUp();});
    connect( ui->activeLayerDownBtn, &QPushButton::clicked,
             [this](){emit drawableListWidget->layerDown();});
}

MainWindow::~MainWindow()
{
    delete ui;
}
