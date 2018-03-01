#include "qdrawablelistwidget.h"

_icon *QdrawableListWidget::icon;

QdrawableListWidget::QdrawableListWidget(QListWidget *list) : list(list)
{
    if( icon == nullptr )
        icon = new _icon();

    resourceItemWidget::setListWidgetPointer(this);

    connect( this, &QdrawableListWidget::changeVisibility, this, &QdrawableListWidget::changeItemVisibility);
    connect( this, &QdrawableListWidget::close, this, &QdrawableListWidget::closeItem);
    connect( this, &QdrawableListWidget::layerUp, this, &QdrawableListWidget::changeItemLayer);
    connect( this, &QdrawableListWidget::layerDown, this, &QdrawableListWidget::changeItemLayer);
    connect( this, &QdrawableListWidget::changeName, this, &QdrawableListWidget::changeItemName);
}

QdrawableListWidget::~QdrawableListWidget()
{
}

void QdrawableListWidget::setActiveWidgets(QPushButton *activeLayerUp, QPushButton *activeLayerDown, QPushButton *activeVisibility, QPushButton *activeClose, QLabel *activeName)
{
    this->activeLayerUp = activeLayerUp;
    this->activeLayerDown = activeLayerDown;
    this->activeVisibility = activeVisibility;
    this->activeClose = activeClose;
    this->activeName = activeName;
}

void QdrawableListWidget::addListItem(PSystemAPI::pDrawableItem *item, QString name, QString *resourceImagePath)
{
    QListWidgetItem *listItem = new QListWidgetItem();
    listItem->setSizeHint(QSize(0,24)); // must be to provide item size to listWidget (width, height)
    resourceItemWidget *rwid = new resourceItemWidget( item, name, resourceImagePath, list);

    list->insertItem(0, listItem);
    list->setItemWidget(listItem, rwid);
}

void QdrawableListWidget::setController(PSystemAPI::pDrawItemsController *value)
{
    controller = value;
    connect( controller, &PSystemAPI::pDrawItemsController::activeChanged, this, &QdrawableListWidget::setActive);
}

void QdrawableListWidget::setActiveItem(PSystemAPI::pDrawableItem *value)
{
    activeItem = value;
    setActiveItemDesc( activeItem );
}

void QdrawableListWidget::changeActiveItemIcon(QString *path)
{
    getWidgetFromPosition(activeItemRow)->setIcon(path);
}

PSystemAPI::pDrawableItem *QdrawableListWidget::getActiveItem()
{
    return activeItem;
}

resourceItemWidget *QdrawableListWidget::getWidgetFromPosition(int position)
{
    QListWidgetItem *lwi = list->item(position);
    return reinterpret_cast<resourceItemWidget*>( list->itemWidget(lwi) );
}

void QdrawableListWidget::setActiveItemVisibilityIcon(bool isVisible)
{
    if (isVisible)
        activeVisibility->setIcon(icon->visible);
    else
        activeVisibility->setIcon(icon->hidden);
}

void QdrawableListWidget::setActiveItemDesc(PSystemAPI::pDrawableItem *item)
{
    if (item != nullptr)
    {
        activeName->setText(item->getName());
        setActiveItemVisibilityIcon( item->isVisible() );
        activeVisibility->setEnabled(true);
        activeClose->setEnabled(true);
        activeLayerUp->setEnabled(true);
        activeLayerDown->setEnabled(true);
    }
    else
    {
        activeName->setText(QString(""));
        activeVisibility->setIcon(icon->visible);
        activeVisibility->setEnabled(false);
        activeClose->setEnabled(false);
        activeLayerUp->setEnabled(false);
        activeLayerDown->setEnabled(false);
    }
}

void QdrawableListWidget::setActiveItemFromList(QListWidgetItem *item)
{
    controller->setActiveItem( reinterpret_cast<resourceItemWidget*>(list->itemWidget(item) )->getItemPointer() );
}

void QdrawableListWidget::changeItemVisibility(resourceItemWidget *item)
{
    PSystemAPI::pDrawableItem *itemP = item == nullptr ? activeItem : item->getItemPointer();
    item = item == nullptr ? getWidgetFromPosition(activeItemRow) : item;

    bool isVisible = itemP->changeVisibility();
    item->changeVisibilityIcon(isVisible);

    if (itemP == activeItem)
    {
        qDebug() << "QdrawableListWidget::changeItemVisibility at position " << activeItemRow;
        setActiveItemVisibilityIcon(isVisible);
    }
}

void QdrawableListWidget::closeItem(resourceItemWidget *item)
{
    PSystemAPI::pDrawableItem *itemP = item == nullptr ? activeItem : item->getItemPointer();
    item = item == nullptr ? getWidgetFromPosition(activeItemRow) : item;

    int position = controller->deleteItem(itemP);

    if(position != -1)
        delete list->takeItem(position);

    if( itemP == activeItem )
        controller->setFirstItemAsActive();

}

void QdrawableListWidget::changeItemLayer(resourceItemWidget *item, int layerDirection)
{
    PSystemAPI::pDrawableItem *itemP = item == nullptr ? activeItem : item->getItemPointer();
    item = item == nullptr ? getWidgetFromPosition(activeItemRow) : item;

    int itemPosition = controller->getItemPosition(itemP);
    int result = controller->moveLayer(itemP, layerDirection);

    if ( result != -1 )
    {
        resourceItemWidget *rw = new resourceItemWidget(*item);

        QListWidgetItem *lw = list->takeItem(itemPosition);
        //lw->setSizeHint(QSize(0,24));
        list->insertItem( result, lw);qDebug() << "QdrawableListWidget::changeItemLayer inser item";
        list->setItemWidget(lw, rw);qDebug() << "QdrawableListWidget::changeItemLayer set item widget";

        qDebug() << "QdrawableListWidget::changeItemLayer new position " << result;
        if ( itemP == activeItem)
            activeItemRow = result;
        else
            if( result == activeItemRow )
                activeItemRow += layerDirection * (-1);
        qDebug() << "QdrawableListWidget::changeItemLayer active position " << activeItemRow;
    }
}

void QdrawableListWidget::changeItemName(resourceItemWidget *item, QString name)
{
    PSystemAPI::pDrawableItem *itemP = item == nullptr ? activeItem : item->getItemPointer();
    item = item == nullptr ? getWidgetFromPosition(activeItemRow) : item;

    itemP->setName(name);
    if ( itemP == activeItem)
        activeName->setText(name);
}

void QdrawableListWidget::setActive(PSystemAPI::pDrawableItem *active, int activeRow)
{
    activeItem = active;
    activeItemRow = activeRow;

    setActiveItemDesc(active);
}

PSystemAPI::pDrawItemsController *QdrawableListWidget::getController()
{
    return controller;
}
