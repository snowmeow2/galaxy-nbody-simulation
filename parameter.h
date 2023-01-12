#pragma once
#include <cmath>

const float G = 4.47e-6; // Gravitational constant [kpc^3 / (Msun * Gyr^2)]
const float Mass = 1e6;  // Mass of the particles [Msun]

const float dt = 0.0001; // Time step [Gyr] = 1e5 years
const float softening = pow(1.5 * pow(10000, -0.44), 2);
const int N = 5000;        // Iterations = 5e8 years
const int save_iter = 100; // Save the positions every N iterations