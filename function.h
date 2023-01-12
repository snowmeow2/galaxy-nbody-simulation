#pragma once
#include "particle.h"
#include "tree.h"

// Calculate the acceleration of the particle
void accelerate(Node node, Particle &p);

// update the position and velocity of the particle with Verlet integration
void update(float dt, Node node, Particle &p);