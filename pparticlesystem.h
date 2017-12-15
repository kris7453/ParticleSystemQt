#ifndef PPARTICLESYSTEM_H
#define PPARTICLESYSTEM_H

#include "pdrawableitem.h"
#include "pbuffer.h"
#include <time.h>

namespace PSystemAPI
{
    class pParticleSystem : public pDrawableItem
    {
        public:
            pParticleSystem();
            pParticleSystem( QVector2D position, QVector4D color);

            void updateParticles( float deltaT);
            void draw();

        private:
            pBuffer *particles;
            float timeElapsed;
            QVector2D position;
    };
}

#endif // PPARTICLESYSTEM_H
