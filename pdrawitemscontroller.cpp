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

    void pDrawItemsController::setParametersController(QparametersController *controller)
    {
        this->parametersController = controller;
        controller->setDrawableController(this);
    }

    void pDrawItemsController::drawItems(float deltaT)
    {

        for (QList<pParticleSystem*>::reverse_iterator i = pSystemsList.rbegin();
             i != pSystemsList.rend(); i++)
        {
            if ((*i)->isVisible())
                (*i)->updateParticles(deltaT);
        }


        for (QList<pDrawableItem*>::reverse_iterator i = drawableItemsList.rbegin();
             i != drawableItemsList.rend(); i++)
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

    pParticleSystem *pDrawItemsController::addDefaultParticleSystem( QString name, QString *resourceImagePath )
    {
        pParticleSystem *pSystem = new PSystemAPI::pParticleSystem(QVector2D(0,0), resourceImagePath, name);

        drawableItemsList.push_front(pSystem);
        pSystemsList.push_front(pSystem);
        setActiveItem( pSystem );

        drawableListWidget->addListItem( pSystem, name, resourceImagePath);
        return pSystem;
    }

    void pDrawItemsController::addParticleSystemFromFile(QString filePath, QString name)
    {qDebug() << "add from file" << filePath;
        pParticleSystem *pSystem = addDefaultParticleSystem(name, nullptr);
        pSystem->loadFromFile(filePath);
        setActiveItem( pSystem );
    }

    void pDrawItemsController::addParticleSystem(QString *resourceImagePath, QString name)
    {
        addDefaultParticleSystem(name, resourceImagePath);
    }

    void pDrawItemsController::saveParticleSystemToFile(QString filePath)
    {
        if (activeSystem != nullptr)
            activeSystem->saveToFile(filePath);
    }

    void pDrawItemsController::setActiveItemTexture(QString texturePath)
    {
        if ( activeItem != nullptr )
        {
            activeItem->setTexture(&texturePath);
            drawableListWidget->changeActiveItemIcon(&texturePath);
        }
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
        if( activeItem != nullptr && activeItem->isParticleSystem() )
        {
            activeSystem = reinterpret_cast<pParticleSystem*>(activeItem);
            parametersController->changeValues(activeSystem);
        }
        else
        {
            activeSystem = nullptr;
            parametersController->changeValues(activeItem);
        }

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

    void pDrawItemsController::resetSystem()
    {
        if ( activeSystem != nullptr )
            activeSystem->restart();
    }

    void pDrawItemsController::resetSystemPosition()
    {
        if ( activeSystem != nullptr )
            activeSystem->setPosition(QVector2D(0.0f, 0.0f));
    }

    void pDrawItemsController::setDurationTime(float time)
    {
        if ( activeSystem != nullptr )
            activeSystem->setDurationTime(time);
    }

    void pDrawItemsController::setSpawnRate(int rate)
    {
        if ( activeSystem != nullptr )
            activeSystem->setSpawnRate(rate);
    }

    void pDrawItemsController::setMaxParticles(int max)
    {
        if ( activeSystem != nullptr )
            activeSystem->setMaxParticles(max);
    }

    void pDrawItemsController::setAngle(int angle)
    {
        if ( activeSystem != nullptr )
            activeSystem->setAngle(angle);
        else
            activeItem->setAngle(angle);
    }

    void pDrawItemsController::setAngleVariance(int variance)
    {
        if ( activeSystem != nullptr )
            activeSystem->setAngleVariance(variance);
    }

    void pDrawItemsController::setMode(int mode)
    {
        if ( activeSystem != nullptr )
            activeSystem->setSystemMode(static_cast<PSystemAPI::pSystemMode>(mode));
    }

    void pDrawItemsController::setParticleLife(float life)
    {
        if ( activeSystem != nullptr )
            activeSystem->setParticlesLife(life);
    }

    void pDrawItemsController::setParticleLifeVariance(float variance)
    {
        if ( activeSystem != nullptr )
            activeSystem->setLifeVariance(variance);
    }

    void pDrawItemsController::setPositionVarianceX(int variance)
    {
        if ( activeSystem != nullptr )
            activeSystem->setPositionVarianceX(variance);
    }

    void pDrawItemsController::setPositionVarianceY(int variance)
    {
        if ( activeSystem != nullptr )
            activeSystem->setPositionVarianceY(variance);
    }

    void pDrawItemsController::setSpeed(int speed)
    {
        if ( activeSystem != nullptr )
            activeSystem->setSpeed(speed);
    }

    void pDrawItemsController::setSpeedVariance(int variance)
    {
        if ( activeSystem != nullptr )
            activeSystem->setSpeedVariance(variance);
    }

    void pDrawItemsController::setGravityX(int x)
    {
        if ( activeSystem != nullptr )
            activeSystem->setGravityX(x);
    }

    void pDrawItemsController::setGravityXVariance(int variance)
    {
        if ( activeSystem != nullptr )
            activeSystem->setGravityXVariance(variance);
    }

    void pDrawItemsController::setGravityY(int y)
    {
        if ( activeSystem != nullptr )
            activeSystem->setGravityY(y);
    }

    void pDrawItemsController::setGravityYVariance(int variance)
    {
        if ( activeSystem != nullptr )
            activeSystem->setGravityYVariance(variance);
    }

    void pDrawItemsController::setRadialAccValue(int acceleration)
    {
        if ( activeSystem != nullptr )
            activeSystem->setRadialAccValue(acceleration);
    }

    void pDrawItemsController::setRadialAccVariance(int variance)
    {
        if ( activeSystem != nullptr )
            activeSystem->setRadialAccVariance(variance);
    }

    void pDrawItemsController::setTangentialAccValue(int acceleration)
    {
        if ( activeSystem != nullptr )
            activeSystem->setTangentialAccValue(acceleration);
    }

    void pDrawItemsController::setTangentialAccVariance(int variance)
    {
        if ( activeSystem != nullptr )
            activeSystem->setTangentialAccVariance(variance);
    }

    void pDrawItemsController::setStartRadius(int radius)
    {
        if ( activeSystem != nullptr )
            activeSystem->setStartRadius(radius);
    }

    void pDrawItemsController::setStartRadiusVariance(int variance)
    {
        if ( activeSystem != nullptr )
            activeSystem->setStartRadiusVariance(variance);
    }

    void pDrawItemsController::setEndRadius(int radius)
    {
        if ( activeSystem != nullptr )
            activeSystem->setEndRadius(radius);
    }

    void pDrawItemsController::setEndRadiusVariance(int variance)
    {
        if ( activeSystem != nullptr )
            activeSystem->setEndRadiusVariance(variance);
    }

    void pDrawItemsController::setRotatePerSec(int rotate)
    {
        if ( activeSystem != nullptr )
            activeSystem->setRotatePerSec(rotate);
    }

    void pDrawItemsController::setRotatePerSecVariance(int variance)
    {
        if ( activeSystem != nullptr )
            activeSystem->setRotatePerSecVariance(variance);
    }

    void pDrawItemsController::setParticleStartColor(QColor color)
    {
        if ( activeSystem != nullptr )
            activeSystem->setParticleStartColor(color);
    }

    void pDrawItemsController::setParticleStartColorAlpha(int alpha)
    {
        if ( activeSystem != nullptr )
            activeSystem->setParticleStartColorAlpha(alpha);
    }

    void pDrawItemsController::setParticleStartColorVariance(QColor color)
    {
        if ( activeSystem != nullptr )
            activeSystem->setParticleStartColorVariance(color);
    }

    void pDrawItemsController::setParticleStartColorVarianceAlpha(int alpha)
    {
        if ( activeSystem != nullptr )
            activeSystem->setParticleStartColorVarianceAlpha(alpha);
    }

    void pDrawItemsController::setParticleEndColor(QColor color)
    {
        if ( activeSystem != nullptr )
            activeSystem->setParticleEndColor(color);
    }

    void pDrawItemsController::setParticleEndColorAlpha(int alpha)
    {
        if ( activeSystem != nullptr )
            activeSystem->setParticleEndColorAlpha(alpha);
    }

    void pDrawItemsController::setParticleEndColorVariance(QColor color)
    {
        if ( activeSystem != nullptr )
            activeSystem->setParticleEndColorVariance(color);
    }

    void pDrawItemsController::setParticleEndColorVarianceAlpha(int alpha)
    {
        if ( activeSystem != nullptr )
            activeSystem->setParticleEndColorVarianceAlpha(alpha);
    }

    void pDrawItemsController::setParticleStartSpin( short spin)
    {
        if ( activeSystem != nullptr )
            activeSystem->setParticleStartSpin(spin);
    }

    void pDrawItemsController::setParticleStartSpinVariance( short variance)
    {
        if ( activeSystem != nullptr )
            activeSystem->setParticleStartSpinVariance(variance);
    }

    void pDrawItemsController::setParticleEndSpin( short spin)
    {
        if ( activeSystem != nullptr )
            activeSystem->setParticleEndSpin(spin);
    }

    void pDrawItemsController::setParticleEndSpinVariance( short variance)
    {
        if ( activeSystem != nullptr )
            activeSystem->setParticleEndSpinVariance(variance);
    }

    void pDrawItemsController::setParticleStartSize( short size)
    {
        if ( activeSystem != nullptr )
            activeSystem->setParticleStartSize(size);
    }

    void pDrawItemsController::setParticleStartSizeVariance( short variance)
    {
        if ( activeSystem != nullptr )
            activeSystem->setParticleStartSizeVariance(variance);
    }

    void pDrawItemsController::setParticleEndSize( short size)
    {
        if ( activeSystem != nullptr )
            activeSystem->setParticleEndSize(size);
    }

    void pDrawItemsController::setParticleEndSizeVariance( short variance)
    {
        if ( activeSystem != nullptr )
            activeSystem->setParticleEndSizeVariance(variance);
    }

    void pDrawItemsController::setSrcBlendingFactor(int factor)
    {
        if ( activeSystem != nullptr )
            activeSystem->setSrcBlendingFactor(factor);
    }

    void pDrawItemsController::setDstBlendingFactor(int factor)
    {
        if ( activeSystem != nullptr )
            activeSystem->setDstBlendingFactor(factor);
    }

    QColor pDrawItemsController::getParticleStartColor()
    {
        return activeSystem->getParticleStartColor();
    }

    QColor pDrawItemsController::getParticleStartColorVariance()
    {
        return activeSystem->getParticleStartColorVariance();
    }

    QColor pDrawItemsController::getParticleEndColor()
    {
        return activeSystem->getParticleEndColor();
    }

    QColor pDrawItemsController::getParticleEndColorVariance()
    {
        return activeSystem->getParticleEndColorVariance();
    }

    pParticleSystem *pDrawItemsController::getActiveSystem() const
    {
        return activeSystem;
    }
}
