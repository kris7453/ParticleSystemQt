#ifndef QPARAMETERSCONTROLLER_H
#define QPARAMETERSCONTROLLER_H

#include <QObject>
#include <QWidget>
#include <QToolBox>
#include <QComboBox>
#include <QSpacerItem>
#include <QColorDialog>
#include <QFileDialog>

#include "rangeparameterwidget.h"
#include "rangeparametervariancewidget.h"
#include "pdrawitemscontroller.h"
#include "qmainglwidget.h"

namespace PSystemAPI
{
    class pDrawItemsController;
    class pDrawableItem;
    class pParticleSystem;
}

class QmainGLWidget;

class QparametersController : public QObject
{
    Q_OBJECT
    public:
        explicit QparametersController( QToolBox *emiterMode, QWidget *drawableWidget,
                                        QWidget *gravityWidget, QWidget *radialWidget, QWidget *outlookWidget,
                                        QLabel *texturePath, QPushButton *textureButton, QPushButton *addSystemButton,
                                        QmainGLWidget *mainGLWidget, QObject *parent = 0);

        void setDrawableController(PSystemAPI::pDrawItemsController *controller);
        void changeValues(PSystemAPI::pDrawableItem *item);
        void changeValues(PSystemAPI::pParticleSystem *system);

    signals:

    private slots:
        void setMainValue(int itemId, double value);
        void setVarianceValue(int itemId, double value);

    private:
        enum range
        {
            angle,
            particleLife,

            position,
            speed,
            gravityX,
            gravityY,
            radialAccValue,
            tangentialAccValue,

            startRadius,
            endRadius,
            rotatePerSec,

            startSize,
            endSize,
            startSpin,
            endSpin,

            startColor,
            endColor,

            startColorAlpha,
            endColorAlpha
        };

        QmainGLWidget *glWidget;
        QToolBox *emiterMode;
        PSystemAPI::pDrawItemsController *controller;

        QLabel *texturePath;
        QPushButton *resetPositionButton;

        rangeParameterWidget **systemProperties;
        rangeParameterVarianceWidget **values;
        std::pair<QPushButton*,rangeParameterWidget*> **colorWidget;
        std::pair<QLabel*,QComboBox*> **blendingWidget;

        void colorButtonSetColor(std::pair<QPushButton*,rangeParameterWidget*> *pair, QString text, QColor color);

};

#endif // QPARAMETERSCONTROLLER_H
