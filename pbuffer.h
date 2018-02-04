#ifndef PBUFFER_H
#define PBUFFER_H

#include <cstring>
#include <GL/gl.h>

#include <QVector2D>
#include <QVector4D>
#include <QDebug>
#include <math.h>

#define STRIDE_SIZE 8 // x, y, r, g, b, a, size, spin

using namespace std;

namespace PSystemAPI
{
    struct pParticleValues
    {
        short       radialAcceleration,
                    tangentialAcceleration;
        QVector2D   velocity,
                    gravity,
                    tangentialVelocity,
                    centerPosition;
        GLfloat     growthPerSec,
                    rotatePerSec;
        QVector4D   colorIncreasePerSec;

        pParticleValues(){tangentialVelocity=QVector2D(0,0);}
        pParticleValues( QVector2D velocity,  QVector2D centerPosition, QVector2D gravity, short radialAcceleration, short tangentialAcceleration,
                         GLfloat growthPerSec, GLfloat rotatePerSec, QVector4D colorIncreasePerSec) :
            velocity(velocity), centerPosition(centerPosition), radialAcceleration(radialAcceleration), gravity(gravity),
            tangentialAcceleration(tangentialAcceleration), growthPerSec(growthPerSec), rotatePerSec(rotatePerSec), colorIncreasePerSec(colorIncreasePerSec){}
    };

    struct pParticle
    {
        float           life;
        QVector2D       position;
        GLfloat         size,
                        spin;
        QVector4D       color;

        pParticleValues values;

        pParticle(){}
        pParticle(float life, QVector2D pos, GLfloat size, GLfloat spin, QVector4D color) :
            life(life), position(pos), size(size), spin(spin), color(color){}
        //void setValues(float life, pPosition pos, GLfloat size){this->life = life; this->position = pos; this->size = size;}
    };

    class pBuffer
    {
        public:
            pBuffer( int size = 128);
            ~pBuffer();

            void addParticle( pParticle* particle);
            void addParticles( pParticle* particles, int count);
            void updateParticles( float deltaT);
            void loadBuffer(GLfloat *particlesBuffer);

            int getAliveParticlesCount();

        private:
            pParticle   *buffer;
            int         size,
                        aliveParticles;
    };
}

#endif // PBUFFER_H
