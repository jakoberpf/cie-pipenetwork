//
// Created by Jakob Erpf on 08.12.19.
//

#include <fstream>
#include <vector>
#include <iostream>
#include <inc/Node.hpp>
#include <inc/Tube.hpp>
#include <cmath>
#include <inc/linalg.hpp>

#include "inc/PipeNetwork.hpp"

namespace cie {

    namespace pipenetwork {

        PipeNetwork::PipeNetwork(const std::string& inputFilename) {
            pFilename = inputFilename;
            parameters = Parameters();
        }

        std::vector<double> PipeNetwork::computeFluxes() {
            int numberOfNodes;
            int numberOfTubes;
            std::vector<Node> nodeList;
            std::vector<Tube> tubeList;

            // Read input file
            std::ifstream inputStream;
            inputStream.open(pFilename, std::ios_base::in);

            if (!inputStream)
                std::cerr << "Input file can not be opened\n";
            else {
                std::ofstream Zieldatei("output_" + pFilename);
                if (!Zieldatei)
                    std::cerr << "Output file can not be opened\n";
                else {
                    inputStream >> numberOfNodes;
                    std::cout << "numberOfNodes: ";
                    std::cout << numberOfNodes << std::endl;
                    std::cout << " " << std::endl;

                    inputStream >> numberOfTubes;
                    std::cout << "numberOfTubes: ";
                    std::cout << numberOfTubes << std::endl;
                    std::cout << " " << std::endl;

                    std::cout << "List of Nodes" << std::endl;
                    nodeList.reserve( numberOfNodes );
                    for(int i = 0; i < numberOfNodes; ++i ) {
                        double x;
                        double y;
                        double flow;
                        inputStream >> x;
                        inputStream >> y;
                        inputStream >> flow;
                        nodeList.push_back(Node(x,y,flow,i));
                        std::cout << "Node# -> ";
                        std::cout << nodeList[i].id();
                        std::cout << "  X: ";
                        std::cout << x;
                        std::cout << "  Y: ";
                        std::cout << y;
                        std::cout << "  Flow: ";
                        std::cout << flow << std::endl;
                    }
                    std::cout << " " << std::endl;

                    std::cout << "List of Tubes" << std::endl;
                    tubeList.reserve( numberOfTubes );
                    for(int i = 0; i < numberOfTubes; ++i ) {
                        int Node1Id;
                        int Node2Id;
                        Node* node1;
                        Node* node2;
                        double diameter;
                        inputStream >> Node1Id;
                        inputStream >> Node2Id;
                        inputStream >> diameter;
                        node1 = &nodeList[Node1Id];
                        node2 = &nodeList[Node2Id];
                        tubeList.push_back(Tube(node1, node2, diameter));
                        std::cout << "Node1: ";
                        std::cout << node1->id();
                        std::cout << "  Node2: ";
                        std::cout << node2->id();
                        std::cout << "  Diameter: ";
                        std::cout << diameter << std::endl;
                    }
                    std::cout << " " << std::endl;

                    // B matrix initialisation
                    cie::linalg::Matrix B = cie::linalg::Matrix(numberOfNodes, numberOfNodes, 0);
                    std::cout << "Created zeroed matrix with size: ";
                    std::cout << B.size1();
                    std::cout << "x";
                    std::cout << B.size2() << std::endl;
                    std::cout << " " << std::endl;

                    // Permeability vector initialisation
                    linalg::Vector permeabilitys;

                    // B matrix calculation
                    std::cout << "Calculated permeability's: ";
                    for(int i = 0; i < numberOfTubes; ++i ) {
                        Tube* currentTube = &tubeList[i];
                        permeabilitys.push_back(permeability(currentTube->node1()->x(), currentTube->node1()->y(), currentTube->node2()->x(), currentTube->node2()->y(), currentTube->diameter()));
//                        permeability(currentTube->node1(), currentTube->node2(), currentTube->diameter());
                        std::cout << std::setw(10) << permeabilitys[i];
                        B.operator()(currentTube->node1()->id(), currentTube->node1()->id()) += permeabilitys[i];
                        B.operator()(currentTube->node2()->id(), currentTube->node2()->id()) += permeabilitys[i];
                        B.operator()(currentTube->node1()->id(), currentTube->node2()->id()) -= permeabilitys[i];
                        B.operator()(currentTube->node2()->id(), currentTube->node1()->id()) -= permeabilitys[i];
                    }
                    std::cout << " " << std::endl;
                    std::cout << " " << std::endl;

                    // B matrix cout
                    std::cout << "B Matrix looks like (without boundary constraints): " << std::endl;
                    for (int j = 0; j < B.size1(); ++j) {
                        for (int i = 0; i < B.size2(); ++i) {
                            std::cout << std::setw(10) << B.operator()(j, i);
                        }
                        std::cout << " " << std::endl;
                    }
                    std::cout << " " << std::endl;

                    // Q vector setting
                    linalg::Vector Q(numberOfNodes);
                    for (int i = 0; i < numberOfNodes; ++i) {
                        Q[i] -= nodeList[i].flow();
                    }

                    // Q vector cout
                    std::cout << "Q Vector looks like: " << std::endl;
                    for (int i = 0; i < numberOfNodes; ++i) {
                        std::cout << std::setw(5) << Q[i];
                    }
                    std::cout << " " << std::endl;
                    std::cout << " " << std::endl;

                    // Boundary Conditions
                    for (int i = 0; i < numberOfNodes; ++i) {
                        B.operator()(i, 0) = 0;
                        B.operator()(0, i) = 0;
                    }
                    B.operator()(0, 0) = 1;
                    Q[0] = 0;

                    // B matrix cout
                    std::cout << "B Matrix looks like (with boundary constraints): " << std::endl;
                    for (int j = 0; j < B.size1(); ++j) {
                        for (int i = 0; i < B.size2(); ++i) {
                            std::cout << std::setw(10) << B.operator()(j, i);
                        }
                        std::cout << " " << std::endl;
                    }
                    std::cout << " " << std::endl;

                    // Q vector cout
                    std::cout << "Q Vector looks like: " << std::endl;
                    for (int i = 0; i < numberOfNodes; ++i) {
                        std::cout << std::setw(5) << Q[i];
                    }
                    std::cout << " " << std::endl;
                    std::cout << " " << std::endl;

                    // Solve equation system
                    linalg::Vector h = linalg::solve(B, Q);

                    // h vector cout
                    std::cout << "h Vector looks like: " << std::endl;
                    for (double i : h) {
                        std::cout << std::setw(10) << i;
                    }
                    std::cout << " " << std::endl;
                    std::cout << " " << std::endl;

                    // Postprocessing fluxes
                    linalg::Vector q(numberOfTubes);
                    for (int i = 0; i < numberOfTubes; ++i) {
                        double h1 = h[tubeList[i].node1()->id()];
                        double h2 = h[tubeList[i].node2()->id()];
                        q[i] = permeabilitys[i] * (h1 - h2);
                    }

                    // q vector cout
                    std::cout << "q Vector looks like: " << std::endl;
                    for (double i : q) {
                        std::cout << std::setw(10) << i;
                    }
                    std::cout << " " << std::endl;
                    std::cout << " " << std::endl;

                    return q;
                }
            }
            return std::vector<double>();
        }

        // Native Implementation of permeability()
        double PipeNetwork::permeability(double node1x, double node1y, double node2x, double node2y, double diameter) {
            double length = sqrt(pow(node1x - node2x, 2) + pow(node1y - node2y, 2));
            return parameters.getPi() * parameters.getG() * std::pow(diameter, 4) / (128 * 1e-6 * length);;
        }

        // Instance Implementation of permeability()
        double PipeNetwork::permeability(const Node* pNode1, const Node* pNode2, double diameter) {
            double length = sqrt(pow(pNode1->x() - pNode2->x(), 2) + pow(pNode1->y() - pNode2->y(), 2));
            return parameters.getPi() * parameters.getG() * std::pow( diameter, 4 ) / ( 128 * 1e-6 * length );;
        }
    } // namespace pipenetwork
} // namespace cie

