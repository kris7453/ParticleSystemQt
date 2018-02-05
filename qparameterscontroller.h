#ifndef QPARAMETERSCONTROLLER_H
#define QPARAMETERSCONTROLLER_H

#include <QObject>
#include <QWidget>
#include <QSpacerItem>
#include "rangeparameterwidget.h"
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
        explicit QparametersController(QWidget *drawableWidget, QWidget *gravityWidget, QObject *parent = 0);

        void setDrawableController(PSystemAPI::pDrawItemsController *controller);
        void changeValues(PSystemAPI::pDrawableItem *item);
        void changeValues(PSystemAPI::pParticleSystem *system);

    signals:

    private slots:
        void setValue(int itemId, int value);

    private:
        enum range
        {
            angel,
            speed,
            gravityX,
            gravityY,
            radialAccValue,
            tangentialAccValue
        };

        PSystemAPI::pDrawItemsController *controller;
        rangeParameterWidget **values;
};

#endif // QPARAMETERSCONTROLLER_H
