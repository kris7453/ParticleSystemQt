#include "pparticlesystem.h"

namespace PSystemAPI
{
    pParticleSystem::pParticleSystem( QString *resourceImagePath, QString name) : pDrawableItem(resourceImagePath, name)
    {
        particleSystem = true;

        maxParticles = 10000;
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

    pParticleSystem::pParticleSystem(QVector2D position, QString *resourceImagePath, QString name) : pParticleSystem(resourceImagePath, name)
    {
        if ( resourceImagePath == nullptr )
            resourceImagePath = new QString(":/particles/star.png");

        srcBlendingFactor = GL_SRC_ALPHA;
        dstBlendingFactor = GL_ONE;

        setPosition(position);
        setPositionVariance(QVector2D(0.0f,0.0f));
        setSystemMode( pSystemMode::radius );
        
        setParticlesLife(3.1f, 0.4f);
        setParticlesSize(40, 10, 100, 0);
        setParticlesSpin( 0, 0, 0, 0);
        setParticlesColor(QColor(255*0.2f, 255*0.4f, 255*0.69f, 255*1.0f),
                          QColor(0, 0, 0, 0),
                          QColor(0, 0, 0, 255*1.0f),
                          QColor(0, 0, 0, 0));

        timeElapsed = 0.0f;
        simulatingTime = 0.0f;
        durationTime = -1.0f;
        setSpawnRate(200);
        angle = 90;
        angleVariance = 0;
qDebug() << name << " duration time " << durationTime << " simulating time & max particles " << simulatingTime << " " << maxParticles;
        // Gravity

        pProperties.speed = 10;
        pVariances.speed = 0;
        velocityDirection = QVector2D(cos(angle*M_PI/180),sin(angle*M_PI/180));
        pProperties.velocity = QVector2D(cos(angle*M_PI/180),sin(angle*M_PI/180)) * pProperties.speed;

        pProperties.gravity = QVector2D(0,0);
        pVariances.gravity = QVector2D(0,0);

        pProperties.radialAccValue = 0;
        pVariances.radialAccValue = 0;
        pProperties.tangentialAccValue = 0;
        pVariances.tangentialAccValue = 0;

        // Radius

        pProperties.startRadius = 400;
        pVariances.startRadius = 0;
        pProperties.endRadius = 0;
        pVariances.endRadius = 0;
        pProperties.rotatePerSec = 180;
        pVariances.rotatePerSec = 0;

    }

    void pParticleSystem::spawnParticle()
    {
        float life = (pProperties.life + (rand()%(2*pVariances.life+1) - pVariances.life)) / 1000.0f;
        QVector2D posVariance = QVector2D(rand()%(2*static_cast<int>(positionVariance.x())+1) - positionVariance.x(),
                                          rand()%(2*static_cast<int>(positionVariance.y())+1) - positionVariance.y());

        short angle = this->angle + rand()%(2*this->angleVariance+1) - this->angleVariance;
        double angleInRadians = angle /180.0f*M_PI;
        short speed = pProperties.speed + rand()%(2*pVariances.speed+1) - pVariances.speed;

        pProperties.velocity = QVector2D(cos(angleInRadians),sin(angleInRadians)) * speed;

        QVector2D gravityVariance = QVector2D(rand()%(2*static_cast<int>(pVariances.gravity.x())+1) - pVariances.gravity.x(),
                                         rand()%(2*static_cast<int>(pVariances.gravity.y())+1) - pVariances.gravity.y());

        short radialAccValue = pProperties.radialAccValue + ( rand() % (2*pVariances.radialAccValue+1) - pVariances.radialAccValue);
        short tangentialAccValue = pProperties.tangentialAccValue + ( rand() % (2*pVariances.tangentialAccValue+1) - pVariances.tangentialAccValue);

        GLfloat startRadius = pProperties.startRadius + rand()%(2*pVariances.startRadius+1) - pVariances.startRadius;
        GLfloat endRadius = pProperties.endRadius + rand()%(2*pVariances.endRadius+1) - pVariances.endRadius;
        GLfloat rotatePerSec = pProperties.rotatePerSec + rand()%(2*pVariances.rotatePerSec+1) - pVariances.rotatePerSec;

        GLfloat startSize = pProperties.startSize + rand()%(2*pVariances.startSize+1) - pVariances.startSize;
        GLfloat startSpin = pProperties.startSpin + rand()%(2*pVariances.startSpin+1) - pVariances.startSpin;

        GLfloat endSize = pProperties.endSize + rand()%(2*pVariances.endSize+1) - pVariances.endSize;
        GLfloat endSpin = pProperties.endSpin + rand()%(2*pVariances.endSpin+1) - pVariances.endSpin;

        int r,g,b,a;

        r = pVariances.startColor.red();
        r = r == 0 ? 0 : rand()%(2*r+1) - r;
        r += pProperties.startColor.red();
        r = r > 255 ? 255 : (r < 0 ? 0 : r);

        g = pVariances.startColor.green();
        g = g == 0 ? 0 : rand()%(2*g+1) - g;
        g += pProperties.startColor.green();
        g = g > 255 ? 255 : (g < 0 ? 0 : g);

        b = pVariances.startColor.blue();
        b = b == 0 ? 0 : rand()%(2*b+1) - b;
        b += pProperties.startColor.blue();
        b = b > 255 ? 255 : (b < 0 ? 0 : b);

        a = pVariances.startColor.alpha();
        a = a == 0 ? 0 : rand()%(2*a+1) - a;
        a += pProperties.startColor.alpha();
        a = a > 255 ? 255 : (a < 0 ? 0 : a);

        QVector4D startColor = QVector4D( r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);

        r = pVariances.endColor.red();
        r = r == 0 ? 0 : rand()%(2*r+1) - r;
        r += pProperties.endColor.red();
        r = r > 255 ? 255 : (r < 0 ? 0 : r);

        g = pVariances.endColor.green();
        g = g == 0 ? 0 : rand()%(2*g+1) - g;
        g += pProperties.endColor.green();
        g = g > 255 ? 255 : (g < 0 ? 0 : g);

        b = pVariances.endColor.blue();
        b = b == 0 ? 0 : rand()%(2*b+1) - b;
        b += pProperties.endColor.blue();
        b = b > 255 ? 255 : (b < 0 ? 0 : b);

        a = pVariances.endColor.alpha();
        a = a == 0 ? 0 : rand()%(2*a+1) - a;
        a += pProperties.endColor.alpha();
        a = a > 255 ? 255 : (a < 0 ? 0 : a);

        QVector4D endColor = QVector4D( r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);

        pParticle *pbuffer = new pParticle(
                    life,
                    this->mode == pSystemMode::gravity ?
                        position + posVariance:
                        position + QVector2D(-cos(angleInRadians), -sin(angleInRadians)) * startRadius,
                    startSize,
                    startSpin,
                    startColor);

        pbuffer->values.centerPosition = this->position;

        // Gravity
        pbuffer->values.velocity = pProperties.velocity;
        pbuffer->values.gravity = pProperties.gravity + gravityVariance;
        pbuffer->values.radialAccValue = radialAccValue;
        pbuffer->values.tangentialAccValue = tangentialAccValue;

        // Radius
        pbuffer->values.radius = startRadius;
        pbuffer->values.radiusIncreasePerSec = (endRadius - startRadius) / life;
        pbuffer->values.angle = angle;
        pbuffer->values.rotatePerSec = rotatePerSec;

        // Outlook
        pbuffer->values.growthPerSec = (endSize - startSize) / life;
        pbuffer->values.colorIncreasePerSec = (endColor - startColor) / life;
        pbuffer->values.spinPerSec = (startSpin - endSpin) / life; // Rotate to the opposite side than mathematicial

        particles->addParticle(pbuffer);
        delete pbuffer;
    }

    void pParticleSystem::updateParticles(float deltaT)
    {
        if ( durationTime > 0 && simulatingTime < durationTime )
            simulatingTime += deltaT;
//qDebug() << name <<  " simulating time " << simulatingTime << " for " << durationTime << " durationTime";
        if ( simulatingTime < durationTime || durationTime == -1 )
        {//qDebug() <<" simulate " ;
            timeElapsed += deltaT;

            for( ; timeElapsed >= spawnTimeSpan && !isFull() ; timeElapsed -= spawnTimeSpan)
                spawnParticle();
        }


        particles->updateParticles(deltaT);
        //qDebug() << particles->getAliveParticlesCount() << " alive particles";
    }

    void pParticleSystem::draw()
    {
        size_t aliveParticles = particles->getAliveParticlesCount();
        //qDebug() << "alive" << aliveParticles;
        if ( aliveParticles > 0 )
        {//qDebug() << "System draws " << aliveParticles << "particles ";
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
            oGLFunct->glEnable(GL_BLEND);
            oGLFunct->glBlendFunc(srcBlendingFactor,dstBlendingFactor);

            oGLFunct->glBindBuffer(GL_ARRAY_BUFFER, propertiesBuffer);
            oGLFunct->glBufferSubData(GL_ARRAY_BUFFER,0,bufferSize*sizeof(GLfloat),reinterpret_cast<GLvoid*>(buffer));
            delete [] buffer;

            oGLFunct->glActiveTexture(GL_TEXTURE0);
            texture->bind();

            oGLFunct->glBindVertexArray(vao);
            oGLFunct->glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, aliveParticles);
        }
    }

