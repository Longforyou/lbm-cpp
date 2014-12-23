#include "periodic.h"

bool periodic(size_t x, size_t y, size_t dx, size_t dy)
{
    return ((x > 0 && x <= dx) && (y > 0 && y <= dy));
}

bool bounceback(size_t x, size_t y, size_t dx, size_t dy)
{
    // return false;
    // return false;
    // return x == 1 && y == 1;
    // return ((x == 3 || x == 4) && (y == 3 || y == 4));
    // return true;
    return (x == 0 || x == (dx - 1) || y == 0 || y == (dy - 1));
}

void initializeNodeAt(VelocitySet &set, Node &node, size_t idx)
{
    if (set.nDimensions != 2)
        throw "Dimensie van velocity set is niet 2";
    size_t nDirections = set.nDirections;
    node.type = Cell;

    Distribution *distributions = new Distribution[nDirections];
    for (size_t dir = 0; dir < nDirections; ++dir)
    {
        distributions[dir].value     = set.weights[dir];
        distributions[dir].nextValue = set.weights[dir];
    }
    node.distributions = distributions;
}

void connectNodeToNeighbours(VelocitySet &set, Node *nodes, size_t x, size_t y, size_t dx, size_t dy)
{
    if (set.nDimensions != 2)
        throw "Dimensie van velocity set is niet 2";

    size_t idx = x + dx * y;
    size_t nDirections = set.nDirections;

    // note this should be checked durign the streaming fase
    if (bounceback(x, y, dx, dy))
        nodes[idx].type = BoundaryNode;

    for (size_t dir = 0; dir < nDirections; ++dir)
    {
        // TODO: Check if neighbour node direciton is a boundary
        // then do boundary stuff
        // otherwise do periodic stuff
        size_t neighbour_x = x + set.directions[dir][0];
        size_t neighbour_y = y + set.directions[dir][1];

        if (bounceback(neighbour_x, neighbour_y, dx, dy))
        {
            // std::cout << "Bounce Back" << '\t';
            size_t op_dir = oppositeDirectionOf(dir);
            nodes[idx].distributions[dir].neighbour = &nodes[idx].distributions[op_dir].nextValue;
            // std::cout << "Bounce back on (" << x << ", " << y << ") from a value "
            //     << nodes[idx].distributions[dir].value << " to "
            //     << "(" << neighbour_x << ", " << neighbour_y << ")"
            //     << " with opp: " << op_dir << ", with value: " << *nodes[idx].distributions[dir].neighbour
            //     << '\n';
        }
        else
        {
            // std::cout << "Periodic" << '\t';
            // add dx and dy to the modulo statements to make it positive
            size_t neighbour_idx = ((neighbour_x + dx) % dx) + dx * ((neighbour_y + dy) % dy);
            nodes[idx].distributions[dir].neighbour = &nodes[neighbour_idx].distributions[dir].nextValue;
        }
    }
}

Node *initialize(VelocitySet &set, size_t &totalNodes, size_t dx, size_t dy)
{
    // Initialize the velocity set (note: should be available to all processors)
    initializeVelocitySet(set);

    totalNodes = dx * dy;
    Node *nodes = new Node[totalNodes];

    for (size_t x = 0; x < dx; ++x)
        for (size_t y = 0; y < dy; ++y)
            initializeNodeAt(set, nodes[x + dx * y], x + dx * y);

    for (size_t x = 0; x < dx; ++x)
    {
        for (size_t y = 0; y < dy; ++y)
            connectNodeToNeighbours(set, nodes, x, y, dx, dy);
        // std::cout << '\n' << '\n' << '\n';
    }

    reportOnInitialSetup(set, nodes, dx, dy);

    return nodes;
}

// apply zou he etc.
void applyBoundaryConditions(VelocitySet set, Node *nodes)
{
    return;
}

// in BoundaryConitions.cc :
// applyZouHe(VelocitySet set, Node &node, orientation)
