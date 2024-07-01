/*
 * node.cpp
 *
 *  Created on: Feb 1, 2009
 *      Author: rohit rpiyadarshi
 */

#include "node.hpp"

namespace bdd {

std::ostream&
Sink::dump(std::ostream& o) const {
	//o << "SINK";
	o << "SINK_" << this;
	o << " (" << value() << ")";
	return o;
}

std::ostream&
Node::dump(std::ostream& o) const {
	//o << "NODE";
	o << "NODE_" << this;
	o << " (" << index() << ", "
		<< *one() << ", " << *zero() << ")";
	return o;
}

}
