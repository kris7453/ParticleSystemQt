#include "rangeparametervariancewidget.h"

rangeParameterVarianceWidget::rangeParameterVarianceWidget(QString name,
                                                           QString firstParameterName, double firstMin, double firstMax,
                                                           QString secondParameterName, double secondMin, double secondMax,
                                                           int id, QWidget *parent) : QWidget(parent)
{
    itemId = id;
    verticalLayout = new QVBoxLayout(this);

    nameLabel = new QLabel(name, this);
    verticalLayout->addWidget(nameLabel);

    firstParameter = new rangeParameterWidget( firstParameterName, firstMin, firstMax, id, this);
    verticalLayout->addWidget(firstParameter);

    secondParameter = new rangeParameterWidget( secondParameterName, secondMin, secondMax, id, this);
    verticalLayout->addWidget(secondParameter);

    connect(firstParameter, static_cast<void(rangeParameterWidget::*)(int,double)>(&rangeParameterWidget::valueChanged), this, &rangeParameterVarianceWidget::firstValueChanged);
    connect(secondParameter, static_cast<void(rangeParameterWidget::*)(int,double)>(&rangeParameterWidget::valueChanged), this, &rangeParameterVarianceWidget::secondValueChanged);
}

void rangeParameterVarianceWidget::setFirstValue(double value)
{
    firstParameter->setValue(value);
}

void rangeParameterVarianceWidget::setSecondValue(double value)
{
    secondParameter->setValue(value);
}
