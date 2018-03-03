#ifndef RANGEPARAMETERWIDGET_H
#define RANGEPARAMETERWIDGET_H

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>

class rangeParameterWidget : public QWidget
{
    Q_OBJECT
    public:
        explicit rangeParameterWidget(QString name, double min, double max, QWidget *parent = 0);
        rangeParameterWidget(QString name, double min, double max, int id, QWidget *parent = 0);
        void setValue(double value);
        double getValue();

    signals:
        void valueChanged(double value);
        void valueChanged(int itemId, double value);
        void valueConfirmed(double value);

    public slots:

    private:
        int rangeId;

        QVBoxLayout *mainLayout;
        QLabel      *nameLabel;

        QWidget     *horizontalWidget;
        QHBoxLayout *horizontalLayout;

        QSlider     *slider;
        QDoubleSpinBox    *numberBox;
};

#endif // RANGEPARAMETERWIDGET_H
