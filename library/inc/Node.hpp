//
// Created by Jakob Erpf on 08.12.19.
//

#ifndef LINALG_NODE_HPP
#define LINALG_NODE_HPP

class Node;

namespace cie {

    namespace pipenetwork {

        class Node {

        public:
            Node(double x, double y, double flow, int id);

            double x() const;

            double y() const;

            double flow() const;

            int id() const;

        private:
            double x_coordinate;
            double y_coordinate;
            double flow_rate;
            int identifier;

        };
    } // namespace pipenetwork
} // namespace cie

#endif //LINALG_NODE_HPP
