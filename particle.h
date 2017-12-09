#ifndef PARALLEL_PROG_LABS_PARTICLE_H
#define PARALLEL_PROG_LABS_PARTICLE_H

#include "vector.h"

class Particle{
public:
    double mass;
    Vector pos;
    Vector vel;

    Particle(double mass, Vector pos, Vector vel=0);
};

#endif //PARALLEL_PROG_LABS_PARTICLE_H
