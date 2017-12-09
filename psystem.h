#ifndef PSYSTEM_H
#define PSYSTEM_H

#include <QOpenGLWidget>
#include <vector>
#include "psystemdata.h"

    using namespace std;
    using namespace PSystemData;

class PSystem
{
    public:
        PSystem         ( const char *xmlPath = nullptr);
        void importXML  ( const char *xmlPath);

        // ------------------------- FUNCTIONS

        void setPixelSize( PSizei contextSize);
        void addParticles( short i);
        vector<PParticle> *getPList();

    private:

        vector<PParticle> particles;

        float                   duration;
        dataVariance<float>     angle;
        dataRange<float>        color;
        dataRange<float>        size;


};

#endif // PSYSTEM_H
