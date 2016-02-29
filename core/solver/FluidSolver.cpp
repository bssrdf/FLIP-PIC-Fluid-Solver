//
// Created by austin on 2/28/16.
//

#include "FluidSolver.h"

float FluidSolver::g = -9.80665f;

FluidSolver::FluidSolver(float particleSep) : particle_radius(particleSep) {
}

FluidSolver::~FluidSolver() {
    delete _container;
}

void FluidSolver::setContainer(GeoObject *container) {
    _container = container;
}

void FluidSolver::addFluid(GeoObject *fluid) {
    Bound& b = fluid->bound();
    for (float x = b.minX(); x < b.maxX(); x += particle_radius) {
        for (float y = b.minY(); y < b.maxY(); y += particle_radius) {
            for (float z = b.minZ(); z < b.maxZ(); z += particle_radius) {
                glm::vec3 pos = glm::vec3(x, y, z);
                if (fluid->contains(pos)) {
                    FluidParticle p;
                    p.pos = pos;
                    _particles.push_back(p);
                    _particlesOld.push_back(p);
                }
            }
        }
    }
}

void FluidSolver::update(float step) {
    presolve(step);
    solve(step);
    postsolve(step);
}

void FluidSolver::presolve(float step) {

}

void FluidSolver::solve(float step) {
//    for (FluidParticle &p : _particles) {
//        p.vel.y += g * step;
//        p.pos += p.vel * step;
//    }
    for (unsigned int i = 0; i < _particles.size(); i++) {
        _particles[i].vel.y = _particlesOld[i].vel.y + g * step;
        _particles[i].pos += _particlesOld[i].vel * step;
        if (_container->collides(_particlesOld[i].pos, _particles[i].pos)) {
            _particles[i].col = glm::vec3(1,0,0);
            _particlesOld[i].col = glm::vec3(1,0,0);
        }
    }
    swap(_particles, _particlesOld);
}

void FluidSolver::postsolve(float step) {

}
