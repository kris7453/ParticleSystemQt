#include "pparticlesystem.h"

namespace PSystemAPI
{
    pParticleSystem::pParticleSystem()
    {
        timeElapsed = 0;
        particles = new pBuffer();

        oGLFunct->glGenVertexArrays(1,&vao);

        oGLFunct->glBindVertexArray(vao);
        oGLFunct->glBindBuffer(GL_ARRAY_BUFFER, bilboard);
        oGLFunct->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), reinterpret_cast<GLvoid*>(0)); // bilboard coord
        oGLFunct->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), reinterpret_cast<GLvoid*>(2*sizeof(GLfloat))); // texture coord
        oGLFunct->glEnableVertexAttribArray(0);
        oGLFunct->glEnableVertexAttribArray(1);

        oGLFunct->glGenBuffers(1, &propertiesBuffer);
        oGLFunct->glBindBuffer(GL_ARRAY_BUFFER, propertiesBuffer);
        oGLFunct->glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*7*128, nullptr, GL_DYNAMIC_DRAW);
        oGLFunct->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), reinterpret_cast<GLvoid*>(0)); // particle coord
        oGLFunct->glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), reinterpret_cast<GLvoid*>(2*sizeof(GLfloat))); // particle size
        oGLFunct->glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), reinterpret_cast<GLvoid*>(3*sizeof(GLfloat))); // particle color
        oGLFunct->glEnableVertexAttribArray(2);
        oGLFunct->glEnableVertexAttribArray(3);
        oGLFunct->glEnableVertexAttribArray(4);

        oGLFunct->glVertexAttribDivisor(0,0); // textureCoord
        oGLFunct->glVertexAttribDivisor(1,0); // particleBilboardCoord
        oGLFunct->glVertexAttribDivisor(2,1); // particlePosition
        oGLFunct->glVertexAttribDivisor(3,1); // particleSize
        oGLFunct->glVertexAttribDivisor(4,1); // particleColor
        oGLFunct->glBindVertexArray(0);
    }

    pParticleSystem::pParticleSystem(QVector2D position, QVector4D color) : pParticleSystem()
    {
//        float r,g,b,a, posX,posY;
//        r = color.x();
//        g = color.y();
//        b = color.z();
//        a = color.w();
//        posX = position.x();
//        posY = position.y();
//        GLfloat particlesProperties[] = {    posX+20,  posY+30, 10,r,g,b,a,
//                                             posX-20,  posY+30, 20,r,g,b,a,
//                                             posX+20,  posY-30, 30,r,g,b,a,
//                                             posX+70,  posY+30, 40,r,g,b,a,
//                                             posX+128, posY+65, 50,r,g,b,a};


//        this->position = position;
//        oGLFunct->glBindBuffer(GL_ARRAY_BUFFER, propertiesBuffer);
//        oGLFunct->glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(particlesProperties),reinterpret_cast<GLvoid*>(particlesProperties));
        pParticle *pbuffer = new pParticle[5];

        for(short i = 0; i < 5; i++)
            pbuffer[i].setValues(1.0f, pPosition(position.x() + (rand()%161 - 80),position.y() + (rand()%161 - 80)),20);

        particles->addParticles(pbuffer, 5);
    }

    void pParticleSystem::updateParticles(float deltaT)
    {
        timeElapsed += deltaT;
        particles->updateParticles(deltaT);

        if(timeElapsed >= 1.0f)
        {
            timeElapsed -= 1.0f;

            pParticle *pbuffer = new pParticle[5];

            for(short i = 0; i < 5; i++)
                pbuffer[i].setValues(1.0f, pPosition(position.x() + (rand()%161 - 80),position.y() + (rand()%161 - 80)),20);

            particles->addParticles(pbuffer, 5);
        }
    }

    void pParticleSystem::draw()
    {
        size_t aliveParticles = particles->getAliveParticlesCount();
        size_t bufferSize = 7 * aliveParticles;
        GLfloat *buffer = new GLfloat[bufferSize];
        particles->loadBuffer(buffer);
        oGLFunct->glBindBuffer(GL_ARRAY_BUFFER, propertiesBuffer);
        oGLFunct->glBufferSubData(GL_ARRAY_BUFFER,0,bufferSize*sizeof(GLfloat),reinterpret_cast<GLvoid*>(buffer));
        delete [] buffer;

        oGLFunct->glBindVertexArray(vao);
        oGLFunct->glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, aliveParticles);
    }
}
