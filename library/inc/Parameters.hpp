//
// Created by Jakob Erpf on 23.12.19.
//

#ifndef LINALG_PARAMETERS_HPP
#define LINALG_PARAMETERS_HPP

class Parameters;

namespace cie {

    namespace pipenetwork {

        class Parameters {

        public:
            Parameters();

            double getPi() const;

            double getG() const;

        private:
            double pi;
            double g;
        };
    } // namespace pipenetwork
} // namespace cie



#endif //LINALG_PARAMETERS_HPP
