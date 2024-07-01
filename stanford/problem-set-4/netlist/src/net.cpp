/*
 * net.cpp
 *
 *  Created on: Feb 16, 2009
 *      Author: rohit
 */

#include "pin.hpp"
#include "net.hpp"
#include "gate.hpp"
#include "port.hpp"
#include "netlist.hpp"
#include <iostream>

namespace nl {

bool
Net::isOne() const {
	const Pin* pn = input();
	if (pn) {
		const BaseGate* g = pn->gate();
		const Netlist* nlst = g->netlist();
		if (nlst->one() == g)
			return true;
	}
	return false;
}

bool
Net::isZero() const {
	const Pin* pn = input();
	if (pn) {
		const BaseGate* g = pn->gate();
		const Netlist* nlst = g->netlist();
		if (nlst->zero() == g)
			return true;
	}
	return false;
}

void
Net::connect(Pin* p) {
	const Pin* cp = p;
	p->connect(this);

	if (cp->type() == Pin::Output) {
		input(p);
	}
	else {
		outputs().push_back(p);
	}
}

void
Net::connect(Pin* p, bool is_driver) {
	p->connect(this, is_driver);
	if (is_driver) {
		input(p);
	}
	else {
		outputs().push_back(p);
	}
}

void
Net::connect(Port* po, Pin::Type t) {
	po->connect(this, t);
}

void
Net::disconnect(Pin* p) {
	p->disconnect(this);
	if (p == input()) {
		input(0);
	}
	else {
		for (_Pins::iterator itr = outputs().begin();
			itr != outputs().end(); itr++) {
			if (p == (*itr)) {
				outputs().erase(itr);
				break;
			}
		}
	}
}

void
Net::import(Net* np) {
	while (!np->outputs().empty()) {
		Pin* pn = np->outputs().front();
		//np->outputs().pop_front();
		np->disconnect(pn);
		connect(pn);
	}
}

std::ostream&
Net::wire(std::ostream& o) const {
	if (isConstant()/* || isConstantStr()*/) {
		return o;
	}
	if (input()) {
		const Pin* pn = input();
		const BaseGate* g = pn->gate();
		if (g->name() == name()) {
			return o;
		}
	}
	for (_Pins::const_iterator itr = outputs().begin();
		itr != outputs().end(); itr++) {
		const Pin* pn = (*itr);
		const BaseGate* g = pn->gate();
		if (g->name() == name()) {
			return o;
		}
	}
	o << "wire " << name() << ";" << std::endl;
	return o;
}

std::ostream&
Net::instance(std::ostream& o) const {
	o << name();
	return o;
}

std::istream&
Net::instance(std::istream& i) {
	char c;
	std::string s;
	while (i >> c) {
		if (c == ',') {
			break;
		}
		if (c == ';') {
			i.putback(c);
			break;
		}
		s += c;
	}
	name() = s;
	return i;
}

bool
Net::check(std::ostream& o) const {
	bool baddesign = false;
	if (isConstant())
		return baddesign;

	const Pin* pi = input();
	const Pin* po = 0;
	for (_Pins::const_iterator itr = outputs().begin();
		itr != outputs().end(); itr++) {
		po = (*itr);
	}
	if (!pi) {
		// No driver pin
		o << "WARNING: Dangling net \"";
		o << name();
		o << "\" has no driver";
		o << std::endl;
		baddesign = true;
	}
	if (!po) {
		// Drives nothing
		o << "WARNING: Dangling net \"";
		o << name();
		o << "\" drives nothing";
		o << std::endl;
		baddesign = true;
	}
	return baddesign;
}

bool
Net::loops(std::ostream& o) const {
	bool hasloops = false;
	for (_Pins::const_iterator itr = outputs().begin();
		itr != outputs().end(); itr++) {
		Pin* po = const_cast<Pin*>(*itr);
		hasloops |= po->loops(o);
	}
	return hasloops;
}

bool
Net::robdd(bdd::Manager& m, _PinQueue& _q) {
	for (_Pins::iterator itr = outputs().begin();
		itr != outputs().end(); itr++) {
		Pin* pn = (*itr);
		_q.push_back(pn);
	}
	return false;
}

bool
Net::requeue(_PinQueue& _q) {
	for (_Pins::iterator itr = outputs().begin();
		itr != outputs().end(); itr++) {
		Pin* pn = (*itr);
		const Pin* cpn = pn;
		const BaseGate* cg = const_cast<BaseGate*>(cpn->gate());
		const Pin* po = cg->output();
		if (po && po->net()) { // not disconnected nets.
			BaseGate* g = const_cast<BaseGate*>(cg);
			g->decrPropagate();
			_q.push_back(pn);
		}
	}
	return false;
}

bool
Net::cleanup() {
	if (outputs().size())
		return false;

	Pin* p = input();
	if (p) {
		disconnect(p);
	}
	return true;
}

}
