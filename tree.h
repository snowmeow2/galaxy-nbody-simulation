#pragma once
#include <vector>
#include <memory>

using namespace std;

class Node
{
public:
    // Properties
    int mass = 0.0;
    double pos[3],
        mass_center[3],
        size;
    Particle *particle = NULL; // not initialized
    vector<shared_ptr<Node>> child;

    // constructor
    Node(double pos[3], double size);

    // destructor
    ~Node();

    // Add a particle to the node
    void add(Particle &particle);

    // Divide the node into 8 children
    void divide();

    // Check if a particle is in the node
    bool is_in_node(double pos[3]);

    // Print the node and its children
    void print();
};