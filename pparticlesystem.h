#ifndef PPARTICLESYSTEM_H
#define PPARTICLESYSTEM_H

#include "pdrawableitem.h"
#include "pbuffer.h"
#include <time.h>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

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
        QColor   startColor,
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

            void loadFromFile(QString filePath);
            void saveToFile( QString filePath);
            void spawnParticle();
            void updateParticles( float deltaT);
            void draw();
            void restart();
            bool isFull();

            void setSystemMode( pSystemMode mode);
            void setPosition( QVector2D position);
            void setPositionVariance( QVector2D positionVariance);
            void setPositionVarianceX( int variance);
            void setPositionVarianceY( int variance);

            void setDurationTime( float time);
            void setSpawnRate( int rate);
            void setMaxParticles( int max);

            void setParticlesLife( int life, int lifeVariance = 0);
            void setParticlesLife( float life);
            void setParticlesLife( float life, float lifeVariance);

            void setLifeVariance( int lifeVariance);
            void setLifeVariance( float lifeVariance);

            void setParticleStartColor( QColor color);
            void setParticleStartColorAlpha( int alpha);
            void setParticleStartColorAlpha( double alpha);
            void setParticleStartColorRed( double red);
            void setParticleStartColorGreen( double green);
            void setParticleStartColorBlue( double blue);
            void setParticleStartColorVariance( QColor color);
            void setParticleStartColorVarianceAlpha( int alpha);
            void setParticleStartColorVarianceAlpha( double alpha);
            void setParticleStartColorVarianceRed( double red);
            void setParticleStartColorVarianceGreen( double green);
            void setParticleStartColorVarianceBlue( double blue);
            void setParticleEndColor( QColor color);
            void setParticleEndColorAlpha( int alpha);
            void setParticleEndColorAlpha( double alpha);
            void setParticleEndColorRed( double red);
            void setParticleEndColorGreen( double green);
            void setParticleEndColorBlue( double blue);
            void setParticleEndColorVariance( QColor color);
            void setParticleEndColorVarianceAlpha( int alpha);
            void setParticleEndColorVarianceAlpha( double alpha);
            void setParticleEndColorVarianceRed( double red);
            void setParticleEndColorVarianceGreen( double green);
            void setParticleEndColorVarianceBlue( double blue);

            void setParticlesColor( QColor startColor, QColor startColorVariance);
            void setParticlesColor( QColor startColor, QColor startColorVariance, QColor endColor, QColor endColorVariance);

            void setParticleStartSpin( short spin);
            void setParticleStartSpinVariance( short variance);
            void setParticleEndSpin( short spin);
            void setParticleEndSpinVariance( short variance);

            void setParticlesSpin( short startSpin, short startSpinVariance);
            void setParticlesSpin( short startSpin, short startSpinVariance, short endSpin, short endSpinVariance);

            void setParticleStartSize( short size);
            void setParticleStartSizeVariance( short variance);
            void setParticleEndSize( short size);
            void setParticleEndSizeVariance( short variance);
            
            void setParticlesSize( short startSize, short startSizeVariance);
            void setParticlesSize( short startSize, short startSizeVariance, short endSize, short endSizeVariance);

            void setAngle(short angle);
            void setAngleVariance(short variance);

            void setSpeed(short speed);
            void setSpeedVariance(short variance);

            void setGravityX( int x);
            void setGravityXVariance( int variance);
            void setGravityY( int y);
            void setGravityYVariance( int variance);

            void setRadialAccValue( short acceleration);
            void setRadialAccVariance(short variance);
            void setRadialAccValue( short acceleration, short accelerationVariance);

            void setTangentialAccValue( short acceleration);
            void setTangentialAccVariance(short variance);
            void setTangentialAccValue( short acceleration, short accelerationVariance);

            void setStartRadius(int radius);
            void setStartRadiusVariance(int variance);

            void setEndRadius(int radius);
            void setEndRadiusVariance(int variance);

            void setRotatePerSec(int rotates);
            void setRotatePerSecVariance(int variance);

            void setSrcBlendingFactor(int factor);
            void setDstBlendingFactor(int factor);

            QColor getParticleStartColor();
            QColor getParticleStartColorVariance();
            QColor getParticleEndColor();
            QColor getParticleEndColorVariance();

            int getParticleStartSpin();
            int getParticleStartSpinVariance();
            int getParticleEndSpin();
            int getParticleEndSpinVariance();

            int getParticleStartSize();
            int getParticleStartSizeVariance();
            int getParticleEndSize();
            int getParticleEndSizeVariance();

            int getSystemMode();

            float getParticleLife();
            float getParticleLifeVariance();

            float getDurationTime();
            float getSpawnRate();
            int getMaxParticles();

            int getPositionVarianceX();
            int getPositionVarianceY();
            int getAngleVariance();
            int getSpeed();
            int getSpeedVariance();

            int getGravityX();
            int getGravityXVariance();
            int getGravityY();
            int getGravityYVariance();

            int getRadialAccValue();
            int getRadialAccVariance();

            int getTangentialAccValue();
            int getTangentialAccVariance();

            int getStartRadius();
            int getStartRadiusVariance();

            int getEndRadius();
            int getEndRadiusVariance();

            int getRotatePerSec();
            int getRotatePerSecVariance();

            int getSrcBlendingFactor();
            int getDstBlendingFactor();

            bool operator==(const pParticleSystem& item);

        private:
            pBuffer     *particles;

            QVector2D   positionVariance;
            QVector2D   velocityDirection;
            pSystemMode mode;

            short angleVariance;
            pParticleProperties pProperties;
            pParticleProperties pVariances;

            float   durationTime;
            int     maxParticles;
            float   timeElapsed;
            float   simulatingTime;
            float   spawnRate; // emission rate
            float   spawnTimeSpan;
            QTimer  *spawnTimer;

            uint srcBlendingFactor;
            uint dstBlendingFactor;
    };
}

#endif // PPARTICLESYSTEM_H
