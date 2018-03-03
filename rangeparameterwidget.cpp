#include "rangeparameterwidget.h"

rangeParameterWidget::rangeParameterWidget(QString name, double min, double max, int id, QWidget *parent) : QWidget(parent)
{
    this->rangeId = id;

    if( name.length() > 0 )
    {
        mainLayout = new QVBoxLayout(this);
        mainLayout->setSpacing(0);
        mainLayout->setContentsMargins(0, 0, 0, 0);

        nameLabel = new QLabel( name, this);

        horizontalWidget = new QWidget(this);
        horizontalLayout = new QHBoxLayout(horizontalWidget);
    }
    else
        horizontalLayout = new QHBoxLayout(this);

    horizontalLayout->setSpacing(4);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    slider = new QSlider(this);
    slider->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    slider->setMinimum( min );
    slider->setMaximum( max );
    slider->setValue(0);
    slider->setOrientation(Qt::Horizontal);

    numberBox = new QDoubleSpinBox(this);
    numberBox->setSizePolicy( QSizePolicy::Maximum,QSizePolicy::Preferred);
    numberBox->setMaximumSize(QSize(16777215, 60));
    numberBox->setMinimum( min );
    numberBox->setMaximum( max );
    numberBox->setValue(0);

    horizontalLayout->addWidget(slider);
    horizontalLayout->addWidget(numberBox);

    if( name.length() > 0 )
    {
        mainLayout->addWidget(nameLabel);
        mainLayout->addWidget(horizontalWidget);
    }

    connect( slider, &QSlider::valueChanged, [this](int val){
        numberBox->setValue(val);
        emit valueChanged(val);
        emit valueChanged(rangeId, val);
    });

    connect( slider, &QSlider::sliderReleased, [this](){
        emit valueConfirmed(slider->value());
    });

    connect( numberBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [this](double val){
        slider->setValue(val);
        emit valueChanged(val);
        emit valueChanged(rangeId, val);
        emit valueConfirmed(val);
    });
}

rangeParameterWidget::rangeParameterWidget(QString name, double min, double max, QWidget *parent) :
    rangeParameterWidget( name,  min,  max, 0, parent)
{
}

void rangeParameterWidget::setValue(double value)
{
    numberBox->setValue(value);
    slider->setValue(value);
}

double rangeParameterWidget::getValue()
{
    return slider->value();
}
