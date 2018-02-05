#include "qparameterscontroller.h"

QparametersController::QparametersController(QWidget *drawableWidget, QWidget *gravityWidget, QObject *parent) : QObject(parent)
{
    int controllersCount = 6;
    values = new rangeParameterWidget *[controllersCount]
    {
        new rangeParameterWidget("Kąt", -180, 180, range::angel, drawableWidget),
        new rangeParameterWidget("Prędkość", -2000, 2000, range::speed, drawableWidget),
        new rangeParameterWidget("Grawitacja X", -1000, 1000, range::gravityX, gravityWidget),
        new rangeParameterWidget("Grawitacja Y", -1000, 1000, range::gravityY, gravityWidget),
        new rangeParameterWidget("Przyspieszenie promieniowe", -1000, 1000, range::radialAccValue, gravityWidget),
        new rangeParameterWidget("Przyspieszenie styczne", -1000, 1000, range::tangentialAccValue, gravityWidget)
    };

    QLayout *layout;

    layout = drawableWidget->layout();
    layout->addWidget(values[range::angel]);

    layout = gravityWidget->layout();
    layout->addWidget(values[range::speed]);
    layout->addWidget(values[range::gravityX]);
    layout->addWidget(values[range::gravityY]);
    layout->addWidget(values[range::radialAccValue]);
    layout->addWidget(values[range::tangentialAccValue]);
    layout->addItem(new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    for (int i = 0; i < controllersCount; i++)
        connect(values[i], &rangeParameterWidget::valueChanged, this, &QparametersController::setValue );
}

void QparametersController::setDrawableController(PSystemAPI::pDrawItemsController *controller)
{
    this->controller = controller;
}

// Functions called by pDrawableItemsController class object to set widgets value from active item when changed

void QparametersController::changeValues(PSystemAPI::pDrawableItem *item)
{
    values[range::angel]->setValue(item->getAngle());
    qDebug() << "Set angle to " << item->getAngle();
}

void QparametersController::changeValues(PSystemAPI::pParticleSystem *system)
{
    changeValues(reinterpret_cast<PSystemAPI::pDrawableItem*>(system));

    values[range::speed]->setValue(system->getSpeed());
    qDebug() << "Set speed to " << system->getSpeed();
}

// Redirection from widgets to parameters setters

void QparametersController::setValue(int itemId, int value)
{
    switch( itemId )
    {
        case range::angel: controller->setAngle(value); break;
        case range::speed: controller->setSpeed(value); break;
        case range::gravityX: controller->setGravityX(value); break;
        case range::gravityY: controller->setGravityY(value); break;
        case range::radialAccValue: controller->setRadialAccValue(value); break;
        case range::tangentialAccValue: controller->setTangentialAccValue(value); break;
    }

}
