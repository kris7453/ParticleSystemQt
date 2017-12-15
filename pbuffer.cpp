#include "pbuffer.h"

namespace PSystemAPI
{
    pBuffer::pBuffer( int size) : size(size)
    {
        buffer = new pParticle[size];
        aliveParticles = 0;
    }

    pBuffer::~pBuffer()
    {
        delete buffer;
    }

    void pBuffer::addParticles(pParticle *particles, int count)
    {
        void *pAddress = buffer + aliveParticles;
        memcpy( pAddress, particles, sizeof(pParticle) * count);
        aliveParticles += count;
    }

    void pBuffer::updateParticles(float deltaT)
    {
        for( pParticle *p = buffer, *end = buffer+aliveParticles; p < end; p++)
        {
            p->life -= deltaT;

            // Replace dead particle with last alive particle in buffer

            while( p->life <= 0 )
            {
                if ( p < end-1)
                {
                    memcpy(p, buffer+aliveParticles-1,sizeof(pParticle));
                    aliveParticles--;
                    end--;
                    p->life -= deltaT;
                }
                else
                {
                    aliveParticles--;
                    return;
                }
            }

            // Update values for live particle

        }
    }

    void pBuffer::loadBuffer(GLfloat *particlesBuffer)
    {
        pParticle *particle = buffer;
        GLfloat *bufferParticle = particlesBuffer;

        for( int i = 0; i < aliveParticles; i++, particle++, bufferParticle += 7)
        {
            *bufferParticle     = particle->position.x;
            *(bufferParticle+1) = particle->position.y;
            *(bufferParticle+2) = particle->size;
            *(bufferParticle+3) = 1.0f;
            *(bufferParticle+4) = 0.0f;
            *(bufferParticle+5) = 0.0f;
            *(bufferParticle+6) = 1.0f;
        }
    }

    int pBuffer::getAliveParticlesCount()
    {
        return aliveParticles;
    }
}
