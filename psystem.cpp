#include "psystem.h"

PSystem::PSystem( const char *xmlPath)
{
    //pixelSize = pSizei(oglWidget->);

}

void PSystem::importXML( const char *xmlPath)
{

}

void PSystem::addParticles(short i)
{
    particles.insert( particles.begin(), PParticle( PCoord<int>(0,0), PColor(1.0f, 1.0f, 1.0f)));
    //particles.push_back( PParticle( PCoord<int>(0,0), PColor(1.0f, 1.0f, 1.0f)));
}

vector<PParticle> *PSystem::getPList()
{
    return &particles;
}
