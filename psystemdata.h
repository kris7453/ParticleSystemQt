#ifndef PSYSTEMDATA_H
#define PSYSTEMDATA_H

namespace PSystemData
{
    // ------------------------- STRUCTS

    struct PParticle;

    template <class d> struct PCoord;
    template <class d> struct dataVariance;
    template <class d> struct dataRange;
    typedef PCoord<int> PSizei;
    typedef PCoord<float> PSizef;
    struct PColor;

    // ------------------------- STRUCTS DEFINITIONS

    template <class d> struct PCoord
    {
        d x,y;

        PCoord(){}
        PCoord( d x, d y) : x(x), y(y){}
    };

    template <class d> struct dataVariance
    {
        d data, variance;

        dataVariance(){}
        dataVariance( d data, d variance = 0) : data(data), variance(variance){}
    };

    template <class d> struct dataRange
    {
        dataVariance<d> start, end;

        dataRange(){}
        dataRange( dataVariance<d> start, dataVariance<d> end) : start(start), end(end){}
    };

    struct PColor
    {
        float r, g, b, a;

        PColor( float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a){}
        PColor( int r, int g, int b, int a = 255) : r( 255/r), g( 255/ g), b( 255/b), a( 255/a){}
    };

    struct PParticle
    {
        PCoord<int> position;
        PColor color;
        //PSizei size;

        PParticle( PCoord<int> pos = PCoord<int>(0,0), PColor color = PColor(1.0f,1.0f,1.0f)) : position(pos), color(color){}
    };

}

#endif // PSYSTEMDATA_H
