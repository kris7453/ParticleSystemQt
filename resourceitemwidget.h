#ifndef RESOURCEITEMWIDGET_H
#define RESOURCEITEMWIDGET_H

#include <QWidget>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "qdrawablelistwidget.h"
#include "pdrawableitem.h"

class QdrawableListWidget;
struct _icon;

class resourceItemWidget : public QWidget
{
    Q_OBJECT
    public:
        explicit resourceItemWidget( QWidget *parent = 0);
        resourceItemWidget( PSystemAPI::pDrawableItem *item, QString name, QString *path = nullptr, QWidget *parent = 0);
        resourceItemWidget(resourceItemWidget &item);
        ~resourceItemWidget();

        void changeVisibilityIcon( bool isVisible);
        void setLayersButtonsEnabled( bool upButton, bool downButton);

        static void initializeIcons();
        static void setListWidgetPointer( QdrawableListWidget *listWidget);
        PSystemAPI::pDrawableItem *getItemPointer();

    private:
        void createElements();
        void connectElements();
        void clearElementsConnections();
        void createButtonsConnections();
        void clearButtonsConnections();

        QString name;
        QString *path;

        QHBoxLayout *widgetLayout;
        QVBoxLayout *layerButtonsLayout;

        QWidget     *layerButtonsWidget;

        QPushButton *visibilityBtn,
                    *closeBtn,
                    *layerUpBtn,
                    *layerDownBtn;

        QLabel      *iconImage,
                    *resourceName;

        PSystemAPI::pDrawableItem *itemPointer;
        static QdrawableListWidget *listWidgetPointer;
        static _icon *icon;

        QMetaObject::Connection visibilityConnection;
        QMetaObject::Connection closeConnection;
        QMetaObject::Connection layerUpConnection;
        QMetaObject::Connection layerDownConnection;
};

#endif // RESOURCEITEMWIDGET_H
