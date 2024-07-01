/*
 * gate.cpp
 *
 *  Created on: Feb 16, 2009
 *      Author: rohit
 */

#include "pin.hpp"
#include "gate.hpp"
#include "net.hpp"
#include "netlist.hpp"
#include "node.hpp"
#include "manager.hpp"

namespace nl {

Pin*
BaseGate::createPin(const std::string& name, enum Pin::Type t) {
	return new Pin(name, t, this);
}

Pin*
BaseGate::createInputPin(const std::string& n) {
	Pin* p = createPin(n, Pin::Input);
	inputs().push_back(p);
	return p;
}

Pin*
BaseGate::createOutputPin(const std::string& n) {
	output(createPin(n, Pin::Output));
	return output();
}

const Pin*
BaseGate::findPin(const std::string& n) const {
	if (output() && output()->name() == n) {
		return output();
	}
	for (_Pins::const_iterator itr = inputs().begin();
		itr != inputs().end(); itr++) {
		const Pin* p = (*itr);
		if (p->name() == n) {
			return p;
		}
	}
	return 0;
}

std::ostream&
BaseGate::instance(std::ostream& o) const {
	module(o);
	o << " " << name() << "(";
	if (output()) {
		output()->instance(o);
	}
	_Pins::const_iterator itr = inputs().begin();
	if (itr != inputs().end()) {
		if (output()) {
			(*itr)->instance(", ", o);
		}
		else {
			(*itr)->instance(o);
		}
		itr++;
	}
	for (; itr != inputs().end(); itr++) {
		(*itr)->instance(", ", o);
	}
	o << ");" << std::endl;
	return o;
}

std::istream&
BaseGate::name(std::istream& i) {
	char c;
	std::string s;
	while (i >> c) {
		if (c == '(') {
			i.putback(c);
			break;
		}
		s += c;
	}
	name() = s;
	return i;
}

std::istream&
BaseGate::instance(std::istream& i) {
	name(i);
	char c;
	std::string s;
	const Pin* pn = 0;
	while (i >> c) {
		if (c == '(') {
			if (!s.empty()) {
				pn = findPin(s);
			}
			s.clear();
			continue;
		}
		else if (c == '.') {
			s.clear();
			continue;
		}
		else if (c == ',') {
			continue;
		}
		else if (c == ')') {
			if (s.empty()) {
				continue;
			}
			Netlist* nlst = netlist();
			Net* np = nlst->findNet(s);
			if(!np) {
				np = nlst->createNet(s);
			}
			if (pn) {
				np->connect(const_cast<Pin*>(pn));
			}

			s.clear();
			continue;
		}
		else if (c == ';') {
			break;
		}
		s += c;
	}
	return i;
}

std::ostream&
BaseGate::checkMsg(std::ostream& o, const Pin* pn) const {
	o << "WARNING: Unconnected pin \"";
	o << pn->name();
	o << "\", in instance ";
	instance(o);
	return o;
}

bool
BaseGate::check(std::ostream& o) const {
	bool baddesign = false;
	const Pin* po = output();
	const Net* cn = po->net();
	if (!cn) {
		// unconnected pin
		checkMsg(o, po);
		baddesign = true;
	}
	for (_Pins::const_iterator itr = inputs().begin();
		itr != inputs().end(); itr++) {
		const Pin* pi = (*itr);
		cn = pi->net();
		if (!cn) {
			// unconnected pin
			checkMsg(o, pi);
			baddesign = true;
		}
	}
	return baddesign;
}

bool
BaseGate::loops(std::ostream& o) const {
	bool hasloops = false;
	Pin* p = const_cast<Pin*>(output());
	if (p) {
		hasloops |= p->loops(o);
	}
	return hasloops;
}

void
BaseGate::resetLoopcount() const {
	Pin* p = const_cast<Pin*>(output());
	if (p) {
		p->resetLoopcount();
	}
	for (_Pins::const_iterator itr = inputs().begin();
		itr != inputs().end(); itr++) {
		p = const_cast<Pin*>(*itr);
		p->resetLoopcount();
	}
}

bool
BaseGate::robdd(bdd::Manager& m, _PinQueue& _q) {
	decrPropagate();
	if (canPropagate()) {
		Pin* po = output();
		const Pin* cpo = po;
		if (cpo && cpo->net()) {
			buildrobdd(m);
			optimize(m, _q);
			_q.push_back(po);
			return true;
		}
	}
	return false;
}

bool
BaseGate::buildrobdd(bdd::Manager& m) {
	return false;
}

bool
BaseGate::initialrobdd(bdd::Manager& m, _PinQueue& _q) {
	if (output()) {
		buildrobdd(m);
		_q.push_back(output());
		return true;
	}

	return false;
}

std::ostream&
BaseGate::reasonMsg(std::ostream& o, const Pin* pi) {
	const Pin* po = output();

	o << "Found MATCHING BDD: ";
	module(o);
	o << " ";
	o << name() << " input pin (";
	pi->instance(o);
	o << ") output pin (";
	po->instance(o);
	o << ")" << std::endl;
	return o;
}

bool
BaseGate::optimize(bdd::Manager& m, _PinQueue& _q) {
#if 0
	bool foundone = false;
	Pin* po = output();
	const Pin* cpo = po;
	//const Net* cno = cpo->net();
	//Net* no = const_cast<Net*>(cno);

	// Check for matching bdd
	for (_Pins::iterator itr = inputs().begin();
		itr != inputs().end(); itr++) {
		const Pin* cpi = (*itr);
		const Net* cni = cpi->net();
		const Pin* cpd = cni->input();

		if (cpo->bdd() == cpd->bdd()) {
			//np->disconnect(const_cast<Pin*>(pn));
			//nz->import(np);
			//nz->requeue(_q);
			//return true;
			foundone = true;
			reasonMsg(std::cerr, cpi);
		}
	}
	if (foundone) {
		for (itr = inputs().begin();
			itr != inputs().end(); itr++) {
			Pin* pi = (*itr);
			if (po->bdd() != pi->bdd()) {
			}
		}
	}
	else {
		foundone |= netlist()->optimize(m, _q, po);
	}

	return foundone;
#else
	Pin* po = output();
	return netlist()->optimize(m, _q, po);
#endif
}

std::ostream&
BaseGate::robdd(std::ostream& o) const {
	if (output()) {
		output()->robdd(o);
	}
	return o;
}

const bdd::NodeBase*
BaseGate::bdd(unsigned int idx) const {
	const Pin* pn = inputs()[idx];
	const Net* np = pn->net();
	const Pin* po = np->input();
	return po->bdd();
}

bool
BaseGate::cleanup() {
	const Pin* po = output();
	if (po->net()) {
		return false;
	}
	// not driving anything
	for (_Pins::iterator itr = inputs().begin();
		itr != inputs().end(); itr++) {
		Pin* p = (*itr);
		const Pin* cp = p;
		const Net* cn = cp->net();
		Net* n = const_cast<Net*>(cn);
		if (n) {
			n->disconnect(p);
		}
	}
	return true;
}

bool
Not::buildrobdd(bdd::Manager& m) {
	Pin* po = output();
	po->bdd(m.iteNot(bdd(0)));
	return false;
}

bool
And2::buildrobdd(bdd::Manager& m) {
	Pin* po = output();
	po->bdd(m.iteAnd(bdd(0), bdd(1)));
	return false;
}

bool
Or2::buildrobdd(bdd::Manager& m) {
	Pin* po = output();
	po->bdd(m.iteOr(bdd(0), bdd(1)));
	return false;
}

bool
Xor2::buildrobdd(bdd::Manager& m) {
	Pin* po = output();
	po->bdd(m.iteXor(bdd(0), bdd(1)));
	return false;
}

bool
Zero::buildrobdd(bdd::Manager& m) {
	Pin* po = output();
	//Pin* pn = inputs()[0];
	po->bdd(m.zero());
	return false;
}

bool
One::buildrobdd(bdd::Manager& m) {
	Pin* po = output();
	//Pin* pn = inputs()[0];
	po->bdd(m.one());
	return false;
}

bool
Xor2::optimize(bdd::Manager& m, _PinQueue& _q) {
	bool foundone = false;
	const Netlist* nlst = netlist();
	const BaseGate* z = nlst->zero();
	const BaseGate* o = nlst->one();
	const Pin* pz = z->output();
	const Pin* po = o->output();
	Net* nz = const_cast<Net*>(pz->net());
	Net* no = const_cast<Net*>(po->net());
	assert(no);
	assert(nz);

	const Pin* pn = output();
	Net* np = const_cast<Net*>(pn->net());

	// Check for matching bdd
	_Pins::iterator itr;
	for (itr = inputs().begin();
		itr != inputs().end(); itr++) {
		const Pin* pi = (*itr);
		const Net* ni = pi->net();
		const Pin* pd = ni->input();

		const bdd::NodeBase* bdd = pd->bdd();
		if (bdd == m.one()) {
			reasonMsg(std::cerr, pi);
			// remove existing connections.
			np->disconnect(const_cast<Pin*>(pn));
			const_cast<Net*>(ni)->disconnect(const_cast<Pin*>(pi));

			// Invent a name.
			std::string str("not_");
			str += name();
			BaseGate* g = const_cast<Netlist*>(nlst)->createGate(str, BaseGate::Not);
			const BaseGate* cg = g;

			// Connect to new gate.
			np->connect(const_cast<Pin*>(cg->output()));
			const_cast<Net*>(ni)->connect(const_cast<Pin*>(cg->firstInput()));
			_q.push_back(const_cast<Pin*>(cg->output()));
			foundone = true;
		}
	}

	if (foundone) {
		for (itr = inputs().begin();
			itr != inputs().end(); itr++) {
			const Pin* pi = (*itr);
			const Net* ni = pi->net();
			// remove existing connections.
			if (ni) {
				const_cast<Net*>(ni)->disconnect(const_cast<Pin*>(pi));
			}
		}
	}
	else {
		foundone |= netlist()->optimize(m, _q, pn);
	}

	return foundone;
}

}