    void pParticleSystem::restart()
    {
        particles->restart();
        timeElapsed = 0.0f;
        simulatingTime = 0.0f;
    }

    bool pParticleSystem::isFull()
    {
        return particles->getAliveParticlesCount() >= maxParticles;
    }

    void pParticleSystem::setSystemMode(pSystemMode mode)
    {
        this->mode = mode;
        this->particles->changeUpdateMode(mode);
        restart();
    }

    void pParticleSystem::setPosition(QVector2D position)
    {
        this->position = position;
    }

    void pParticleSystem::setPositionVariance(QVector2D positionVariance)
    {
        this->positionVariance = positionVariance;
    }

    void pParticleSystem::setPositionVarianceX(int variance)
    {
        positionVariance.setX(variance);
    }

    void pParticleSystem::setPositionVarianceY(int variance)
    {
        positionVariance.setY(variance);
    }

    void pParticleSystem::setDurationTime(float time)
    {qDebug()<< name << " set durration to " << time;
        this->durationTime = time;
        simulatingTime = 0.0f;
        timeElapsed = 0.0f;
    }

    void pParticleSystem::setSpawnRate(int rate)
    {
        this->spawnRate = rate;
        spawnTimeSpan = 1.0f / spawnRate;
    }

    void pParticleSystem::setMaxParticles(int max)
    {
        this->maxParticles = max;
    }

