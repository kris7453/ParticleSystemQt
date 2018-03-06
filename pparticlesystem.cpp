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
        restartTime = 0.0f;
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

    void pParticleSystem::loadFromFile(QString filePath)
    {
        QFile file(filePath);
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            return;
        }

        QXmlStreamReader reader;
        reader.setDevice(&file);
        reader.readNext();

        // read to the begining of values

        while ( !reader.atEnd())
        {
            if (reader.isStartElement())
                if ( reader.name() == "dict")
                {
                    reader.readNextStartElement();
                    break;
                }

            reader.readNextStartElement();
        }

        // reading values

        while ( !reader.atEnd())
        {
            if (reader.isStartElement())
                if ( reader.name() == "key")
                {
                    QString key = reader.readElementText();
                    reader.readNextStartElement();
                    if ( reader.name() == "real" || reader.name() == "integer" )
                    {
                        if ( key == "angle" )
                            setAngle(reader.readElementText().toShort());

                        if ( key == "angleVariance" )
                            setAngleVariance(reader.readElementText().toShort());

                        if ( key == "duration" )
                            setDurationTime(reader.readElementText().toFloat());

                        if ( key == "emitterType" )
                            setSystemMode(reader.readElementText().toShort() == 0 ? pSystemMode::gravity : pSystemMode::radius);

                        if ( key == "maxParticles" )
                        {
                            int max = reader.readElementText().toInt();
                            setMaxParticles( max > 10000 ? 10000 : max );
                        }

                        if ( key == "emissionRate" )
                            setSpawnRate(reader.readElementText().toInt());

                        if ( key == "particleLifespan" )
                            setParticlesLife(reader.readElementText().toFloat());

                        if ( key == "particleLifespanVariance" )
                            setLifeVariance(reader.readElementText().toFloat());

                        //gravity

                        if ( key == "sourcePositionVariancex" )
                            setPositionVarianceX(reader.readElementText().toInt());

                        if ( key == "sourcePositionVariancey" )
                            setPositionVarianceY(reader.readElementText().toInt());

                        if ( key == "speed" )
                            setSpeed(reader.readElementText().toShort());

                        if ( key == "speedVariance" )
                            setSpeedVariance(reader.readElementText().toShort());

                        if ( key == "gravityx" )
                            setGravityX(reader.readElementText().toInt());

                        if ( key == "gravityy" )
                            setGravityY(reader.readElementText().toInt());

                        if ( key == "radialAcceleration" )
                            setRadialAccValue(reader.readElementText().toShort());

                        if ( key == "radialAccelerationVariance" )
                            setRadialAccVariance(reader.readElementText().toShort());

                        if ( key == "tangentialAcceleration" )
                            setTangentialAccValue(reader.readElementText().toShort());

                        if ( key == "tangentialAccelVariance" )
                            setTangentialAccVariance(reader.readElementText().toShort());

                        // radius

                        if ( key == "minRadius" )
                            setEndRadius(reader.readElementText().toShort());

                        if ( key == "minRadius" )
                            setEndRadiusVariance(reader.readElementText().toShort());

                        if ( key == "maxRadius" )
                            setStartRadius(reader.readElementText().toShort());

                        if ( key == "maxRadiusVariance" )
                            setStartRadiusVariance(reader.readElementText().toShort());

                        if ( key == "rotatePerSecond" )
                            setRotatePerSec(reader.readElementText().toShort());

                        if ( key == "rotatePerSecondVariance" )
                            setRotatePerSecVariance(reader.readElementText().toShort());

                        // particle properties

                        if ( key == "rotationStart" )
                            setParticleStartSpin(reader.readElementText().toShort());

                        if ( key == "rotationStartVariance" )
                            setParticleStartSpinVariance(reader.readElementText().toShort());

                        if ( key == "rotationEnd" )
                            setParticleEndSpin(reader.readElementText().toShort());

                        if ( key == "rotationEndVariance" )
                            setParticleEndSpinVariance(reader.readElementText().toShort());

                        if ( key == "startParticleSize" )
                            setParticleStartSize(reader.readElementText().toShort());

                        if ( key == "startParticleSizeVariance" )
                            setParticleStartSizeVariance(reader.readElementText().toShort());

                        if ( key == "finishParticleSize" )
                            setParticleEndSize(reader.readElementText().toShort());

                        if ( key == "finishParticleSizeVariance" )
                            setParticleEndSizeVariance(reader.readElementText().toShort());

                        //color

                        if ( key == "blendFuncSource" )
                            setSrcBlendingFactor(reader.readElementText().toUInt());

                        if ( key == "blendFuncDestination" )
                            setDstBlendingFactor(reader.readElementText().toUInt());

                        if ( key == "startColorAlpha" )
                            setParticleStartColorAlpha(reader.readElementText().toDouble());

                        if ( key == "startColorBlue" )
                            setParticleStartColorBlue(reader.readElementText().toDouble());

                        if ( key == "startColorGreen" )
                            setParticleStartColorGreen(reader.readElementText().toDouble());

                        if ( key == "startColorRed" )
                            setParticleStartColorRed(reader.readElementText().toDouble());

                        if ( key == "startColorVarianceAlpha" )
                            setParticleStartColorVarianceAlpha(reader.readElementText().toDouble());

                        if ( key == "startColorVarianceBlue" )
                            setParticleStartColorVarianceBlue(reader.readElementText().toDouble());

                        if ( key == "startColorVarianceGreen" )
                            setParticleStartColorVarianceGreen(reader.readElementText().toDouble());

                        if ( key == "startColorVarianceRed" )
                            setParticleStartColorVarianceRed(reader.readElementText().toDouble());

                        if ( key == "finishColorAlpha" )
                            setParticleEndColorAlpha(reader.readElementText().toDouble());

                        if ( key == "finishColorBlue" )
                            setParticleEndColorBlue(reader.readElementText().toDouble());

                        if ( key == "finishColorGreen" )
                            setParticleEndColorGreen(reader.readElementText().toDouble());

                        if ( key == "finishColorRed" )
                            setParticleEndColorRed(reader.readElementText().toDouble());

                        if ( key == "finishColorVarianceAlpha" )
                            setParticleEndColorVarianceAlpha(reader.readElementText().toDouble());

                        if ( key == "finishColorVarianceBlue" )
                            setParticleEndColorVarianceBlue(reader.readElementText().toDouble());

                        if ( key == "finishColorVarianceGreen" )
                            setParticleEndColorVarianceGreen(reader.readElementText().toDouble());

                        if ( key == "finishColorVarianceRed" )
                            setParticleEndColorVarianceRed(reader.readElementText().toDouble());
                    }
                }

            reader.readNextStartElement();
        }

        file.close();
    }

    void pParticleSystem::saveToFile( QString filePath)
    {
        QFile file(filePath);
        file.open(QIODevice::WriteOnly);

        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();
        xmlWriter.writeDTD("<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">");
            xmlWriter.writeStartElement("plist");
            xmlWriter.writeAttribute("version", "1.0");
                xmlWriter.writeStartElement("dict");

                    xmlWriter.writeTextElement("key", "angle");
                    xmlWriter.writeTextElement("integer", QString::number(angle));

                    xmlWriter.writeTextElement("key", "angleVariance");
                    xmlWriter.writeTextElement("integer", QString::number(angleVariance));

                    xmlWriter.writeTextElement("key", "blendFuncDestination");
                    xmlWriter.writeTextElement("integer", QString::number(dstBlendingFactor));

                    xmlWriter.writeTextElement("key", "blendFuncSource");
                    xmlWriter.writeTextElement("integer", QString::number(srcBlendingFactor));

                    xmlWriter.writeTextElement("key", "duration");
                    xmlWriter.writeTextElement("integer", QString::number(durationTime));

                    xmlWriter.writeTextElement("key", "emitterType");
                    xmlWriter.writeTextElement("integer", QString::number(mode == pSystemMode::gravity ? 0 : 1 ));

                    QColor color = pProperties.endColor;

                    xmlWriter.writeTextElement("key", "finishColorAlpha");
                    xmlWriter.writeTextElement("real", QString::number( color.alphaF() ));

                    xmlWriter.writeTextElement("key", "finishColorBlue");
                    xmlWriter.writeTextElement("real", QString::number( color.blueF() ));

                    xmlWriter.writeTextElement("key", "finishColorGreen");
                    xmlWriter.writeTextElement("real", QString::number( color.greenF() ));

                    xmlWriter.writeTextElement("key", "finishColorRed");
                    xmlWriter.writeTextElement("real", QString::number( color.redF() ));

                    color = pVariances.endColor;

                    xmlWriter.writeTextElement("key", "finishColorVarainceAlpha");
                    xmlWriter.writeTextElement("real", QString::number( color.alphaF() ));

                    xmlWriter.writeTextElement("key", "finishColorVarianceBlue");
                    xmlWriter.writeTextElement("real", QString::number( color.blueF() ));

                    xmlWriter.writeTextElement("key", "finishColorVarianceGreen");
                    xmlWriter.writeTextElement("real", QString::number( color.greenF() ));

                    xmlWriter.writeTextElement("key", "finishColorVarianceRed");
                    xmlWriter.writeTextElement("real", QString::number( color.redF() ));

                    xmlWriter.writeTextElement("key", "finishParticleSize");
                    xmlWriter.writeTextElement("real", QString::number( pProperties.endSize));

                    xmlWriter.writeTextElement("key", "finishParticleSizeVariance");
                    xmlWriter.writeTextElement("integer", QString::number( pVariances.endSize));

                    xmlWriter.writeTextElement("key", "gravityx");
                    xmlWriter.writeTextElement("real", QString::number( pProperties.gravity.x() ));

                    xmlWriter.writeTextElement("key", "gravityy");
                    xmlWriter.writeTextElement("real", QString::number( pProperties.gravity.y() ));

                    xmlWriter.writeTextElement("key", "maxParticles");
                    xmlWriter.writeTextElement("integer", QString::number( maxParticles ));

                    xmlWriter.writeTextElement("key", "maxRadius");
                    xmlWriter.writeTextElement("integer", QString::number( pProperties.startRadius));

                    xmlWriter.writeTextElement("key", "maxRadiusVariance");
                    xmlWriter.writeTextElement("integer", QString::number( pVariances.startRadius));

                    xmlWriter.writeTextElement("key", "minRadius");
                    xmlWriter.writeTextElement("integer", QString::number( pProperties.endRadius));

                    xmlWriter.writeTextElement("key", "particleLifespan");
                    xmlWriter.writeTextElement("real", QString::number( pProperties.life/1000.0f));

                    xmlWriter.writeTextElement("key", "particleLifespanVariance");
                    xmlWriter.writeTextElement("real", QString::number( pVariances.life/1000.0f));

                    xmlWriter.writeTextElement("key", "radialAccelVariance");
                    xmlWriter.writeTextElement("real", QString::number( pVariances.radialAccValue));

                    xmlWriter.writeTextElement("key", "radialAcceleration");
                    xmlWriter.writeTextElement("real", QString::number( pProperties.radialAccValue));

                    xmlWriter.writeTextElement("key", "rotatePerSecond");
                    xmlWriter.writeTextElement("integer", QString::number( pProperties.rotatePerSec));

                    xmlWriter.writeTextElement("key", "rotatePerSecondVariance");
                    xmlWriter.writeTextElement("integer", QString::number( pVariances.rotatePerSec));

                    xmlWriter.writeTextElement("key", "rotationEnd");
                    xmlWriter.writeTextElement("real", QString::number( pProperties.endSpin));

                    xmlWriter.writeTextElement("key", "rotationEndVariance");
                    xmlWriter.writeTextElement("real", QString::number( pVariances.endSpin));

                    xmlWriter.writeTextElement("key", "rotationStart");
                    xmlWriter.writeTextElement("real", QString::number( pProperties.startSpin));

                    xmlWriter.writeTextElement("key", "rotationStartVariance");
                    xmlWriter.writeTextElement("real", QString::number( pVariances.startSpin));

                    xmlWriter.writeTextElement("key", "sourcePositionVariancex");
                    xmlWriter.writeTextElement("integer", QString::number( positionVariance.x()));

                    xmlWriter.writeTextElement("key", "sourcePositionVariancey");
                    xmlWriter.writeTextElement("integer", QString::number( positionVariance.y()));

                    xmlWriter.writeTextElement("key", "sourcePositionx");
                    xmlWriter.writeTextElement("real", QString::number( 0.0f));

                    xmlWriter.writeTextElement("key", "sourcePositiony");
                    xmlWriter.writeTextElement("real", QString::number( 0.0f));

                    xmlWriter.writeTextElement("key", "speed");
                    xmlWriter.writeTextElement("integer", QString::number( pProperties.speed));

                    xmlWriter.writeTextElement("key", "speedVariance");
                    xmlWriter.writeTextElement("integer", QString::number( pVariances.speed));

                    color = pProperties.startColor;

                    xmlWriter.writeTextElement("key", "startColorAlpha");
                    xmlWriter.writeTextElement("real", QString::number( color.alphaF() ));

                    xmlWriter.writeTextElement("key", "startColorBlue");
                    xmlWriter.writeTextElement("real", QString::number( color.blueF() ));

                    xmlWriter.writeTextElement("key", "startColorGreen");
                    xmlWriter.writeTextElement("real", QString::number( color.greenF() ));

                    xmlWriter.writeTextElement("key", "startColorRed");
                    xmlWriter.writeTextElement("real", QString::number( color.redF() ));

                    color = pVariances.startColor;

                    xmlWriter.writeTextElement("key", "startColorVarainceAlpha");
                    xmlWriter.writeTextElement("real", QString::number( color.alphaF() ));

                    xmlWriter.writeTextElement("key", "startColorVarianceBlue");
                    xmlWriter.writeTextElement("real", QString::number( color.blueF() ));

                    xmlWriter.writeTextElement("key", "startColorVarianceGreen");
                    xmlWriter.writeTextElement("real", QString::number( color.greenF() ));

                    xmlWriter.writeTextElement("key", "startColorVarianceRed");
                    xmlWriter.writeTextElement("real", QString::number( color.redF() ));

                    xmlWriter.writeTextElement("key", "startParticleSize");
                    xmlWriter.writeTextElement("real", QString::number( pProperties.startSize));

                    xmlWriter.writeTextElement("key", "startParticleSizeVariance");
                    xmlWriter.writeTextElement("integer", QString::number( pVariances.startSize));

                    xmlWriter.writeTextElement("key", "tangentialAccelVariance");
                    xmlWriter.writeTextElement("real", QString::number( pVariances.tangentialAccValue));

                    xmlWriter.writeTextElement("key", "tangentialAcceleration");
                    xmlWriter.writeTextElement("real", QString::number( pProperties.tangentialAccValue));

                    xmlWriter.writeTextElement("key", "emissionRate");
                    xmlWriter.writeTextElement("real", QString::number( spawnRate));

                xmlWriter.writeEndElement(); // dist
            xmlWriter.writeEndElement(); // plist
        xmlWriter.writeEndDocument();

        file.close();
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
        pbuffer->values.gravity = pProperties.gravity;
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
        else
            if (restartTime < 3.0f)
                restartTime += deltaT;
            else
                restartTime = simulatingTime = 0.0f;

        if ( simulatingTime < durationTime || durationTime == -1 )
        {
            timeElapsed += deltaT;

            for( ; timeElapsed >= spawnTimeSpan && !isFull() ; timeElapsed -= spawnTimeSpan)
                spawnParticle();
        }

        particles->updateParticles(deltaT);
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

    void pParticleSystem::setParticleStartColorAlpha(double alpha)
    {
        this->pProperties.startColor.setAlphaF(alpha);
    }

    void pParticleSystem::setParticleStartColorRed(double red)
    {
        this->pProperties.startColor.setRedF(red);
    }

    void pParticleSystem::setParticleStartColorGreen(double green)
    {
        this->pProperties.startColor.setGreenF(green);
    }

    void pParticleSystem::setParticleStartColorBlue(double blue)
    {
        this->pProperties.startColor.setBlueF(blue);
    }

    void pParticleSystem::setParticleStartColorVariance(QColor color)
    {
        this->pVariances.startColor = color;
    }

    void pParticleSystem::setParticleStartColorVarianceAlpha(int alpha)
    {
        this->pVariances.startColor.setAlpha(alpha);
    }

    void pParticleSystem::setParticleStartColorVarianceAlpha(double alpha)
    {
        this->pVariances.startColor.setAlphaF(alpha);
    }

    void pParticleSystem::setParticleStartColorVarianceRed(double red)
    {
        this->pVariances.startColor.setRedF(red);
    }

    void pParticleSystem::setParticleStartColorVarianceGreen(double green)
    {
        this->pVariances.startColor.setGreenF(green);
    }

    void pParticleSystem::setParticleStartColorVarianceBlue(double blue)
    {
        this->pVariances.startColor.setBlueF(blue);
    }

    void pParticleSystem::setParticleEndColor(QColor color)
    {
        this->pProperties.endColor = color;
    }

    void pParticleSystem::setParticleEndColorAlpha(int alpha)
    {
        this->pProperties.endColor.setAlpha(alpha);
    }

    void pParticleSystem::setParticleEndColorAlpha(double alpha)
    {
        this->pProperties.endColor.setAlphaF(alpha);
    }

    void pParticleSystem::setParticleEndColorRed(double red)
    {
        this->pProperties.endColor.setRedF(red);
    }

    void pParticleSystem::setParticleEndColorGreen(double green)
    {
        this->pProperties.endColor.setGreenF(green);
    }

    void pParticleSystem::setParticleEndColorBlue(double blue)
    {
        this->pProperties.endColor.setBlueF(blue);
    }

    void pParticleSystem::setParticleEndColorVariance(QColor color)
    {
        this->pVariances.endColor = color;
    }

    void pParticleSystem::setParticleEndColorVarianceAlpha(int alpha)
    {
        this->pVariances.endColor.setAlpha(alpha);
    }

    void pParticleSystem::setParticleEndColorVarianceAlpha(double alpha)
    {
        this->pVariances.endColor.setAlphaF(alpha);
    }

    void pParticleSystem::setParticleEndColorVarianceRed(double red)
    {
        this->pVariances.endColor.setRedF(red);
    }

    void pParticleSystem::setParticleEndColorVarianceGreen(double green)
    {
        this->pVariances.endColor.setGreenF(green);
    }

    void pParticleSystem::setParticleEndColorVarianceBlue(double blue)
    {
        this->pVariances.endColor.setBlueF(blue);
    }

    void pParticleSystem::setParticlesColor(QColor startColor, QColor startColorVariance)
    {
        this->pProperties.startColor = startColor;
        this->pProperties.endColor = startColor;
        this->pVariances.startColor = startColorVariance;
        this->pVariances.endColor = startColorVariance;
    }

    void pParticleSystem::setParticlesColor(QColor startColor, QColor startColorVariance, QColor endColor, QColor endColorVariance )
    {
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
    }

    void pParticleSystem::setAngleVariance(short variance)
    {
        this->angleVariance = variance;
    }

    void pParticleSystem::setSpeed(short speed)
    {
        this->pProperties.speed = speed;
    }

    void pParticleSystem::setSpeedVariance(short variance)
    {
        this->pVariances.speed = variance;
    }

    void pParticleSystem::setGravityX(int x)
    {
        this->pProperties.gravity.setX( x);
    }

    void pParticleSystem::setGravityY(int y)
    {
        this->pProperties.gravity.setY( y);
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
    }

    void pParticleSystem::setDstBlendingFactor(int factor)
    {
        dstBlendingFactor = factor;
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
        return pProperties.life / 1000.0f;
    }

    float pParticleSystem::getParticleLifeVariance()
    {
        return pVariances.life / 1000.0f;
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

    int pParticleSystem::getGravityY()
    {
        return pProperties.gravity.y();
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
