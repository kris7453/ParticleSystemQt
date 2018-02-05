#ifndef PPARTICLESYSTEM_H
#define PPARTICLESYSTEM_H

#include "pdrawableitem.h"
#include "pbuffer.h"
#include <time.h>

#define STRIDE_SIZE 8 // x, y size, spin, r, g, b, a

//#define DEBUG_LOADED_BUFFER


namespace PSystemAPI
{
    struct pParticleProperties
    {
        int     life; // in miliseconds

        // outlook
        short   startSpin,
                endSpin,
                startSize,
                endSize;
        QVector4D   startColor,
                    endColor;

        //gravity mode
        short   speed,
                radialAccValue,
                tangentialAccValue;
        QVector2D   velocity,
                    gravity;

        //radius mode
        short   startRadius,
                endRadius,
                rotatePerSec;
    };

    class pParticleSystem : public pDrawableItem
    {
        public:
            pParticleSystem( QString *resourceImagePath, QString name = QString("Default"));
            pParticleSystem( QVector2D position, QString *resourceImagePath, QString name = QString("Default"));

            void spawnParticle();
            void updateParticles( float deltaT);
            void draw();

            void setSystemMode( pSystemMode mode);
            void setPosition( QVector2D position, QVector2D positionVariance = QVector2D(0.0f,0.0f));
            void setPositionVariance( QVector2D positionVariance);

            void setParticlesLife( int life, int lifeVariance = 0);
            void setParticlesLife( float life, float lifeVariance = 0.0f);

            void setLifeVariance( int lifeVariance);
            void setLifeVariance( float lifeVariance);

            void setParticlesColor( QVector4D startColor, QVector4D startColorVariance);
            void setParticlesColor( QVector4D startColor,QVector4D startColorVariance, QVector4D endColor, QVector4D endColorVariance);

            void setParticlesSpin( short startSpin, short startSpinVariance);
            void setParticlesSpin( short startSpin, short startSpinVariance, short endSpin, short endSpinVariance);
            
            void setParticlesSize( short startSize, short startSizeVariance);
            void setParticlesSize( short startSize, short startSizeVariance, short endSize, short endSizeVariance);

            void setAngle(short angle);
            void setSpeed(short speed);

            void setGravityX( int x);
            void setGravityY( int y);

            void setRadialAccValue( short acceleration);
            void setRadialAccValue( short acceleration, short accelerationVariance);

            void setTangentialAccValue( short acceleration);
            void setTangentialAccValue( short acceleration, short accelerationVariance);

            int getSpeed();

            bool operator==(const pParticleSystem& item);

        private:
            pBuffer     *particles;
            float       timeElapsed;
            QVector2D   positionVariance;
            QVector2D   velocityDirection;
            pSystemMode mode;

            pParticleProperties pProperties;
            pParticleProperties pVariances;

            int     maxParticles;
            float   spawnRate; // emission rate
            float   spawnTimeSpan;
            QTimer  *spawnTimer;
    };
}

#endif // PPARTICLESYSTEM_H
