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

    void pBuffer::addParticle(pParticle *particle)
    {
        //qDebug() << "life " << fixed << qSetRealNumberPrecision(2) << particle->life;
        void *pAddress = buffer + aliveParticles;
        memcpy( pAddress, particle, sizeof(pParticle));
        aliveParticles++;
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
                    memcpy(p, end-1,sizeof(pParticle));
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

            p->position += p->values.velocity * deltaT;
            p->color += p->values.colorIncreasePerSec * deltaT;
            p->size += p->values.growthPerSec * deltaT;
            p->spin += p->values.rotatePerSec * deltaT;

        }
    }

    void pBuffer::loadBuffer(GLfloat *particlesBuffer)
    {
        pParticle *particle = buffer;
        GLfloat *bufferParticle = particlesBuffer;
        //qDebug() << "load particles";
        for( int i = 0; i < aliveParticles; i++, particle++, bufferParticle += STRIDE_SIZE)
        {
            *bufferParticle     = particle->position.x();
            *(bufferParticle+1) = particle->position.y();
            *(bufferParticle+2) = particle->size;
            *(bufferParticle+3) = particle->spin;

            *(bufferParticle+4) = particle->color.x();
            *(bufferParticle+5) = particle->color.y();
            *(bufferParticle+6) = particle->color.z();
            *(bufferParticle+7) = particle->color.w();

            //qDebug()<< *bufferParticle << " " << *(bufferParticle+1) << " " << *(bufferParticle+2) << " " << *(bufferParticle+3) << " " << *(bufferParticle+4) << " " << *(bufferParticle+5) << " " << *(bufferParticle+6) << " " << *(bufferParticle+7) << " " << particle->life;
        }
    }

    int pBuffer::getAliveParticlesCount()
    {
        return aliveParticles;
    }
}