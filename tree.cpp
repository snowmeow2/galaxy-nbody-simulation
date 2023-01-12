#include <iostream>
#include "particle.h"
#include "tree.h"

Node::Node(double pos[3], double size)
{
    this->pos[0] = pos[0]; // x
    this->pos[1] = pos[1]; // y
    this->pos[2] = pos[2]; // z
    this->size = size;
}

Node::~Node()
{
    // recursively delete the children to avoid memory leaks
    child.clear();
}

void Node::add(Particle &particle)
{
    // Update mass and mass center of node
    if (this->mass == 0.0)
    {
        // if mass is not initialized, replace it with the the mass of the particle
        // and set the mass center to the position of the particle
        for (int i = 0; i < 3; i++)
        {
            this->mass_center[i] = particle.pos[i];
        }
        this->mass = particle.mass;
    }
    else
    {
        // if mass is initialized, update the mass and mass center
        float total_mass = this->mass + particle.mass;
        for (int i = 0; i < 3; i++)
        {
            this->mass_center[i] =
                (this->mass_center[i] * this->mass + particle.pos[i] * particle.mass) / total_mass;
        }
        this->mass = total_mass;
    }

    // Place the particle
    if (this->child.size() != 0)
    {
        // If node has children, check what child node the particle should be placed in
        // and add the particle to that child node
        for (int i = 0; i < this->child.size(); i++)
        {
            if (this->child[i]->is_in_node(particle.pos))
            {
                this->child[i]->add(particle);
                break;
            }
        }
    }
    else
    {
        // If node has no children, check if the node contains a particle
        if (this->particle == NULL)
        {
            // If node does not have a particle, add the particle to the node
            this->particle = &particle;
        }
        else
        {
            // If node contains a particle, create 8 children nodes
            // and assign both origin particle and new particle to the correct child node
            this->divide();

            // Place the particle that was already in the node
            for (int i = 0; i < this->child.size(); i++)
            {
                if (this->child[i]->is_in_node(this->particle->pos))
                {
                    this->child[i]->add(*this->particle);
                    break;
                }
            }

            // Remove the particle from the node
            this->particle = NULL;

            // Place the new particle
            for (int i = 0; i < this->child.size(); i++)
            {
                if (this->child[i]->is_in_node(particle.pos))
                {
                    this->child[i]->add(particle);
                    break;
                }
            }
        }
    }
}

void Node::divide()
{
    double child_size = this->size / 2,
           child_pos[3];
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                // Calculate the position of the child
                child_pos[0] = this->pos[0] + i * child_size;
                child_pos[1] = this->pos[1] + j * child_size;
                child_pos[2] = this->pos[2] + k * child_size;

                // Create the child and add it to the vector of children
                this->child.push_back(make_shared<Node>(child_pos, child_size));
            }
        }
    }
}

bool Node::is_in_node(double pos[3])
{
    if (pos[0] >= this->pos[0] && pos[0] < this->pos[0] + this->size &&
        pos[1] >= this->pos[1] && pos[1] < this->pos[1] + this->size &&
        pos[2] >= this->pos[2] && pos[2] < this->pos[2] + this->size)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Node::print()
{
    if (this->child.size() != 0)
    {
        for (int i = 0; i < this->child.size(); i++)
        {
            this->child[i]->print();
        }
    }
    else
    {
        cout << "Node: " << this->pos[0] << ", " << this->pos[1] << ", " << this->pos[2] << "\n";
        cout << "Mass: " << this->mass << "\n";
        cout << "Mass center: " << this->mass_center[0] << ", " << this->mass_center[1] << ", " << this->mass_center[2] << "\n";
        cout << "Size: " << this->size << "\n";
        if (this->particle != NULL)
        {
            cout << "Particle: " << this->particle->pos[0] << ", " << this->particle->pos[1] << ", " << this->particle->pos[2] << "\n";
        }
        cout << "Children: " << this->child.size() << "\n";
        cout << "\n";
    }
}