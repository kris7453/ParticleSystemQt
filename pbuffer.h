#ifndef PBUFFER_H
#define PBUFFER_H

#include <cstring>
#include <GL/gl.h>

#include <QVector2D>
#include <QVector4D>
#include <QDebug>

#define STRIDE_SIZE 8 // x, y, r, g, b, a, size, spin

using namespace std;

namespace PSystemAPI
{
    struct pParticleValues
    {
        QVector2D velocity;
        GLfloat growthPerSec;
        GLfloat rotatePerSec;
        QVector4D colorIncreasePerSec;

        pParticleValues(){}
        pParticleValues( QVector2D velocity, GLfloat growthPerSec, GLfloat rotatePerSec, QVector4D colorIncreasePerSec) :
            velocity(velocity), growthPerSec(growthPerSec), rotatePerSec(rotatePerSec), colorIncreasePerSec(colorIncreasePerSec){}
    };

    struct pParticle
    {
        float life;
        QVector2D position;
        GLfloat size;
        GLfloat spin;
        QVector4D color;

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
            pParticle *buffer;
            int size;
            int aliveParticles;
    };
}

#endif // PBUFFER_H
