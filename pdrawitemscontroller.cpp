#include "pdrawitemscontroller.h"

namespace PSystemAPI
{
    pDrawItemsController::pDrawItemsController( QdrawableListWidget *drawableListWidget)
    {
        this->drawableListWidget = drawableListWidget;
    }

    pDrawItemsController::~pDrawItemsController()
    {
        qDeleteAll(pSystemsList.begin(),pSystemsList.end());
    }

    void pDrawItemsController::drawItems(float deltaT)
    {

        for (QList<pParticleSystem*>::iterator i = pSystemsList.begin();
             i != pSystemsList.end(); i++)
        {
            if ((*i)->isVisible())
                (*i)->updateParticles(deltaT);
        }


        for (QList<pDrawableItem*>::iterator i = drawableItemsList.begin();
             i != drawableItemsList.end(); i++)
        {
            if ((*i)->isVisible())
                (*i)->draw();
        }
    }

    void pDrawItemsController::saveLastPosition()
    {
        if ( activeItem != nullptr && activeItem->isVisible())
            activeItem->saveLastPosition();
    }

    void pDrawItemsController::translatePosition(QVector2D position)
    {
        if ( activeItem != nullptr && activeItem->isVisible())
            activeItem->translatePosition(position);
    }

    void pDrawItemsController::addDefaultParticleSystem( QString name, QString *resourceImagePath )
    {
        pParticleSystem *pSystem = new PSystemAPI::pParticleSystem(QVector2D(0,0), resourceImagePath, name);

        drawableItemsList.push_front(pSystem);
        pSystemsList.push_front(pSystem);
        setActiveItem( pSystem );

        drawableListWidget->addListItem( pSystem, name, resourceImagePath);
    }

    void pDrawItemsController::addParticleSystem(pParticleSystem *pSystem)
    {
        if( pSystem == nullptr)
            addDefaultParticleSystem();
    }

    void pDrawItemsController::addParticleSystem(QString *resourceImagePath, QString name)
    {
        addDefaultParticleSystem(name, resourceImagePath);
    }

    int pDrawItemsController::deleteItem(pDrawableItem *item)
    {
        int itemIndex = drawableItemsList.indexOf(item);

        if( itemIndex != -1 )
        {
            if( pSystemsList.contains(reinterpret_cast<pParticleSystem*>(item)) )
                pSystemsList.removeOne(reinterpret_cast<pParticleSystem*>(item));

            drawableItemsList.removeOne(item);
        }

        return itemIndex;
    }

    void pDrawItemsController::deleteActive()
    {
        deleteItem( activeItem );
        setFirstItemAsActive();
        drawableListWidget->setActiveItemDesc( activeItem );
    }

    pDrawableItem *pDrawItemsController::getActiveItem()
    {
        return activeItem;
    }

    void pDrawItemsController::setActiveItem(pDrawableItem *value)
    {
        activeItem = value;
        emit activeChanged(activeItem, drawableItemsList.indexOf(activeItem));
    }

    void pDrawItemsController::setFirstItemAsActive()
    {
        setActiveItem( (!drawableItemsList.isEmpty()) ? drawableItemsList.first() : nullptr );
    }

    int pDrawItemsController::getItemsCount()
    {
        return drawableItemsList.length();
    }

    int pDrawItemsController::getItemPosition(pDrawableItem *item)
    {
        return drawableItemsList.indexOf(item);
    }

    int pDrawItemsController::moveLayer(pDrawableItem *item, short direction)
    {
        int itemPosition = drawableItemsList.indexOf(item);

        if (direction < 0)
            if ( itemPosition > 0 )
            {
                drawableItemsList.swap(itemPosition, itemPosition - 1);
                return itemPosition -1;
            }
            else
                return -1;

        if ( itemPosition < (drawableItemsList.length() - 1) )
        {
            drawableItemsList.swap(itemPosition, itemPosition + 1);
            return itemPosition +1;
        }
        else
            return -1;
    }
}
