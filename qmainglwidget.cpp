#include "qmainglwidget.h"

QmainGLWidget::QmainGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    pSystem = new PSystem();
}

QmainGLWidget::~QmainGLWidget()
{

}

void QmainGLWidget::initializeGL()
{   
    oGLFunct = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    oGLFunct->glClearColor( 0.0f, 0.0f, 0.0f, 0.0f);



    pixelSize = QSize( width()/2, height()/2);


    pSystem->addParticles(1);
    vector<PParticle> *particles = pSystem->getPList();
    PParticle p = particles->front();
    qDebug() << "Position: " << p.position.x << " " << p.position.y;

    lastUpdate = QTime::currentTime().msec();

    //updateTime = new QTime();
    //t = new QTimer(this);
    //t->setInterval(1000/30);
    //t->connect(t,SIGNAL(timeout()),this,SLOT(paintGL()));
    //t->start();
    //updateTime->start();



    sprogram = new QOpenGLShaderProgram();
    sprogram->create();
    sprogram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/particle.frag");
    sprogram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/particle.vert");
    sprogram->link();

    box = new float[12]{-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
                       -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f};

    vao = new QOpenGLVertexArrayObject();
    vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

    if (vao->create())
        vao->bind();

    vbo->create();
    vbo->bind();
    vbo->allocate(box,12*sizeof(float));
    vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vao->release();

}

void QmainGLWidget::paintGL()
{
    //float deltaT = (updateTime->elapsed() - lastUpdate) / 1000.0f;
    //lastUpdate = updateTime->elapsed();
    oGLFunct->glClear( GL_COLOR_BUFFER_BIT );
    vao->bind();
    sprogram->bind();
    sprogram->setAttributeBuffer(0,GL_FLOAT,0,2);
    sprogram->enableAttributeArray(0);
    //sprogram->setUniformValue("polygonColor",1.0f, 1.0f, 1.0f);
    oGLFunct->glDrawArrays(GL_TRIANGLES, 0, 6);
    sprogram->release();
    vao->release();
    //qDebug() << deltaT ;

}

void QmainGLWidget::resize(int width, int height)
{
    pixelSize = QSize( width/2, height/2);
}
