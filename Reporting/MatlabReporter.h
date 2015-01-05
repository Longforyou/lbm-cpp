#ifndef INCLUDED_REPORTING_MATLAB_REPORTER
#define INCLUDED_REPORTING_MATLAB_REPORTER

#include "../main.h"
#include <iostream>

namespace Reporting {
    class MatlabReporter {
        std::ostream &d_ostream;

        public:
        MatlabReporter(std::ostream &ofstream);

        void initialReport(VelocitySet &set, size_t dx, size_t dy);
        void reportOnTimeStep(VelocitySet &set, Node *nodes, size_t totalNodes);

        private:
        void showVector(double *vector, size_t nDimensions);
        void showVector(int *vector, size_t nDimensions);
        void showVector(size_t *vector, size_t nDimensions);
        void showNode(VelocitySet &set, Node &node);

    };
}

#endif