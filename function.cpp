#include "function.h"
#include "parameter.h"

void accelerate(Node node, Particle &p)
{
    // Calculate the distance between the particle and the node
    double distance = softening;
    for (int i = 0; i < 3; i++)
    {
        distance += pow((p.pos[i] - node.mass_center[i]), 2);
    }
    distance = sqrt(distance);

    if (node.child.size() != 0)
    {
        // If the node has children, check if the node is far away
        if (node.size / distance < 0.5)
        {
            // If the node is far away, calculate the acceleration directly
            double acc = G * node.mass * pow(distance, -3);
            for (int i = 0; i < 3; i++)
            {
                p.new_acc[i] += acc * (node.mass_center[i] - p.pos[i]);
            }
        }
        else
        {
            // If the node is close, check the acceleration for each child node
            for (int i = 0; i < node.child.size(); i++)
            {
                accelerate(*node.child[i], p);
            }
        }
    }
    else
    {
        // If the node has no children, calculate the acceleration directly
        if ((node.particle != NULL) && (node.particle != &p))
        {
            double acc = G * node.particle->mass * pow(distance, -3);
            for (int i = 0; i < 3; i++)
            {
                p.new_acc[i] += acc * (node.mass_center[i] - p.pos[i]);
            }
        }
    }
}

void update(float dt, Node node, Particle &p)
{
    // Get position at t+dt
    for (int i = 0; i < 3; i++)
    {
        p.new_pos[i] = p.pos[i] + p.vel[i] * dt + 0.5 * p.acc[i] * pow(dt, 2);
    }

    // Get acceleration at t+dt
    accelerate(node, p);

    // Get velocity at t+dt and reset the acceleration
    for (int i = 0; i < 3; i++)
    {
        p.vel[i] = p.vel[i] + 0.5 * (p.acc[i] + p.new_acc[i]) * dt;

        p.acc[i] = p.new_acc[i];
        p.new_acc[i] = 0.0;
    }
}