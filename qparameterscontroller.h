#ifndef QPARAMETERSCONTROLLER_H
#define QPARAMETERSCONTROLLER_H

#include <QObject>
#include <QWidget>
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
        explicit QparametersController(QWidget *drawableWidget, QWidget *gravityWidget, QWidget *radialWidget,QObject *parent = 0);

        void setDrawableController(PSystemAPI::pDrawItemsController *controller);
        void changeValues(PSystemAPI::pDrawableItem *item);
        void changeValues(PSystemAPI::pParticleSystem *system);

    signals:

    private slots:
        void setMainValue(int itemId, int value);
        void setVarianceValue(int itemId, int value);

    private:
        enum range
        {
            angle,
            speed,
            gravityX,
            gravityY,
            radialAccValue,
            tangentialAccValue,

            startRadius,
            endRadius,
            rotatePerSec
        };

        PSystemAPI::pDrawItemsController *controller;
        rangeParameterVarianceWidget **values;
};

#endif // QPARAMETERSCONTROLLER_H