    void pParticleSystem::setParticlesLife(int life, int lifeVariance)
    {
        this->pProperties.life = life;// 1000.0f;
        this->pVariances.life = lifeVariance;
    }

    void pParticleSystem::setParticlesLife(float life)
    {
        this->pProperties.life = life * 1000;
    }

    void pParticleSystem::setParticlesLife(float life, float lifeVariance)
    {
        this->pProperties.life = life * 1000;
        this->pVariances.life = lifeVariance * 1000;
    }

    void pParticleSystem::setLifeVariance(int lifeVariance)
    {
        this->pVariances.life = lifeVariance;
    }

    void pParticleSystem::setLifeVariance(float lifeVariance)
    {
        this->pVariances.life = lifeVariance * 1000.0f;
    }

    void pParticleSystem::setParticleStartColor(QColor color)
    {
        this->pProperties.startColor = color;
    }

    void pParticleSystem::setParticleStartColorAlpha(int alpha)
    {
        this->pProperties.startColor.setAlpha(alpha);
    }

    void pParticleSystem::setParticleStartColorVariance(QColor color)
    {
        this->pVariances.startColor = color;
    }

    void pParticleSystem::setParticleStartColorVarianceAlpha(int alpha)
    {
        this->pVariances.startColor.setAlpha(alpha);
    }

    void pParticleSystem::setParticleEndColor(QColor color)
    {
        this->pProperties.endColor = color;
    }

    void pParticleSystem::setParticleEndColorAlpha(int alpha)
    {
        this->pProperties.endColor.setAlpha(alpha);
    }

    void pParticleSystem::setParticleEndColorVariance(QColor color)
    {
        this->pVariances.endColor = color;
    }

    void pParticleSystem::setParticleEndColorVarianceAlpha(int alpha)
    {
        this->pVariances.endColor.setAlpha(alpha);
    }

    void pParticleSystem::setParticlesColor(QColor startColor, QColor startColorVariance)
    {
        this->pProperties.startColor = startColor;
        this->pProperties.endColor = startColor;
        this->pVariances.startColor = startColorVariance;
        this->pVariances.endColor = startColorVariance;
    }

