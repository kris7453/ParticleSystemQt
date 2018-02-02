#ifndef QDRAWABLELISTWIDGET_H
#define QDRAWABLELISTWIDGET_H

#include <QObject>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include "pdrawitemscontroller.h"
#include "pdrawableitem.h"
#include "resourceitemwidget.h"

namespace PSystemAPI
{
    class pDrawItemsController;
}
class resourceItemWidget;

struct _icon
{
    QIcon   arrowUp,
            arrowDown,
            hidden,
            visible,
            close;
    _icon()
    {
        arrowUp = QIcon(QString(":/icons/upArrow.png"));
        arrowDown = QIcon(QString(":/icons/downArrow.png"));
        hidden = QIcon(QString(":/icons/unvisible.png"));
        visible = QIcon(QString(":/icons/visible.png"));
        close = QIcon(QString(":/icons/cross.png"));
    }
};

class QdrawableListWidget : public QObject
{
    Q_OBJECT
    public:
        QdrawableListWidget( QListWidget *list);
        ~QdrawableListWidget();

        void setController(PSystemAPI::pDrawItemsController *value);
        void setActiveWidgets( QPushButton *activeLayerUp, QPushButton *activeLayerDown, QPushButton *activeVisibility, QPushButton *activeClose, QLabel *activeName);

        void setActiveItemVisibilityIcon(bool isVisible);
        void setActiveItemDesc(PSystemAPI::pDrawableItem *item);
        void refreshActiveItemButtons();

        void addListItem(PSystemAPI::pDrawableItem *item, QString name, QString *resourceImagePath);
        void refreshListItemsButtons();

        void setActiveItem(PSystemAPI::pDrawableItem *value);
        PSystemAPI::pDrawableItem *getActiveItem();

        resourceItemWidget *getWidgetFromPosition(int position);

        static _icon *icon;

        PSystemAPI::pDrawItemsController *getController();

signals:
        void changeVisibility(resourceItemWidget *item = nullptr);
        void close(resourceItemWidget *item = nullptr);
        void layerUp(resourceItemWidget *item = nullptr, int layerDirectio = -1 );
        void layerDown(resourceItemWidget *item = nullptr, int layerDirection = 1 );


    public slots:

        void setActiveItemFromList(QListWidgetItem *item);
        void changeItemVisibility(resourceItemWidget *item = nullptr);
        void closeItem(resourceItemWidget *item = nullptr);
        void changeItemLayer(resourceItemWidget *item = nullptr, int layerDirection = -1);
        void setActive(PSystemAPI::pDrawableItem *active, int activeRow);

    private:
        QPushButton *activeLayerUp;
        QPushButton *activeLayerDown;
        QPushButton *activeVisibility;
        QPushButton *activeClose;
        QLabel      *activeName;

        PSystemAPI::pDrawableItem *activeItem;
        int activeItemRow;

        QListWidget *list;
        PSystemAPI::pDrawItemsController *controller;
};

#endif // QDRAWABLELISTWIDGET_H
