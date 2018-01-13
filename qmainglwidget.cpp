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

    pixelWidth = 2.0f / this->width();
    pixelHeight = 2.0f / this->height();

    lastUpdate = QTime::currentTime().msec();

    sprogram = new QOpenGLShaderProgram();
    sprogram->create();
    sprogram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/particle.frag");
    sprogram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/particle.vert");
    sprogram->link();

    texture = new QOpenGLTexture(QImage(QString(":/particles/star.png")).mirrored());
    texture->generateMipMaps();
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    PSystemAPI::pDrawableItem::initialize(oGLFunct);
    pSystem1 = new PSystemAPI::pParticleSystem(QVector2D(0,0));
    //pSystem2 = new PSystemAPI::pParticleSystem(QVector2D(-100,100),QVector4D(0.0f, 1.0f, 0.0f, 1.0f));

    activeItem = pSystem1;

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
    //lastUpdate = updateTime->elapsed();


    drawParticles(deltaT);

    //qDebug() << deltaT ;


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

    //oGLFunct->glBindVertexArray(vao);

    oGLFunct->glActiveTexture(GL_TEXTURE0);
    texture->bind();
    pSystem1->updateParticles(deltaT);
    pSystem1->draw();
    //pSystem2->draw();
    //oGLFunct->glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 5);
}

void QmainGLWidget::mousePressEvent(QMouseEvent *e)
{
    activeItem->saveLastPosition();
    mouseClickPosition = e->pos();
}

void QmainGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    activeItem->translatePosition(QVector2D(e->pos() - mouseClickPosition)/0.5f);
}
