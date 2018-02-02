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
        explicit rangeParameterWidget(QString name, int min, int max, int id, QWidget *parent = 0);
        void setValue(int value);

    signals:
        void valueChanged(int itemId, int value);

    public slots:

    private:
        int rangeId;

        QWidget     *nameWidget;

        QLabel      *nameLabel;
        QSpinBox    *numberBox;
        QSlider     *slider;

        QVBoxLayout *mainLayout;
        QHBoxLayout *nameLayout;
};

#endif // RANGEPARAMETERWIDGET_H
