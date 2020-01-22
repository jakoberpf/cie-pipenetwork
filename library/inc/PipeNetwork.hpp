//
// Created by Jakob Erpf on 08.12.19.
//

#ifndef LINALG_PIPENETWORK_HPP
#define LINALG_PIPENETWORK_HPP

#include "Node.hpp"
#include <string>
#include "Parameters.hpp"

class PipeNetwork;

namespace cie {

    namespace pipenetwork {

        class PipeNetwork {

        public:
            explicit PipeNetwork(const std::string& inputFilename);

            std::vector<double> computeFluxes();

        private:
            std::string pFilename;
            Parameters parameters;

            double permeability(double node1x, double node1y, double node2x, double node2y, double diameter);
            double permeability(const Node* pNode1, const Node* pNode2, double diameter);
        };
    } // namespace pipenetwork
} // namespace cie

#endif //LINALG_PIPENETWORK_HPP
