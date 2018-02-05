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

            if ( updateMode == pSystemMode::gravity)
            {
                // Calculate acceleration and velocity

                QVector2D acceleration = p->values.gravity;
                QVector2D radialVector = p->position - p->values.centerPosition;
                radialVector.normalize();

                if( radialVector.length() > 0 && p->values.tangentialAccValue != 0)
                {
                    double angle = atan(radialVector.y()/radialVector.x()) + M_PI_2;

                    if ( radialVector.x() < 0 && radialVector.y() > 0)
                        angle += M_PI;
                    else
                        if ( radialVector.x() < 0 && radialVector.y() < 0)
                            angle -= M_PI;

                    QVector2D tangentialVector = QVector2D(cos(angle), sin(angle)) * p->values.tangentialAccValue;
                    acceleration +=  tangentialVector;
                }

                acceleration += radialVector * p->values.radialAccValue;
                p->values.velocity += acceleration * deltaT;
                p->position += p->values.velocity * deltaT;
            }
            else // Radius mode
            {
                p->values.angle += p->values.rotatePerSec * deltaT;
                p->values.radius += p->values.radiusIncreasePerSec * deltaT;

                float angleInRadians = p->values.angle / 180*M_PI;
                p->position = p->values.centerPosition + QVector2D(-cos(angleInRadians), -sin(angleInRadians)) * p->values.radius;
            }

            p->color += p->values.colorIncreasePerSec * deltaT;
            p->size += p->values.growthPerSec * deltaT;
            p->spin += p->values.spinPerSec * deltaT;

        }
    }

    void pBuffer::changeUpdateMode(pSystemMode mode)
    {
        this->updateMode = mode;
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
