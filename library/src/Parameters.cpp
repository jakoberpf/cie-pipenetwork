//
// Created by Jakob Erpf on 23.12.19.
//

#include "inc/Parameters.hpp"

namespace cie {
    namespace pipenetwork {
        Parameters::Parameters() {
            pi = 3.14159265358979323846;
            g = 9.81;
        }

        double Parameters::getPi() const {
            return pi;
        }

        double Parameters::getG() const {
            return g;
        }
    }
}

