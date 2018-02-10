#ifndef RANGEPARAMETERVARIANCEWIDGET_H
#define RANGEPARAMETERVARIANCEWIDGET_H

#include <QWidget>
#include "rangeparameterwidget.h"

class rangeParameterVarianceWidget : public QWidget
{
    Q_OBJECT
    public:
        explicit rangeParameterVarianceWidget(QString name,
                                              QString firstParameterName, double firstMin, double firstMax,
                                              QString secondParameterName, double secondMin, double secondMax,
                                              int id, QWidget *parent = 0);
        void setFirstValue(double value);
        void setSecondValue(double value);

    signals:
        void firstValueChanged(int itemId, double value);
        void secondValueChanged(int itemId, double value);

    public slots:

    private:
        int itemId;

        QVBoxLayout *verticalLayout;
        QLabel      *nameLabel;

        rangeParameterWidget *firstParameter,
                             *secondParameter;

};

#endif // RANGEPARAMETERVARIANCEWIDGET_H
