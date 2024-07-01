/*
 * pin.cpp
 *
 *  Created on: Feb 16, 2009
 *      Author: rohit
 */

#include "pin.hpp"
#include "net.hpp"
#include "gate.hpp"
#include "manager.hpp"
#include "node.hpp"

namespace nl {

std::ostream&
Pin::instance(std::ostream& o) const {
	if (!net()) {
		return o;
	}
	o << "." << name() << "(";
	net()->instance(o);
	o << ")";
	return o;
}

std::ostream&
Pin::direction(std::ostream& o) const {
	switch (type()) {
	case Input:
		o << "output "; // directions reversed for verilog
		break;
	case Output:
		o << "input "; // directions reversed for verilog
		break;
	case Inout:
		o << "inout ";
		break;
	case Unknown:
		o << "unknown ";
		break;
	default:
		break;
	}
	return o;
}

std::ostream&
Pin::instance(const std::string& pre, std::ostream& o) const {
	if (!net()) {
		return o;
	}
	o << pre;
	o << "." << name() << "(";
	net()->instance(o);
	o << ")";
	return o;
}

// ROBDD functions
const bdd::NodeBase*
Pin::createBdd(bdd::Manager& m, unsigned int index) {
	bdd(m.buildVar(index));
	return bdd();
}

bool
Pin::robdd(bdd::Manager& m, _PinQueue& _q) {
	switch (type()) {
	case Input:
		gate()->robdd(m, _q);
		break;
	case Output:
		if (net())
			net()->robdd(m, _q);
		break;
	case Inout:
		break;
	case Unknown:
		break;
	default:
		break;
	}
	return false;
}

std::ostream&
Pin::robdd(std::ostream& o) const {
	if (bdd()) {
		o << *bdd();
	}
	return o;
}

bool
Pin::loops(std::ostream& o) {
	bool hasloops = false;
	switch (type()) {
	case Input:
		if (hasLoop()) {
			const BaseGate* g = gate();
			// already visited! Message
			o << "WARNING: Found loop at gate ";
			o << g->name() << "(";
			instance(o);
			o << ")" << std::endl;
			hasloops = true;
		}
		incrLoopcount();
		if (!hasloops)
			gate()->loops(o);
		break;
	case Output:
		if (net())
			hasloops |= net()->loops(o);
		break;
	case Inout:
		break;
	case Unknown:
		break;
	default:
		break;
	}
	return hasloops;
}


}
