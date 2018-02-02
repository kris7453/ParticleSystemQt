#include "qparameterscontroller.h"

QparametersController::QparametersController(QWidget *drawableWidget, QWidget *gravityWidget, QObject *parent) : QObject(parent)
{
    values = new rangeParameterWidget *[2]
    {
        new rangeParameterWidget("Kąt", -180, 180, range::angel, drawableWidget),
        new rangeParameterWidget("Prędkość", -2000, 2000, range::speed, drawableWidget)
    };

    QLayout *layout;

    layout = drawableWidget->layout();
    layout->addWidget(values[range::angel]);

    layout = gravityWidget->layout();
    layout->addWidget(values[range::speed]);
    layout->addItem(new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    connect(values[range::angel], &rangeParameterWidget::valueChanged, this, &QparametersController::setValue );
    connect(values[range::speed], &rangeParameterWidget::valueChanged, this, &QparametersController::setValue );
}

void QparametersController::setDrawableController(PSystemAPI::pDrawItemsController *controller)
{
    this->controller = controller;
}

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

void QparametersController::setValue(int itemId, int value)
{
    switch( itemId )
    {
        case range::angel: controller->setAngle(value); break;
        case range::speed: controller->setSpeed(value); break;
    }

}
