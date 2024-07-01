/*
 * netlist.cpp
 *
 *  Created on: Feb 16, 2009
 *      Author: rohit
 */

#include "gate.hpp"
#include "net.hpp"
#include "pin.hpp"
#include "port.hpp"
#include "netlist.hpp"

// ROBDD includes
#include "manager.hpp"
#include "node.hpp"
#include "boolfunc.hpp"

namespace nl {

Netlist::Netlist() {
	buildConstants();
}

Netlist::Netlist(const std::string& n) :
	m_name(n) {
	buildConstants();
}

Netlist::~Netlist() {
	delete zero();
	delete one();
}

void
Netlist::buildConstants() {
	zero(createGate(N_Zero, BaseGate::Zero));
	const BaseGate* z = zero();
	const Pin* pz = z->output();
	Net* nz = createNet(N_ZeroNet);
	nz->connect(const_cast<Pin*>(pz));

	one(createGate(N_One, BaseGate::One));
	const BaseGate* o = one();
	const Pin* po = o->output();
	Net* no = createNet(N_OneNet);
	no->connect(const_cast<Pin*>(po));
}
Port*
Netlist::createPort(unsigned int& idx) {
	return new Port(this, idx);
}

Port*
Netlist::createPort(const std::string& n) {
	Port* p = new Port(n, this);
	ports().push_back(p);
	return p;
}

BaseGate*
Netlist::createGate(BaseGate::Type t) {
	BaseGate* g = 0;
	switch (t) {
	case BaseGate::Not:
		g = new Not(this); break;
	case BaseGate::And2:
		g = new And2(this); break;
	case BaseGate::Or2:
		g = new Or2(this); break;
	case BaseGate::Xor2:
		g = new Xor2(this); break;
	case BaseGate::Zero:
		g = new Zero(this); break;
	case BaseGate::One:
		g = new One(this); break;
	default:
		return 0; break;
	};

	return g;
}

BaseGate*
Netlist::createGate(const std::string& n, BaseGate::Type t) {
	BaseGate* g = 0;
	switch (t) {
	case BaseGate::Not:
		g = new Not(n, this); break;
	case BaseGate::And2:
		g = new And2(n, this); break;
	case BaseGate::Or2:
		g = new Or2(n, this); break;
	case BaseGate::Xor2:
		g = new Xor2(n, this); break;
	case BaseGate::Zero:
		g = new Zero(n, this); break;
	case BaseGate::One:
		g = new One(n, this); break;
	default:
		return 0; break;
	};

	gates().insert(g->pair());
	return g;
}

Net*
Netlist::createNet() {
	return new Net();
}

Net*
Netlist::createNet(Port* p, Pin::Type t) {
	const Port* cp = p;
	Net* np = new Net(cp->name());
	nets().insert(np->pair());
	np->connect(p, t);
	return np;
}

Net*
Netlist::createNet(const std::string& n) {
	Net* np = new Net(n);
	nets().insert(np->pair());
	return np;
}

BaseGate*
Netlist::instantiateGate(BaseGate::Type t, const std::string& n) {
	return createGate(n, t);
}

void
Netlist::deleteGate(const BaseGate* g) {
	gates().erase(g->name());
}

BaseGate*
Netlist::findGate(const std::string& n) {
	_Gates::iterator itr = gates().find(n);
	return itr != gates().end()?(*itr).second:0;
}

// Net functions
Net*
Netlist::instantiateNet(const std::string& n) {
	return createNet(n);
}

void
Netlist::deleteNet(const Net* n) {
	nets().erase(n->name());
}

Net*
Netlist::findNet(const std::string& n) {
	_Nets::iterator itr = nets().find(n);
	return itr != nets().end()?(*itr).second:0;
}

Port*
Netlist::findPort(const std::string& n) {
	for (_Ports::iterator itrP = ports().begin();
		itrP != ports().end(); itrP++) {
		Port* p = (*itrP);
		const Port* cp = p;
		if (cp->name() == n)
			return p;
	}
	return 0;
}

void
Netlist::connectNet(Net* n, Pin* p,	bool is_driver) {
	n->connect(p, is_driver);
}

void
Netlist::disconnectNet(Net* n, Pin* p) {
	n->disconnect(p);
}

// Pin functions
Pin*
Netlist::getOutputPin(const BaseGate* g ) {
	return const_cast<Pin*>(g->output());

}

Pin*
Netlist::getInputPin(const BaseGate* g, const std::string& n ) {
	return const_cast<Pin*>(g->findPin(n));
}

Pin*
Netlist::findDriverPin(const Net* n) {
	return const_cast<Pin*>(n->input());
}

unsigned int
Netlist::findNumDrivenPins(const Net* n) {
	return n->outputs().size();
}

Pin*
Netlist::findDrivenPin(const Net* n, int pin_num) {
	return n->outputs()[pin_num];
}

// Debugging functions
void
Netlist::printNetlist() {
	verilog(std::cout);
}

std::ostream&
Netlist::verilog(std::ostream& o) const {
	bool addnl = false;
	// Module definition
	o << "module " << name() << " (";
	// Ports
	_Ports::const_iterator itrP = ports().begin();
	if (itrP != ports().end()) {
		Port* p = (*itrP);
		p->instance(o);
		itrP++;
	}
	for (; itrP != ports().end(); itrP++) {
		o << ", ";
		Port* p = (*itrP);
		p->instance(o);
	}
	o << ");" << std::endl;

	// Port declarations
	for (itrP = ports().begin(); itrP != ports().end(); itrP++) {
		Port* p = (*itrP);
		p->declaration(o);
		addnl = true;
	}
	if (addnl) {
		o << std::endl;
		addnl = false;
	}

	// Wires
	for (_Nets::const_iterator itrN = nets().begin();
		itrN != nets().end(); itrN++) {
		((*itrN).second)->wire(o);
		addnl = true;
	}
	if (addnl) {
		o << std::endl;
		addnl = false;
	}

	// Instances
	for (_Gates::const_iterator itrG = gates().begin();
	itrG != gates().end(); itrG++) {
		((*itrG).second)->instance(o);
		addnl = true;
	}
	if (addnl) {
		o << std::endl;
		addnl = false;
	}

	o << "endmodule" << std::endl;
	o << std::endl;
	return o;
}

std::istream&
Netlist::verilog(std::istream& i, unsigned int& idx) {
	std::string token;
	while (i >> token) {
		if (token == "//") {
			comment(i);
		}
		if (token == "module") {
			name(i);
			ports(i, idx);
		}
		if (token == "input") {
			type(i, Pin::Output); // These drive the net
		}
		if (token == "output") {
			type(i, Pin::Input); // These are driven by the net
		}
		if (token == "inout") {
			type(i, Pin::Inout);
		}
		if (token == "wire") {
			wires(i);
		}
		if (token == "assign") {
			assign(i);
		}
		instances(i);
		if (token == "endmodule") {
			break;
		}
	}
	return i;
}

std::istream&
Netlist::name(std::istream& i) {
	char c;
	std::string s;
	while (i >> c) {
		if (c == '(') {
			i.putback(c);
			break;
		}
		else if (c == ')' || c == ';') {
			break;
		}
		s += c;
	}
	name() = s;
	return i;
}

std::istream&
Netlist::ports(std::istream& i, unsigned int& idx) {
	while (i) {
		Port* p = createPort(idx);
		p->instance(i);
		if (p->valid()) {
			ports().push_back(p);
		}
		else {
			delete p;
		}
		char c;
		i >> c;
		if (c == ';') {
			break;
		}
		else {
			i.putback(c);
		}
	}
	return i;
}

std::istream&
Netlist::type(std::istream& i, Pin::Type t) {
	char c;
	std::string s;
	Port* p = 0;
	while (i >> c) {
		if (c == ',') {
			p = findPort(s);
			createNet(p, t);
			s.clear();
			continue;
		}
		if (c == ';') {
			p = findPort(s);
			createNet(p, t);
			s.clear();
			break;
		}
		s += c;
	}
	return i;
}

std::istream&
Netlist::wires(std::istream& i) {
	while (i) {
		Net* np = createNet();
		np->instance(i);
		if (np->valid()) {
			nets().insert(np->pair());
		}
		else {
			delete np;
		}
		char c;
		i >> c;
		if (c == ';') {
			break;
		}
		else {
			i.putback(c);
		}
	}
	return i;
}

std::istream&
Netlist::assign(std::istream& i) {
	char c;
	std::string s;
	Net* n = 0;
	while (i >> c) {
		if (c == '=') {
			n = findNet(s);
			if (!n) {
				n = createNet(s);
			}
			s.clear();
			continue;
		}
		if (c == ';') {
			std::ostream& o = std::cerr;
			//n->assign(s);
			o << "TBD: assign ";
			n->instance(o);
			o << " = "<< s << ";" << std::endl;
			s.clear();
			break;
		}
		s += c;
	}
	return i;
}

std::istream&
Netlist::instances(std::istream& i) {
	std::string gate;
	while (i>>gate) {
		BaseGate* g = 0;
		if (gate == N_Not) {
			g = createGate(BaseGate::Not);
		}
		else if (gate == N_And2) {
			g = createGate(BaseGate::And2);
		}
		else if (gate == N_Or2) {
			g = createGate(BaseGate::Or2);
		}
		else if (gate == N_Xor2) {
			g = createGate(BaseGate::Xor2);
		}
		else if (gate == N_Zero) {
			g = createGate(BaseGate::Zero);
		}
		else if (gate == N_One) {
			g = createGate(BaseGate::One);
		}
		else {
			for (size_t j = gate.size(); j > 0; j--) {
				i.putback(gate[j-1]);
			}
			break;
		}
		if (g) {
			g->instance(i);
			if (g->valid()) {
				gates().insert(g->pair());
			}
			else {
				delete g;
			}
			char c;
			i >> c;
			if (c == ';') {
				break;
			}
			else {
				i.putback(c);
			}
		}
	}
	return i;
}

std::istream&
Netlist::comment(std::istream& i) {
	char c;
	while (i >> c) {
		if (c == '\n') {
			break;
		}
	}
	return i;
}

bool
Netlist::check(std::ostream& o) const {
	bool baddesign = false;
	// Gates
	for (_Gates::const_iterator itrG = gates().begin();
		itrG != gates().end(); itrG++) {
		baddesign |= ((*itrG).second)->check(o);
	}

	// Wires
	for (_Nets::const_iterator itrN = nets().begin();
		itrN != nets().end(); itrN++) {
		baddesign |= ((*itrN).second)->check(o);
	}
	return baddesign;
}

bool
Netlist::loops(std::ostream& o) const {
	bool hasloops = false;
	// Ports
	for (_Ports::const_iterator itrP = ports().begin();
		itrP != ports().end(); itrP++) {
		const Port* p = (*itrP);
		hasloops |= p->loops(o);
	}

	// clear loopcount
	for (_Gates::const_iterator itrG = gates().begin();
		itrG != gates().end(); itrG++) {
		((*itrG).second)->resetLoopcount();
	}

	return hasloops;
}

bool
Netlist::robdd(bdd::Manager& m) {
	// Instances
	for (_Gates::const_iterator itrG = gates().begin();
		itrG != gates().end(); itrG++) {
		((*itrG).second)->setupPropagation();
	}

	_PinQueue _q;
	// Constant robdds
	one()->initialrobdd(m, _q);
	zero()->initialrobdd(m, _q);

	// Build the initial queue and robdd
	for (_Ports::iterator itrP = ports().begin();
		itrP != ports().end(); itrP++) {
		Port* po = (*itrP);
		po->initialrobdd(m, _q);
	}

	// Propagation of the ROBDD
	while (!_q.empty()) {
		Pin* pn = _q.front();
		_q.pop_front();
		pn->robdd(m, _q);
	}

	// Build final robdd
	for (_Ports::iterator itrP = ports().begin();
		itrP != ports().end(); itrP++) {
		Port* po = (*itrP);
		po->finalrobdd(m, _q);
	}

	return false;
}

std::ostream&
Netlist::reasonMsg(std::ostream& o, const Pin* pn, bool constant) {
	const BaseGate* g = pn->gate();
	const Net* np = pn->net();

	if (constant) {
		o << "Found ONE: ";
	}
	else {
		o << "Found ZERO: ";
	}
	g->module(o);
	o << " ";
	o << g->name() << " pin (";
	pn->instance(o);
	o << ") net (";
	np->instance(o);
	o << ")" << std::endl;
	return o;
}

bool
Netlist::optimize(bdd::Manager& m, _PinQueue& _q, const Pin* pn) {
	const BaseGate* z = zero();
	const BaseGate* o = one();
	const Pin* pz = z->output();
	const Pin* po = o->output();
	Net* nz = const_cast<Net*>(pz->net());
	Net* no = const_cast<Net*>(po->net());
	assert(no);
	assert(nz);

	const bdd::NodeBase* bdd = pn->bdd();
	Net* np = const_cast<Net*>(pn->net());

	// The pin is an output pin. The net is driven by the pin.
	// Since we found that pin is a constant, we can move the outputs
	// of the net to the constant net. Then we can recursively
	// delete the logic behind.
	if (bdd == m.zero()) {
		reasonMsg(std::cerr, pn, false);
		np->disconnect(const_cast<Pin*>(pn));
		nz->import(np);
		nz->requeue(_q);
		return true;
	}
	else if (bdd == m.one()) {
		reasonMsg(std::cerr, pn, true);
		np->disconnect(const_cast<Pin*>(pn));
		no->import(np);
		no->requeue(_q);
		return true;
	}

	return false;
}

std::ostream&
Netlist::robdd(std::ostream& o) const {
	bool addnl = false;
	// Module definition
	o << "robdd " << name() << " (";
	// Ports
	_Ports::const_iterator itrP = ports().begin();
	if (itrP != ports().end()) {
		Port* p = (*itrP);
		p->instance(o);
		itrP++;
	}
	for (; itrP != ports().end(); itrP++) {
		o << ", ";
		Port* p = (*itrP);
		p->instance(o);
	}
	o << ");" << std::endl;

	// Port declarations
	for (itrP = ports().begin(); itrP != ports().end(); itrP++) {
		Port* p = (*itrP);
		p->declaration(o);
		p->robdd(o);
		o << std::endl;
		addnl = true;
	}
	if (addnl) {
		o << std::endl;
		addnl = false;
	}

	// Wires
	for (_Nets::const_iterator itrN = nets().begin();
		itrN != nets().end(); itrN++) {
		((*itrN).second)->wire(o);
		addnl = true;
	}
	if (addnl) {
		o << std::endl;
		addnl = false;
	}

	// Instances
	for (_Gates::const_iterator itrG = gates().begin();
		itrG != gates().end(); itrG++) {
		((*itrG).second)->instance(o);
		((*itrG).second)->robdd(o);
		o << std::endl;
		addnl = true;
	}
	if (addnl) {
		o << std::endl;
		addnl = false;
	}

	o << "endrobdd" << std::endl;
	o << std::endl;
	return o;
}

bool
Netlist::cleanupGates() {
	bool didone = false;
	for (_Gates::iterator itrG = gates().begin();
		itrG != gates().end(); itrG++) {
		BaseGate* g = (*itrG).second;
		if (((*itrG).second)->cleanup()) {
			std::cerr << "REMOVING INSTANCE: ";
			g->instance(std::cerr);
			gates().erase(itrG);
			didone = true;
		}
	}

	return didone;
}

bool
Netlist::cleanupNets() {
	bool didone = false;
	for (_Nets::iterator itrN = nets().begin();
		itrN != nets().end(); itrN++) {
		Net* n = (*itrN).second;
		if (((*itrN).second)->cleanup()) {
			std::cerr << "REMOVING NET: ";
			n->instance(std::cerr);
			std::cerr << std::endl;
			nets().erase(itrN);
			didone = true;
		}
	}

	return didone;
}

bool
Netlist::cleanup() {
	bool didone = false;
	didone |= cleanupGates();
	didone |= cleanupNets();
	bool didsome = didone;
	while (didone) {
		didone = false;
		didone |= cleanupGates();
		didone |= cleanupNets();
	}
	return didsome;
}

}
