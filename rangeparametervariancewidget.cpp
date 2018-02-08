#include "rangeparametervariancewidget.h"

rangeParameterVarianceWidget::rangeParameterVarianceWidget(QString name,
                                                           QString firstParameterName, int firstMin, int firstMax,
                                                           QString secondParameterName, int secondMin, int secondMax,
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

    connect(firstParameter, &rangeParameterWidget::valueChanged, this, &rangeParameterVarianceWidget::firstValueChanged);
    connect(secondParameter, &rangeParameterWidget::valueChanged, this, &rangeParameterVarianceWidget::secondValueChanged);
}

void rangeParameterVarianceWidget::setFirstValue(int value)
{
    firstParameter->setValue(value);
}

void rangeParameterVarianceWidget::setSecondValue(int value)
{
    secondParameter->setValue(value);
}
