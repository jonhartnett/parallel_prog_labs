#include <cstdlib>
#include "io-util.h"
#include "util.h"
#include "vector.h"
#include "particle.h"

#define G 6.673e-11

void solveBasic(Particle particles[], size_t n);
void solveReduced(Particle particles[], size_t n);
void stepEuler(Particle particles[], size_t n, double dt);
void stepRungeKutta(Particle particles[], size_t n, double dt);

int main(){
    /**
     * Example input for the 3-body system of the earth, moon, and sun
        n: 3
        timespan: 3.155693e7
        steps: 1000000
        body 0:
            mass: 1.98855e30
            position: 0 0 0
            velocity: 0 0 0
        body 1:
            mass: 5.9722e24
            position: 1.4959787e11 0 0
            velocity: 0 2.9786e4 0
        body 2:
            mass: 7.34767309e22
            position: 1.4959787e11 3.85e8 0
            velocity: -1022 2.9786e4 0
     */

    StreamIterator iter = allocStreamIter(stdin);
    printf("n: ");
    size_t n = readUInt32(iter);
    printf("timespan: ");
    double timespan = readFloat64(iter);
    printf("steps: ");
    size_t steps = readUInt32(iter);
    double dt = timespan / steps;

    Particle* particles = array(Particle, n);
    for(size_t i = 0; i < n; i++){
        printf("body %ld:\n    mass: ", i);
        double mass = readFloat64(iter);
        printf("    position: ");
        Vector pos = Vector(readFloat64(iter), readFloat64(iter), readFloat64(iter));
        printf("    velocity: ");
        Vector vel = Vector(readFloat64(iter), readFloat64(iter), readFloat64(iter));
        particles[i] = Particle(mass, pos, vel);
    }

    for(size_t step = 0; step < steps; step++)
        stepRungeKutta(particles, n, dt);

    for(size_t i = 0; i < n; i++){
        Particle& p = particles[i];
        printf("body %ld:\n    position: %lf %lf %lf\n    velocity: %lf %lf %lf\n", i, p.pos.x, p.pos.y, p.pos.z, p.vel.x, p.vel.y, p.vel.z);
    }

    freeStreamIter(iter);
}

void solveSingle(Particle particles[], size_t n, size_t q){
    particles[q].acc = 0;
    for(size_t k = 0; k < n; k++){
        if(q == k)
            continue;
        Particle& pq = particles[q];
        Particle& pk = particles[k];
        Vector dif = pq.pos - pk.pos;
        double disSq = dif.lengthSq();
        double multi = G * pk.mass / disSq;
        pq.acc -= dif.normalized() * multi;
    }
}
void solveBasic(Particle particles[], size_t n){
    for(size_t i = 0; i < n; i++)
        particles[i].acc = 0;
    for(size_t q = 0; q < n; q++)
    for(size_t k = 0; k < n; k++){
        if(q == k)
            continue;
        Particle& pq = particles[q];
        Particle& pk = particles[k];
        Vector dif = pq.pos - pk.pos;
        double disSq = dif.lengthSq();
        double multi = G * pk.mass / disSq;
        pq.acc -= dif.normalized() * multi;
    }
}
void solveReduced(Particle particles[], size_t n){
    for(size_t i = 0; i < n; i++)
        particles[i].acc = 0;
    for(size_t q = 0; q < n; q++)
    for(size_t k = q + 1; k < n; k++){
        Particle& pq = particles[q];
        Particle& pk = particles[k];
        Vector dif = pq.pos - pk.pos;
        double disSq = dif.lengthSq();
        double multi = G / disSq;
        Vector force = dif.normalized() * multi;
        pq.acc -= force * pk.mass;
        pk.acc += force * pq.mass;
    }
}

void stepEuler(Particle particles[], size_t n, double dt){
    solveReduced(particles, n);
    for(size_t i = 0; i < n; i++){
        Particle& p = particles[i];
        p.vel += p.acc * dt;
        p.pos += p.vel * dt;
    }
}

void stepRungeKutta(Particle particles[], size_t n, double dt){
    solveReduced(particles, n);
    for(size_t i = 0; i < n; i++){
        Particle& p = particles[i];

        Vector pos = p.pos;
        Vector vel = p.vel;

        Vector vk1 = p.acc;
        Vector pk1 = vel;

        p.pos = pos + pk1 * (dt / 2);
        solveSingle(particles, n, i);
        Vector vk2 = p.acc;
        Vector pk2 = vel + vk1 * (dt / 2);

        p.pos = pos + pk2 * (dt / 2);
        solveSingle(particles, n, i);
        Vector vk3 = p.acc;
        Vector pk3 = vel + vk2 * (dt / 2);

        p.pos = pos + pk3 * dt;
        solveSingle(particles, n, i);
        Vector vk4 = p.acc;
        Vector pk4 = vel + vk3 * dt;

        p.pos = pos;

        p.vel += (vk1 + 2*vk2 + 2*vk3 + vk4) * dt / 6;
        p.pos += (pk1 + 2*pk2 + 2*pk3 + pk4) * dt / 6;
    }
}