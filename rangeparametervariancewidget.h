#ifndef RANGEPARAMETERVARIANCEWIDGET_H
#define RANGEPARAMETERVARIANCEWIDGET_H

#include <QWidget>
#include "rangeparameterwidget.h"

class rangeParameterVarianceWidget : public QWidget
{
    Q_OBJECT
    public:
        explicit rangeParameterVarianceWidget(QString name,
                                              QString firstParameterName, int firstMin, int firstMax,
                                              QString secondParameterName, int secondMin, int secondMax,
                                              int id, QWidget *parent = 0);
        void setFirstValue(int value);
        void setSecondValue(int value);

    signals:
        void firstValueChanged(int itemId, int value);
        void secondValueChanged(int itemId, int value);

    public slots:

    private:
        int itemId;

        QVBoxLayout *verticalLayout;
        QLabel      *nameLabel;

        rangeParameterWidget *firstParameter,
                             *secondParameter;

};

#endif // RANGEPARAMETERVARIANCEWIDGET_H
