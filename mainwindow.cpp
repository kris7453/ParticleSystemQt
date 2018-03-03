#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmainglwidget.h"

QdrawableListWidget *MainWindow::drawableListWidget;

QToolBox *MainWindow::emiterMode;

QWidget *MainWindow::drawableParameters;
QWidget *MainWindow::gravityParameters;
QWidget *MainWindow::radialParameters;
QWidget *MainWindow::outlook;
QLabel *MainWindow::texturePath;
QPushButton *MainWindow::textureButton;
QPushButton *MainWindow::addSystemButton;

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

    emiterMode = ui->emiterModes;
    drawableParameters = ui->drawableWidget;
    gravityParameters = ui->gravityMode;
    radialParameters = ui->radialMode;
    outlook = ui->outlookScrollArea;

    drawableListWidget = new QdrawableListWidget(ui->resourcesList);
    drawableListWidget->setActiveWidgets(ui->activeLayerUpBtn,
                                         ui->activeLayerDownBtn,
                                         ui->activeVisibilityBtn,
                                         ui->activeCloseBtn,
                                         ui->activeName);

    texturePath = ui->texturePath;
    textureButton = ui->textureButton;
    addSystemButton = ui->addSystemBtn;

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
