#include <omp.h>
#include <iostream>
#include <fstream>
#include "parameter.h"
#include "particle.h"
#include "tree.h"
#include "function.h"

using namespace std;

int main()
{
    // Initialize
    double Size = 1000.0, // Size of the simulation [kpc]
        Origin[3] = {0 - Size / 2, 0 - Size / 2, 0 - Size / 2};

    // Read particles positions and velocities from file
    ifstream file("halo.txt");
    cout << "Reading file..."
         << "\n";

    // Create a folder to store the output
    ifstream folder("out");
    if (!folder)
    {
        system("mkdir out");
        cout << "Created ./out folder"
             << "\n";
    }
    else
    {
        system("rm out/*");
        cout << "Cleaned ./out folder"
             << "\n";
    }

    // copy the input file to the output folder
    system("cp halo.txt out/halo_0.txt");

    int total_particles;
    file >> total_particles;
    cout << "Number of particles: " << total_particles << "\n";
    cout << "Number of threads: " << omp_get_max_threads() << "\n";

    // Initialize particles
    Particle *particles = new Particle[total_particles];

    for (int i = 0; i < total_particles; i++)
    {
        file >> particles[i].pos[0] >> particles[i].pos[1] >> particles[i].pos[2] >> particles[i].vel[0] >> particles[i].vel[1] >> particles[i].vel[2];
        particles[i].mass = Mass;
    }

    // Timer
    double tstart, tend, tmid;
    tstart = omp_get_wtime();
    tmid = omp_get_wtime();

    // Start the iteration
    cout << "Set up a tree with size " << Size
         << " and origin at " << Origin[0] << ", " << Origin[1] << ", " << Origin[2] << "\n";
    cout << "Start the iteration...(N = " << N << ")"
         << "\n";

    for (int i = 0; i < N; i++)
    {
        // Create the tree
        Node root = Node(Origin, Size);
        for (int j = 0; j < total_particles; j++)
        {
            root.add(particles[j]);
        }

#pragma omp parallel for
        // Update the particles (velocity)
        for (int j = 0; j < total_particles; j++)
        {
            update(dt, root, particles[j]);
        }

#pragma omp parallel for
        // Update the particles (position)
        for (int j = 0; j < total_particles; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                particles[j].pos[k] = particles[j].new_pos[k];
            }
        }

        // Write the particles positions to file
        if (i % save_iter == 0 && i != 0)
        {
            cout << "Iteration " << i << " done!"
                 << "\n";
            cout << "Time elapsed: " << omp_get_wtime() - tmid << " seconds"
                 << "\n";
            tmid = omp_get_wtime();

            ofstream file("out/halo_" + to_string(i) + ".txt");
            // file << total_particles << "\n";
            for (int j = 0; j < total_particles; j++)
            {
                file << particles[j].pos[0] << " " << particles[j].pos[1] << " " << particles[j].pos[2] << " "
                     << particles[j].vel[0] << " " << particles[j].vel[1] << " " << particles[j].vel[2] << "\n";
            }
            file.close();
        }
    }

    // Calculate time elapsed
    tend = omp_get_wtime();
    cout << "Time elapsed: " << tend - tstart << " seconds"
         << "\n";
    cout << "Time per iteration: " << (tend - tstart) / N << " seconds"
         << "\n";

    cout << "Done!"
         << "\n";
    return 0;
}