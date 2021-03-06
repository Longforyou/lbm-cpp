#include "reporting.h"

#include <iostream>
#include <iomanip>
#include <limits>
#include <fstream>
#include <sstream>

#include "MatlabReporter.h"

namespace Reporting {

    void reportOnInitialSetup(VelocitySet *set, Node *nodes, size_t dx, size_t dy)
    {
        std::cout << "LBM D" << set->nDimensions << "Q" << set->nDirections << " simulation." << '\n'
            << "Grid size " << dx << "x" << dy
            << '\n' << '\n';
        // Shows a grid of letters representing the type of each node
        // for (size_t y = dy; y--;)
        // {
        //     for (size_t x = 0; x < dx; ++x)
        //     {
        //         size_t idx = x + dx * y;
        //         if (nodes[idx].type == NodeType::ZouHe)
        //             std::cout << 'Z';
        //         else if (nodes[idx].type == NodeType::BounceBack)
        //             std::cout << 'B';
        //         else
        //             std::cout << '.';
        //     }
        //     std::cout << '\n';
        // }
    }

    void reportOnNode(VelocitySet *set, Node &node)
    {
        double node_density = density(set, node);
        double *node_velocity = velocity(set, node);

        size_t nDimensions = set->nDimensions;
        // show position
        std::cout << "(";
        for (size_t dir = 0; dir < nDimensions; ++dir)
            std::cout << node.position[dir] << ", ";
        // show density
        std::cout << ") | (" << node_density << ") | (";
        // show velocity
        for (size_t dim = 0; dim < nDimensions; ++dim)
            std::cout << node_velocity[dim] << ", ";
        std::cout << ")" << '\n';
        delete[] node_velocity;
    }

    void reportOnDistributions(VelocitySet *set, Node &node)
    {
        size_t nDirections = set->nDirections;
        std::stringstream ss;



        ss << "Current:     (";
        ss << node.position[0] << ", " << node.position[1] << "), (";
        for (size_t dir = 0; dir < nDirections; ++dir)
            ss << node.distributions[dir].value << ", ";
        ss << ")" << '\n';

        ss << "Next:        (";
        ss << node.position[0] << ", " << node.position[1] << "), (";
        for (size_t dir = 0; dir < nDirections; ++dir)
            ss << node.distributions[dir].nextValue << ", ";
        ss << ")" << '\n';

        ss << "Neighbour:   (";
        ss << node.position[0] << ", " << node.position[1] << "), (";
        for (size_t dir = 0; dir < nDirections; ++dir)
            ss << *node.distributions[dir].neighbour << ", ";
        ss << ")" << '\n';
        std::cout << ss.str();
    }

    void report(VelocitySet *set, Node *nodes, size_t dx, size_t dy)
    {
        std::cout << '\n' << '\n';
        size_t totalNodes = dx * dy;
        for (size_t idx = 0; idx < totalNodes; ++idx)
            reportOnNode(set, nodes[idx]);
    }

    // Report on the total density and the total velocity
    void report(VelocitySet *set, Node *nodes, size_t totalNodes)
    {
        /*
        for (size_t p = 0; p < totalProcessors; ++p)
        {
            if (p == P)
            {
                std::ofstream out("logs/test.txt");
                Reporting::MatlabReporter reporter(out);
                reporter.reportOnTimeStep(set, nodes, totalNodes);
                bsp_sycn();

            }
        }
        */


        double total_density = 0;
        size_t nDimensions = set->nDimensions;
        double *total_velocity = new double[nDimensions]();
        for (size_t idx = 0; idx < totalNodes; ++idx)
        {
            total_density += density(set, nodes[idx]);

            double *node_velocity = velocity(set, nodes[idx]);
            for (size_t dim = 0; dim < nDimensions; ++dim)
                total_velocity[dim] += node_velocity[dim];

            delete[] node_velocity;
        }

        std::cout << "Total density: " << total_density << '\n';


        return;
        std::cout << "Total velocity: ";
        for (size_t dim = 0; dim < nDimensions; ++dim)
            std::cout << total_velocity[dim] << '\t';
        std::cout << '\n';

        delete[] total_velocity;
    }

    void report(std::string outputFileName, VelocitySet *set, Node *nodes, size_t totalNodes)
    {
        std::ofstream out(outputFileName, std::ios::out | std::ios::app);
        Reporting::MatlabReporter reporter(out);
        reporter.reportOnTimeStep(set, nodes, totalNodes);
    }
}