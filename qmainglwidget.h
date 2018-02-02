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

#include "mainwindow.h"
#include "pdrawitemscontroller.h"
#include "qparameterscontroller.h"

using namespace std;

namespace PSystemAPI
{
    class pDrawableItem;
    class pParticleSystem;
    class pDrawItemsController;
}

class QmainGLWidget : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT

    public:

        explicit QmainGLWidget(QWidget *parent = 0);
        ~QmainGLWidget();

        void addParticleSystem(PSystemAPI::pParticleSystem *pSystem = nullptr);
        void addParticleSystem(QString *resourceImagePath, QString name);

    signals:

    public slots:

        void initializeGL();
        void paintGL();
        void resizeGL(int w, int h);

   private:

        QPoint mouseClickPosition;

        QTimer *t;
        QTime *updateTime;
        int lastUpdate;
        QOpenGLFunctions_3_3_Core *oGLFunct;

        QVector2D pixelSize;
        QOpenGLShaderProgram *sprogram;
        PSystemAPI::pDrawItemsController *itemsController;

        void drawParticles( float deltaT);

        void mousePressEvent(QMouseEvent *e);
        void mouseMoveEvent(QMouseEvent *e);
        //void mouseReleaseEvent(QMouseEvent *e);

};

#endif // QMAINGLWIDGET_H
