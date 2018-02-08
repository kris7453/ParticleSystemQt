#include "qparameterscontroller.h"

QparametersController::QparametersController(QWidget *drawableWidget, QWidget *gravityWidget, QWidget *radialWidget,QObject *parent) : QObject(parent)
{
    int controllersCount = 9;
    values = new rangeParameterVarianceWidget *[controllersCount]
    {
        new rangeParameterVarianceWidget("Kąt + rozbierzność", "", -180, 180, "", 0, 180, range::angle, drawableWidget),
        new rangeParameterVarianceWidget("Prędkość + rozbierzność", "", -2000, 2000, "", 0, 1000, range::speed, drawableWidget),
        new rangeParameterVarianceWidget("Grawitacja X + rozbierzność", "", -1000, 1000, "", 0, 1000, range::gravityX, gravityWidget),
        new rangeParameterVarianceWidget("Grawitacja Y + rozbierzność", "", -1000, 1000, "", 0, 1000, range::gravityY, gravityWidget),
        new rangeParameterVarianceWidget("Przyspieszenie promieniowe + rozbierzność", "", -1000, 1000, "", 0, 1000, range::radialAccValue, gravityWidget),
        new rangeParameterVarianceWidget("Przyspieszenie styczne + rozbierzność", "", -1000, 1000, "", 0, 1000, range::tangentialAccValue, gravityWidget),

        new rangeParameterVarianceWidget("Promień startowy + rozbierzność", "", -1000, 1000, "", 0, 1000, range::startRadius, radialWidget),
        new rangeParameterVarianceWidget("Promień końcowy + rozbierzność", "", -1000, 1000, "", 0, 1000, range::endRadius, radialWidget),
        new rangeParameterVarianceWidget("Obrotów/s (stopnie) + rozbierzność", "", -1000, 1000, "", 0, 1000, range::rotatePerSec, radialWidget)
    };

    QLayout *layout;

    layout = drawableWidget->layout();
    layout->addWidget(values[range::angle]);

    layout = gravityWidget->layout();
    layout->addWidget(values[range::speed]);
    layout->addWidget(values[range::gravityX]);
    layout->addWidget(values[range::gravityY]);
    layout->addWidget(values[range::radialAccValue]);
    layout->addWidget(values[range::tangentialAccValue]);
    layout->addItem(new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    layout = radialWidget->layout();
    layout->addWidget(values[range::startRadius]);
    layout->addWidget(values[range::endRadius]);
    layout->addWidget(values[range::rotatePerSec]);
    layout->addItem(new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    for (int i = 0; i < controllersCount; i++)
    {
        connect(values[i], &rangeParameterVarianceWidget::firstValueChanged, this, &QparametersController::setMainValue);
        connect(values[i], &rangeParameterVarianceWidget::secondValueChanged, this, &QparametersController::setVarianceValue);
    }
}

void QparametersController::setDrawableController(PSystemAPI::pDrawItemsController *controller)
{
    this->controller = controller;
}

// Functions called by pDrawableItemsController class object to set widgets value from active item when changed

void QparametersController::changeValues(PSystemAPI::pDrawableItem *item)
{
    values[range::angle]->setFirstValue(item->getAngle());
    //qDebug() << "Set angle to " << item->getAngle();
}

void QparametersController::changeValues(PSystemAPI::pParticleSystem *system)
{
    changeValues(reinterpret_cast<PSystemAPI::pDrawableItem*>(system));

    values[range::angle]->setSecondValue(system->getAngleVariance());
    values[range::speed]->setFirstValue(system->getSpeed());
    values[range::speed]->setSecondValue(system->getSpeedVariance());
    values[range::gravityX]->setFirstValue(system->getGravityX());
    values[range::gravityX]->setSecondValue(system->getGravityXVariance());
    values[range::gravityY]->setFirstValue(system->getGravityY());
    values[range::gravityY]->setSecondValue(system->getGravityYVariance());
    values[range::radialAccValue]->setFirstValue(system->getRadialAccValue());
    values[range::radialAccValue]->setSecondValue(system->getRadialAccVariance());
    values[range::tangentialAccValue]->setFirstValue(system->getTangentialAccValue());
    values[range::tangentialAccValue]->setSecondValue(system->getTangentialAccVariance());

    values[range::startRadius]->setFirstValue(system->getStartRadius());
    values[range::startRadius]->setSecondValue(system->getStartRadiusVariance());
    values[range::endRadius]->setFirstValue(system->getEndRadius());
    values[range::endRadius]->setSecondValue(system->getEndRadiusVariance());
    values[range::rotatePerSec]->setFirstValue(system->getRotatePerSec());
    values[range::rotatePerSec]->setSecondValue(system->getRotatePerSecVariance());
    //qDebug() << "Set speed to " << system->getSpeed();
}

// Redirection from widgets to parameters setters

void QparametersController::setMainValue(int itemId, int value)
{
    switch( itemId )
    {
        case range::angle: controller->setAngle(value); break;
        case range::speed: controller->setSpeed(value); break;
        case range::gravityX: controller->setGravityX(value); break;
        case range::gravityY: controller->setGravityY(value); break;
        case range::radialAccValue: controller->setRadialAccValue(value); break;
        case range::tangentialAccValue: controller->setTangentialAccValue(value); break;

        case range::startRadius: controller->setStartRadius(value); break;
        case range::endRadius: controller->setEndRadius(value); break;
        case range::rotatePerSec: controller->setRotatePerSec(value); break;
    }

}

void QparametersController::setVarianceValue(int itemId, int value)
{
    switch( itemId )
    {
        case range::angle: controller->setAngleVariance(value); break;
        case range::speed: controller->setSpeedVariance(value); break;
        case range::gravityX: controller->setGravityXVariance(value); break;
        case range::gravityY: controller->setGravityYVariance(value); break;
        case range::radialAccValue: controller->setRadialAccVariance(value); break;
        case range::tangentialAccValue: controller->setTangentialAccVariance(value); break;

        case range::startRadius: controller->setStartRadiusVariance(value); break;
        case range::endRadius: controller->setEndRadiusVariance(value); break;
        case range::rotatePerSec: controller->setRotatePerSecVariance(value); break;
    }
}

