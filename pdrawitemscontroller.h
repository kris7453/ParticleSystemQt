#ifndef PDRAWITEMSCONTROLLER_H
#define PDRAWITEMSCONTROLLER_H

#include <QObject>
#include "qmainglwidget.h"
#include "resourceitemwidget.h"

#include "pdrawableitem.h"
#include "pparticlesystem.h"
#include "qdrawablelistwidget.h"

class QmainGLWidget;
class QdrawableListWidget;
class resourceItemWidget;

namespace PSystemAPI
{
    class pDrawItemsController : public QObject
    {
        Q_OBJECT
        public:
            pDrawItemsController( QdrawableListWidget *drawableListWidget);
            ~pDrawItemsController();

            void drawItems( float deltaT);
            void saveLastPosition();
            void translatePosition( QVector2D position);

            void addDefaultParticleSystem( QString name = QString("Default"), QString *resourceImagePath = nullptr );
            void addParticleSystem(pParticleSystem *pSystem = nullptr);
            void addParticleSystem(QString *resourceImagePath, QString name);

            int deleteItem( pDrawableItem *item);

            void deleteActive();

            pDrawableItem *getActiveItem();
            void setActiveItem(pDrawableItem *value);
            void setFirstItemAsActive();

            int getItemsCount();
            int getItemPosition( pDrawableItem *item);

            int moveLayer( pDrawableItem *item, short direction);

        signals:
            void activeChanged(pDrawableItem *active, int activeRow);

        private:
            pDrawableItem *activeItem;

            QList<pDrawableItem*> drawableItemsList;
            QList<pParticleSystem*> pSystemsList;

            QdrawableListWidget *drawableListWidget;
    };
}


#endif // PDRAWITEMSCONTROLLER_H
