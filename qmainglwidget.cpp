#include "qmainglwidget.h"

QmainGLWidget::QmainGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
}

QmainGLWidget::~QmainGLWidget()
{
}

void QmainGLWidget::addParticleSystem(PSystemAPI::pParticleSystem *pSystem)
{
    itemsController->addParticleSystem(pSystem);
}

void QmainGLWidget::addParticleSystem(QString *resourceImagePath, QString name)
{
    itemsController->addParticleSystem(resourceImagePath, name);
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
                                                  MainWindow::drawableParameters,
                                                  MainWindow::gravityParameters,
                                                  MainWindow::radialParameters));
    MainWindow::drawableListWidget->setController(itemsController);

    addParticleSystem( new QString(":/particles/star.png"), QString("star"));
    addParticleSystem( new QString(":/particles/ball.png"), QString("ball"));

    updateTime = new QTime();
    t = new QTimer(this);
    t->setInterval(1);
    t->connect(t,SIGNAL(timeout()),this,SLOT(update()));
    t->start();
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

    qDebug()<< "pixels size " << pixelSize.x()  << " " << pixelSize.y();
    qDebug() << "psize " << (50*pixelSize.x()*0.5) << " " << (50*pixelSize.y()*0.5);

    paintGL();
}

void QmainGLWidget::drawParticles( float deltaT)
{
    oGLFunct->glClear( GL_COLOR_BUFFER_BIT );

    oGLFunct->glEnable(GL_BLEND);
    oGLFunct->glBlendFunc(GL_SRC_ALPHA,GL_ONE);

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
