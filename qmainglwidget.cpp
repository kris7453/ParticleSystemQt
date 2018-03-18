#include "qmainglwidget.h"

QmainGLWidget::QmainGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
}

QmainGLWidget::~QmainGLWidget()
{
}

void QmainGLWidget::initializeGL()
{   
    oGLFunct = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    oGLFunct->glClearColor( 0.0f, 0.0f, 0.0f, 1.0f);

    pixelSize.setX( 2.0f / this->width());
    pixelSize.setY( 2.0f / this->height());

    lastUpdate = QTime::currentTime().msec();

    sprogram = new QOpenGLShaderProgram();
    sprogram->create();
    sprogram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/particle.frag");
    sprogram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/particle.vert");
    sprogram->link();

    PSystemAPI::pDrawableItem::initialize(oGLFunct);
    itemsController = new PSystemAPI::pDrawItemsController( MainWindow::drawableListWidget);
    itemsController->setParametersController( new QparametersController(
                                                  MainWindow::emiterMode,
                                                  MainWindow::drawableParameters,
                                                  MainWindow::gravityParameters,
                                                  MainWindow::radialParameters,
                                                  MainWindow::outlook,
                                                  MainWindow::texturePath,
                                                  MainWindow::textureButton,
                                                  MainWindow::addSystemButton,
                                                  MainWindow::mainMenu,
                                                  this));

    MainWindow::drawableListWidget->setController(itemsController);

    updateTime = new QTime();
    updateInterval = new QTimer(this);
    updateInterval->setInterval(1000.0f / 60);
    updateInterval->connect(updateInterval,SIGNAL(timeout()),this,SLOT(update()));
    updateInterval->start();
    updateTime->start();
}

void QmainGLWidget::paintGL()
{
    float deltaT = updateTime->restart() / 1000.0f;
    drawParticles(deltaT);
}

void QmainGLWidget::resizeGL(int w, int h)
{
    oGLFunct->glViewport(0,0,w,h);

    pixelSize.setX( 2.0f / w);
    pixelSize.setY( 2.0f / h);

//    qDebug()<< "pixels size " << pixelSize.x()  << " " << pixelSize.y();
//    qDebug() << "psize " << (50*pixelSize.x()*0.5) << " " << (50*pixelSize.y()*0.5);

    paintGL();
}

void QmainGLWidget::drawParticles( float deltaT)
{
    oGLFunct->glClear( GL_COLOR_BUFFER_BIT );

    sprogram->bind();
    sprogram->setUniformValue("textureSamp",0);
    sprogram->setUniformValue("pixelSize", pixelSize);
    sprogram->setUniformValue("scale",0.5f);

    itemsController->drawItems(deltaT);
}

void QmainGLWidget::mousePressEvent(QMouseEvent *e)
{
    itemsController->saveLastPosition();
    mouseClickPosition = e->pos();
}

void QmainGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    itemsController->translatePosition(QVector2D(e->pos() - mouseClickPosition)/0.5f);
}
