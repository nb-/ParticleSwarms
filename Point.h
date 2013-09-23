#ifndef POINT_H
#define POINT_H


struct ParticlePointer
{
    double* value;
    double* position;
    double* prevBestValue;
    double* prevBestPosition;
    virtual ~ParticlePointer(){}
};

struct ParticleVPointer: public ParticlePointer
{
    double* velocity;
    virtual ~ParticleVPointer(){}
};


#endif // POINT_H
