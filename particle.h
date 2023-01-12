#pragma once

struct Particle
{
    int mass;
    double pos[3],
        vel[3],
        acc[3] = {0.0, 0.0, 0.0},
        new_pos[3] = {0.0, 0.0, 0.0},
        new_acc[3] = {0.0, 0.0, 0.0};
};