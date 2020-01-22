//
// Created by Jakob Erpf on 08.12.19.
//

#include "inc/Node.hpp"

namespace cie {

    namespace pipenetwork {

        Node::Node(double in_x, double in_y, double in_flow, int in_id) {
            x_coordinate = in_x;
            y_coordinate = in_y;
            flow_rate = in_flow;
            identifier = in_id;
        }

        double Node::x() const {
            return x_coordinate;
        }

        double Node::y() const {
            return y_coordinate;
        }

        double Node::flow() const {
            return flow_rate;
        }

        int Node::id() const {
            return identifier;
        }

    }
}
