#ifndef QMAINGLWIDGET_H
#define QMAINGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <GL/gl.h>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>

#include <QTimer>
#include <QTime>
#include <QDebug>
#include <vector>
#include <QOpenGLTexture>
#include "psystem.h"

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
        void resize( int width, int height);

   private:

        QTimer *t;
        QTime *updateTime;
        int lastUpdate;
        QOpenGLFunctions_3_3_Core *oGLFunct;
        QSize pixelSize;
        PSystem *pSystem;
        float *box;
        QOpenGLVertexArrayObject *vao;
        QOpenGLBuffer *vbo;
        QOpenGLShaderProgram *sprogram;
};

#endif // QMAINGLWIDGET_H