    void pParticleSystem::setParticlesColor(QColor startColor, QColor startColorVariance, QColor endColor, QColor endColorVariance )
    {qDebug() << "start color " << startColor;
        this->pProperties.startColor = startColor;
        this->pProperties.endColor = endColor;
        this->pVariances.startColor = startColorVariance;
        this->pVariances.endColor = endColorVariance;
    }

    void pParticleSystem::setParticleStartSpin( short spin)
    {
        this->pProperties.startSpin = spin;
    }

    void pParticleSystem::setParticleStartSpinVariance( short variance)
    {
        this->pVariances.startSpin = variance;
    }

    void pParticleSystem::setParticleEndSpin( short spin)
    {
        this->pProperties.endSpin = spin;
    }

    void pParticleSystem::setParticleEndSpinVariance( short variance)
    {
        this->pVariances.startSpin = variance;
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

    void pParticleSystem::setParticleStartSize( short size)
    {
        this->pProperties.startSize = size;
    }

    void pParticleSystem::setParticleStartSizeVariance( short variance)
    {
        this->pVariances.startSize = variance;
    }

    void pParticleSystem::setParticleEndSize( short size)
    {
        this->pProperties.endSize = size;
    }

    void pParticleSystem::setParticleEndSizeVariance( short variance)
    {
        this->pVariances.endSize = variance;
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

    void pParticleSystem::setAngle(short angle)
    {
        this->angle = angle;
//        this->velocityDirection = QVector2D(cos(angle*M_PI/180),sin(angle*M_PI/180));
//        this->pProperties.velocity = velocityDirection* pProperties.speed;
    }

    void pParticleSystem::setAngleVariance(short variance)
    {
        this->angleVariance = variance;
    }

    void pParticleSystem::setSpeed(short speed)
    {
        this->pProperties.speed = speed;
//        this->pProperties.velocity =  velocityDirection * pProperties.speed;
    }

    void pParticleSystem::setSpeedVariance(short variance)
    {
        this->pVariances.speed = variance;
    }

    void pParticleSystem::setGravityX(int x)
    {
        this->pProperties.gravity.setX( x);
    }

    void pParticleSystem::setGravityXVariance(int variance)
    {
        this->pVariances.gravity.setX( variance);
    }

    void pParticleSystem::setGravityY(int y)
    {
        this->pProperties.gravity.setY( y);
    }

    void pParticleSystem::setGravityYVariance(int variance)
    {
        this->pVariances.gravity.setY( variance);
    }

    void pParticleSystem::setRadialAccValue(short acceleration)
    {
        this->pProperties.radialAccValue = acceleration;
    }

    void pParticleSystem::setRadialAccVariance(short variance)
    {
        this->pVariances.radialAccValue = variance;
    }

    void pParticleSystem::setRadialAccValue(short acceleration, short accelerationVariance)
    {
        this->pProperties.radialAccValue = acceleration;
        this->pVariances.radialAccValue = accelerationVariance;
    }

    void pParticleSystem::setTangentialAccValue(short acceleration)
    {
        this->pProperties.tangentialAccValue = acceleration;
    }

    void pParticleSystem::setTangentialAccVariance(short variance)
    {
        this->pVariances.tangentialAccValue = variance;
    }

    void pParticleSystem::setTangentialAccValue(short acceleration, short accelerationVariance)
    {
        this->pProperties.tangentialAccValue = acceleration;
        this->pVariances.tangentialAccValue = accelerationVariance;
    }

    void pParticleSystem::setStartRadius(int radius)
    {
        this->pProperties.startRadius = radius;
    }

    void pParticleSystem::setStartRadiusVariance(int variance)
    {
        this->pVariances.startRadius = variance;
    }

    void pParticleSystem::setEndRadius(int radius)
    {
        this->pProperties.endRadius = radius;
    }

    void pParticleSystem::setEndRadiusVariance(int variance)
    {
        this->pVariances.endRadius = variance;
    }

    void pParticleSystem::setRotatePerSec(int rotates)
    {
        this->pProperties.rotatePerSec = rotates;
    }

    void pParticleSystem::setRotatePerSecVariance(int variance)
    {
        this->pVariances.rotatePerSec = variance;
    }

    void pParticleSystem::setSrcBlendingFactor(int factor)
    {
        srcBlendingFactor = factor;
        qDebug()<<name <<" now using src " << srcBlendingFactor << " dst " << dstBlendingFactor;
    }

    void pParticleSystem::setDstBlendingFactor(int factor)
    {
        dstBlendingFactor = factor;
        qDebug()<<name <<" now using src " << srcBlendingFactor << " dst " << dstBlendingFactor;
    }

    QColor pParticleSystem::getParticleStartColor()
    {
        return pProperties.startColor;
    }

    QColor pParticleSystem::getParticleStartColorVariance()
    {
        return pVariances.startColor;
    }

    QColor pParticleSystem::getParticleEndColor()
    {
        return pProperties.endColor;
    }

    QColor pParticleSystem::getParticleEndColorVariance()
    {
        return pVariances.endColor;
    }

    int pParticleSystem::getParticleStartSpin()
    {
        return pProperties.startSpin;
    }

    int pParticleSystem::getParticleStartSpinVariance()
    {
        return pVariances.startSpin;
    }

    int pParticleSystem::getParticleEndSpin()
    {
        return pProperties.endSpin;
    }

    int pParticleSystem::getParticleEndSpinVariance()
    {
        return pVariances.endSpin;
    }

    int pParticleSystem::getParticleStartSize()
    {
        return pProperties.startSize;
    }

    int pParticleSystem::getParticleStartSizeVariance()
    {
        return pVariances.startSize;
    }

    int pParticleSystem::getParticleEndSize()
    {
        return pProperties.endSize;
    }

    int pParticleSystem::getParticleEndSizeVariance()
    {
        return pVariances.endSize;
    }

    int pParticleSystem::getSystemMode()
    {
        return static_cast<int>(mode);
    }

    float pParticleSystem::getParticleLife()
    {
        return pProperties.life;
    }

    float pParticleSystem::getParticleLifeVariance()
    {
        return pVariances.life;
    }

    float pParticleSystem::getDurationTime()
    {
        return durationTime;
    }

    float pParticleSystem::getSpawnRate()
    {
        return spawnRate;
    }

    int pParticleSystem::getMaxParticles()
    {
        return maxParticles;
    }

    int pParticleSystem::getPositionVarianceX()
    {
        return positionVariance.x();
    }

    int pParticleSystem::getPositionVarianceY()
    {
        return positionVariance.y();
    }

    int pParticleSystem::getAngleVariance()
    {
        return angleVariance;
    }

    int pParticleSystem::getSpeed()
    {
        return pProperties.speed;
    }

    int pParticleSystem::getSpeedVariance()
    {
        return pVariances.speed;
    }

    int pParticleSystem::getGravityX()
    {
        return pProperties.gravity.x();
    }

    int pParticleSystem::getGravityXVariance()
    {
        return pVariances.gravity.x();
    }

    int pParticleSystem::getGravityY()
    {
        return pProperties.gravity.y();
    }

    int pParticleSystem::getGravityYVariance()
    {
        return pVariances.gravity.y();
    }

    int pParticleSystem::getRadialAccValue()
    {
        return pProperties.radialAccValue;
    }

    int pParticleSystem::getRadialAccVariance()
    {
        return pVariances.radialAccValue;
    }

    int pParticleSystem::getTangentialAccValue()
    {
        return pProperties.tangentialAccValue;
    }

    int pParticleSystem::getTangentialAccVariance()
    {
        return pVariances.tangentialAccValue;
    }

    int pParticleSystem::getStartRadius()
    {
        return pProperties.startRadius;
    }

    int pParticleSystem::getStartRadiusVariance()
    {
        return pVariances.startRadius;
    }

    int pParticleSystem::getEndRadius()
    {
        return pProperties.endRadius;
    }

    int pParticleSystem::getEndRadiusVariance()
    {
        return pVariances.endRadius;
    }

    int pParticleSystem::getRotatePerSec()
    {
        return pProperties.rotatePerSec;
    }

    int pParticleSystem::getRotatePerSecVariance()
    {
        return pVariances.rotatePerSec;
    }

    int pParticleSystem::getSrcBlendingFactor()
    {
        return srcBlendingFactor;
    }

    int pParticleSystem::getDstBlendingFactor()
    {
        return dstBlendingFactor;
    }

    bool pParticleSystem::operator==(const pParticleSystem &item)
    {
        return reinterpret_cast<int>(this) == reinterpret_cast<int>(&item);
    }
}
