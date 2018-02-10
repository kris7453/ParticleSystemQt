#ifndef QPARAMETERSCONTROLLER_H
#define QPARAMETERSCONTROLLER_H

#include <QObject>
#include <QWidget>
#include <QToolBox>
#include <QSpacerItem>
#include "rangeparameterwidget.h"
#include "rangeparametervariancewidget.h"
#include "pdrawitemscontroller.h"

namespace PSystemAPI
{
    class pDrawItemsController;
    class pDrawableItem;
    class pParticleSystem;
}

class QparametersController : public QObject
{
    Q_OBJECT
    public:
        explicit QparametersController( QToolBox *emiterMode, QWidget *drawableWidget,
                                        QWidget *gravityWidget, QWidget *radialWidget, QWidget *outlookWidget, QObject *parent = 0);

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
            endSpin
        };

        QToolBox *emiterMode;
        PSystemAPI::pDrawItemsController *controller;
        rangeParameterVarianceWidget **values;
};

#endif // QPARAMETERSCONTROLLER_H
