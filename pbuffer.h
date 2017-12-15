#ifndef PBUFFER_H
#define PBUFFER_H

#include <cstring>
#include <GL/gl.h>

using namespace std;

namespace PSystemAPI
{
    struct pPosition
    {
        GLfloat x, y;
        pPosition(GLfloat x = 0.0f, GLfloat y = 0.0f) : x(x), y(y){}
    };

    struct pParticle
    {
        float life;
        pPosition position;
        GLfloat size;

        pParticle(){}
        pParticle(float life, pPosition pos, GLfloat size) : life(life), position(pos), size(size){}
        void setValues(float life, pPosition pos, GLfloat size){this->life = life; this->position = pos; this->size = size;}
    };

    class pBuffer
    {
        public:
            pBuffer( int size = 128);
            ~pBuffer();

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
