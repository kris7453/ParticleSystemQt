#include "pparticlesystem.h"

namespace PSystemAPI
{
    pParticleSystem::pParticleSystem()
    {
        timeElapsed = 0;
        maxParticles = 3000;
        particles = new pBuffer(maxParticles);

        oGLFunct->glGenVertexArrays(1,&vao);

        oGLFunct->glBindVertexArray(vao);
        oGLFunct->glBindBuffer(GL_ARRAY_BUFFER, bilboard);
        oGLFunct->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), reinterpret_cast<GLvoid*>(0)); // bilboard coord
        oGLFunct->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), reinterpret_cast<GLvoid*>(2*sizeof(GLfloat))); // texture coord
        oGLFunct->glEnableVertexAttribArray(0);
        oGLFunct->glEnableVertexAttribArray(1);

        oGLFunct->glGenBuffers(1, &propertiesBuffer);
        oGLFunct->glBindBuffer(GL_ARRAY_BUFFER, propertiesBuffer);
        oGLFunct->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*STRIDE_SIZE*maxParticles, nullptr, GL_DYNAMIC_DRAW);
        oGLFunct->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, STRIDE_SIZE*sizeof(GLfloat), reinterpret_cast<GLvoid*>(0)); // particle coord
        oGLFunct->glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, STRIDE_SIZE*sizeof(GLfloat), reinterpret_cast<GLvoid*>(2*sizeof(GLfloat))); // particle size
        oGLFunct->glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, STRIDE_SIZE*sizeof(GLfloat), reinterpret_cast<GLvoid*>(3*sizeof(GLfloat))); // particle spin
        oGLFunct->glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, STRIDE_SIZE*sizeof(GLfloat), reinterpret_cast<GLvoid*>(4*sizeof(GLfloat))); // particle color
        oGLFunct->glEnableVertexAttribArray(2);
        oGLFunct->glEnableVertexAttribArray(3);
        oGLFunct->glEnableVertexAttribArray(4);
        oGLFunct->glEnableVertexAttribArray(5);

        oGLFunct->glVertexAttribDivisor(0,0); // textureCoord
        oGLFunct->glVertexAttribDivisor(1,0); // particleBilboardCoord
        oGLFunct->glVertexAttribDivisor(2,1); // particlePosition
        oGLFunct->glVertexAttribDivisor(3,1); // particleSize
        oGLFunct->glVertexAttribDivisor(4,1); // particleSpin
        oGLFunct->glVertexAttribDivisor(5,1); // particleColor
        oGLFunct->glBindVertexArray(0);
    }

    pParticleSystem::pParticleSystem(QVector2D position) : pParticleSystem()
    {
        setPosition(position, QVector2D(30,40));
        
        setParticlesLife(1.1f, 0.4f);
        setParticlesSize(110, 10, 40, 0);
        setParticlesSpin( 720, 0, 0, 0);
        setParticlesColor(QVector4D(0.2f, 0.4f, 0.69f, 1.0f),
                          QVector4D(0.0f, 0.0f, 0.0f, 0.0f),
                          QVector4D(0.0f, 0.0f, 0.0f, 1.0f),
                          QVector4D(0.0f, 0.0f, 0.0f, 0.0f));

        spawnRate = 800.0f;
        spawnTimeSpan = 1.0f / spawnRate;
    }

    void pParticleSystem::spawnParticle()
    {
        float life = pProperties.life + (rand()%(2*pVariances.life+1) - pVariances.life) / 1000.0f;
        //qDebug() << fixed << qSetRealNumberPrecision(2) << "pLife " << life;
        QVector2D posVariance = QVector2D(rand()%(2*static_cast<int>(positionVariance.x())+1) - positionVariance.x(),
                                          rand()%(2*static_cast<int>(positionVariance.y())+1) - positionVariance.y());

        GLfloat startSize = pProperties.startSize + rand()%(2*pVariances.startSize+1) - pVariances.startSize;
        GLfloat startSpin = pProperties.startSpin + rand()%(2*pVariances.startSpin+1) - pVariances.startSpin;
        QVector4D startColor = pProperties.startColor + QVector4D(  rand()%(2*static_cast<int>(pVariances.startColor.x())+1) - pVariances.startColor.x(),
                                                                    rand()%(2*static_cast<int>(pVariances.startColor.y())+1) - pVariances.startColor.y(),
                                                                    rand()%(2*static_cast<int>(pVariances.startColor.z())+1) - pVariances.startColor.z(),
                                                                    rand()%(2*static_cast<int>(pVariances.startColor.w())+1) - pVariances.startColor.w())
                                                                    / 1000.0f;
        startColor.normalize();

        GLfloat endSize = pProperties.endSize + rand()%(2*pVariances.endSize+1) - pVariances.endSize;
        GLfloat endSpin = pProperties.endSpin + rand()%(2*pVariances.endSpin+1) - pVariances.endSpin;
        QVector4D endColor = pProperties.endColor + QVector4D(  rand()%(2*static_cast<int>(pVariances.endColor.x())+1) - pVariances.endColor.x(),
                                                                rand()%(2*static_cast<int>(pVariances.endColor.y())+1) - pVariances.endColor.y(),
                                                                rand()%(2*static_cast<int>(pVariances.endColor.z())+1) - pVariances.endColor.z(),
                                                                rand()%(2*static_cast<int>(pVariances.endColor.w())+1) - pVariances.endColor.w())
                                                                / 1000.0f;
        endColor.normalize();

        pParticle *pbuffer = new pParticle(
                    life,
                    position + posVariance,
                    startSize,
                    startSpin,
                    startColor);

        pbuffer->values.velocity = QVector2D(0,244);
        pbuffer->values.growthPerSec = (endSize - startSize) / life;
        pbuffer->values.colorIncreasePerSec = (endColor - startColor) / life;
        pbuffer->values.rotatePerSec = (endSpin - startSpin) / life;

        particles->addParticle(pbuffer);
        delete pbuffer;
    }

    void pParticleSystem::updateParticles(float deltaT)
    {
        timeElapsed += deltaT;
        particles->updateParticles(deltaT);

        for( ; timeElapsed >= spawnTimeSpan; timeElapsed -= spawnTimeSpan)
        {

            spawnParticle();
//            pParticle *pbuffer = new pParticle[5];

//            for(short i = 0; i < 5; i++)
//                pbuffer[i].setValues(1.0f, pPosition(position.x() + (rand()%161 - 80),position.y() + (rand()%161 - 80)),20);

//            particles->addParticles(pbuffer, 5);
        }
    }

    void pParticleSystem::draw()
    {
        size_t aliveParticles = particles->getAliveParticlesCount();
        //qDebug() << "alive" << aliveParticles;
        if ( aliveParticles > 0 )
        {
            size_t bufferSize = STRIDE_SIZE * aliveParticles;
            GLfloat *buffer = new GLfloat[bufferSize];

            particles->loadBuffer(buffer);

#ifdef DEBUG_LOADED_BUFFER
            qDebug() << "buffer";

            GLfloat *wsk=buffer;

            for(int i = 0; i < aliveParticles; i++, wsk+=STRIDE_SIZE)
            {
                qDebug() << *wsk << " " << *(wsk+1) << " " << *(wsk+2) << " " << *(wsk+3) << " " << *(wsk+4) << " " << *(wsk+5) << " " << *(wsk+6) << " " << *(wsk+7);
            }
#endif
            oGLFunct->glBindBuffer(GL_ARRAY_BUFFER, propertiesBuffer);
            oGLFunct->glBufferSubData(GL_ARRAY_BUFFER,0,bufferSize*sizeof(GLfloat),reinterpret_cast<GLvoid*>(buffer));
            delete [] buffer;

            oGLFunct->glBindVertexArray(vao);
            oGLFunct->glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, aliveParticles);
        }
    }

    void pParticleSystem::setPosition(QVector2D position, QVector2D positionVariance)
    {
        this->position = position;
        this->positionVariance = positionVariance;
    }

    void pParticleSystem::setPositionVariance(QVector2D positionVariance)
    {
        this->positionVariance = positionVariance;
    }

    void pParticleSystem::setParticlesLife(int life, int lifeVariance)
    {
        this->pProperties.life = life / 1000.0f;
        this->pVariances.life = lifeVariance;
    }

    void pParticleSystem::setParticlesLife(float life, float lifeVariance)
    {
        this->pProperties.life = life;
        this->pVariances.life = lifeVariance * 1000.0f;
    }

    void pParticleSystem::setLifeVariance(int lifeVariance)
    {
        this->pVariances.life = lifeVariance;
    }

    void pParticleSystem::setLifeVariance(float lifeVariance)
    {
        this->pVariances.life = lifeVariance * 1000.0f;
    }

    void pParticleSystem::setParticlesColor(QVector4D startColor, QVector4D startColorVariance)
    {
        this->pProperties.startColor = startColor;
        this->pProperties.endColor = startColor;
        this->pVariances.startColor = startColorVariance * 1000.0f;
        this->pVariances.endColor = startColorVariance * 1000.0f;
    }

    void pParticleSystem::setParticlesColor(QVector4D startColor,QVector4D startColorVariance, QVector4D endColor, QVector4D endColorVariance )
    {
        this->pProperties.startColor = startColor;
        this->pProperties.endColor = endColor;
        this->pVariances.startColor = startColorVariance * 1000.0f;
        this->pVariances.endColor = endColorVariance * 1000.0f;
    }

    void pParticleSystem::setParticlesSpin(short startSpin, short startSpinVariance)
    {
        this->pProperties.startSpin = startSpin;
        this->pProperties.endSpin = startSpin;
        this->pVariances.startSpin = startSpinVariance;
        this->pVariances.endSpin = startSpinVariance;
    }

    void pParticleSystem::setParticlesSpin(short startSpin, short startSpinVariance, short endSpin, short endSpinVariance)
    {
        this->pProperties.startSpin = startSpin;
        this->pProperties.endSpin = endSpin;
        this->pVariances.startSpin = startSpinVariance;
        this->pVariances.endSpin = endSpinVariance;
    }

    void pParticleSystem::setParticlesSize(short startSize, short startSizeVariance)
    {
        this->pProperties.startSize = startSize;
        this->pProperties.endSize = startSize;
        this->pVariances.startSize = startSizeVariance;
        this->pVariances.endSize = startSizeVariance;
    }

    void pParticleSystem::setParticlesSize(short startSize, short startSizeVariance, short endSize, short endSizeVariance)
    {
        this->pProperties.startSize = startSize;
        this->pProperties.endSize = endSize;
        this->pVariances.startSize = startSizeVariance;
        this->pVariances.endSize = endSizeVariance;
    }
}
