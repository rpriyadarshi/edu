/*
 * port.cpp
 *
 *  Created on: Feb 21, 2009
 *      Author: rohit
 */

#include <iostream>
#include "pin.hpp"
#include "net.hpp"
#include "port.hpp"

namespace nl {

Pin*
Port::connect(Net* n, Pin::Type t) {
	Pin* p = 0;
	switch (t) {
	case Pin::Output:
		p = createOutputPin(N_pi); 	// output pin is actually driven
		break;
	case Pin::Input:
		p = createInputPin(N_po);	// input pin is actually a driver
		break;
	case Pin::Unknown:
	case Pin::Inout:
	default:
		break;
	};
	n->connect(p);
	return p;
}

std::ostream&
Port::instance(std::ostream& o) const {
	o << name();
	return o;
}

std::ostream&
Port::declaration(std::ostream& o) const {
	if (output()) {
		const Pin* pn = output();
		pn->direction(o);
	}
	else {
		for (_Pins::const_iterator itr = inputs().begin();
			itr != inputs().end(); itr++) {
			const Pin* pn = (*itr);
			pn->direction(o);
			break;
		}
	}
	o << name() << ";" << std::endl;
	return o;
}

std::istream&
Port::instance(std::istream& i) {
	char c;
	std::string s;
	while (i >> c) {
		if (c == '(') {
			continue;
		}
		else if (c == ',' || c == ')') {
			break;
		}
		s += c;
	}
	name() = s;
	return i;
}

std::istream&
Port::declaration(std::istream& i) const {
	return i;
}


std::ostream&
Port::module(std::ostream& o) const {
	o << "<invalid>";
	return o;
}

const Pin*
Port::_pin() const {
	const Pin* pn = 0;
	if (output()) {
		pn = output();
	}
	else {
		for (_Pins::const_iterator itr = inputs().begin();
			itr != inputs().end(); itr++) {
			pn = (*itr);
			break;
		}
	}
	return pn;
}

bool
Port::robdd(bdd::Manager& m, _PinQueue& _q) {
	return false;
}

bool
Port::buildrobdd(bdd::Manager& m) {
	if (output()) {
		output()->createBdd(m, index());
		return true;
	}

	return false;
}

bool
Port::finalrobdd(bdd::Manager& m, _PinQueue& _q) {
	return false;
}

std::ostream&
Port::robdd(std::ostream& o) const {
	const Pin* pn = pin();
	pn->robdd(o);
	return o;
}

}
