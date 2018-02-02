#include "rangeparameterwidget.h"

rangeParameterWidget::rangeParameterWidget(QString name, int min, int max, int id, QWidget *parent) : QWidget(parent)
{
    this->rangeId = id;
//    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
//    setGeometry(0,0,0,160);
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    nameWidget = new QWidget(this);
    nameLayout = new QHBoxLayout(nameWidget);
    nameLayout->setSpacing(0);
    nameLayout->setContentsMargins(0, 0, 0, 0);

    nameLabel = new QLabel(nameWidget);
    nameLabel->setText(name);

    numberBox = new QSpinBox(nameWidget);
    numberBox->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    numberBox->setMaximumSize(QSize(16777215, 60));
    numberBox->setMinimum( min );
    numberBox->setMaximum( max );
    numberBox->setValue(0);

    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(numberBox);

    slider = new QSlider(this);
    slider->setMinimum( min );
    slider->setMaximum( max );
    slider->setValue(0);
    slider->setOrientation(Qt::Horizontal);

    mainLayout->addWidget(nameWidget);
    mainLayout->addWidget(slider);

    connect( slider, &QSlider::valueChanged, [this](int val){
        numberBox->setValue(val);
        emit valueChanged(rangeId, val);
    });
    connect( numberBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int val){
        slider->setValue(val);
        emit valueChanged(rangeId, val);
    });
}

void rangeParameterWidget::setValue(int value)
{
    numberBox->setValue(value);
    slider->setValue(value);
}
