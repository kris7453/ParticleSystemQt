#ifndef QMAINGLWIDGET_H
#define QMAINGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <GL/gl.h>

#include <QOpenGLShaderProgram>
#include <QOpenGLShader>

#include <QMouseEvent>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QOpenGLTexture>

#include "pparticlesystem.h"

    using namespace std;

class QmainGLWidget : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT

    public:

        explicit QmainGLWidget(QWidget *parent = 0);
        ~QmainGLWidget();

    signals:

    public slots:

        void initializeGL();
        void paintGL();
        void resizeGL(int w, int h);

   private:

        QPoint mouseClickPosition;
        PSystemAPI::pDrawableItem *activeItem;

        QTimer *t;
        QTime *updateTime;
        int lastUpdate;
        QOpenGLFunctions_3_3_Core *oGLFunct;

        QVector2D pixelSize;

        float   pixelWidth;
        float   pixelHeight;

        PSystemAPI::pParticleSystem *pSystem1;
        //PSystemAPI::pParticleSystem *pSystem2;

        QOpenGLTexture *texture;
        QOpenGLShaderProgram *sprogram;

        void drawParticles( float deltaT);

        void mousePressEvent(QMouseEvent *e);
        void mouseMoveEvent(QMouseEvent *e);
        //void mouseReleaseEvent(QMouseEvent *e);

};

#endif // QMAINGLWIDGET_H
