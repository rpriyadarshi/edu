/*
 * gate.hpp
 *
 *  Created on: Feb 16, 2009
 *      Author: rohit
 */

#ifndef GATE_HPP_
#define GATE_HPP_

#include <vector>
#include <deque>
#include <string>
#include <iostream>
#include <assert.h>
#include <limits>

#include "pin.hpp"

namespace bdd {
class Manager;
}

namespace nl {
class Netlist;
class Pin;

// Constant strings used for pin names
const std::string N_o("y");
const std::string N_po("po");
const std::string N_pi("pi");
const std::string N_i("i");
const std::string N_i1("a");
const std::string N_i2("b");

// Constant strings used for gate names
const std::string N_Not("not");
const std::string N_And2("and2");
const std::string N_Or2("or2");
const std::string N_Xor2("xor2");
const std::string N_Zero("zero");
const std::string N_One("one");
const int Bad(-1);

typedef std::deque<Pin*> _PinQueue;

class BaseGate {
public: // typedefs
	typedef std::vector<Pin*> _Pins;
	typedef std::pair<std::string, BaseGate*> _Pair;

public: // Enums
	enum Type { Not, And2, Or2, Xor2, Zero, One };

public: // Constructors/Destructors
	explicit BaseGate(Netlist* nlst) :
		m_output(0), m_netlist(nlst), m_propagate(Bad) {}
	BaseGate(const std::string& n, Netlist* nlst) :
		m_name(n), m_output(0), m_netlist(nlst), m_propagate(Bad)  {}
	~BaseGate() { delete output(); }

public: // Accessors
	const std::string& name() const { return m_name; }
	const Pin* output() const { return m_output; }
	const _Pins& inputs() const { return m_inputs; }
	const Netlist* netlist() const { return m_netlist; }
	int propagate() const { return m_propagate; }

public: // Creation functions
	Pin* createInputPin(const std::string& n);
	Pin* createOutputPin(const std::string& n);
	Pin* createPin(const std::string& n, enum Pin::Type t);

public: // Utility
	_Pair pair() { return _Pair(name(), this); }
	const Pin* firstInput() const { return !inputs().empty()?inputs()[0]:0; }
	const Pin* secondInput() const { return !inputs().empty()?inputs()[1]:0; }
	const Pin* findPin(const std::string& n) const;
	bool canPropagate() { return propagate() <= 0; }
	void incrPropagate() { propagate(propagate()+1); }
	void decrPropagate() { propagate(propagate()-1); }
	void setupPropagation() { propagate(inputs().size()); };

	// ROBDD functions
	virtual unsigned int index() const { return std::numeric_limits<int>::max(); }
	virtual void index(unsigned int i) { assert(0); }
	virtual bool robdd(bdd::Manager& m, _PinQueue& _q);
	virtual bool initialrobdd(bdd::Manager& m, _PinQueue& _q);
	virtual bool buildrobdd(bdd::Manager& m);
	const bdd::NodeBase* bdd(unsigned int idx) const;
	virtual bool optimize(bdd::Manager& m, _PinQueue& _q);
	virtual bool cleanup();

public: // Writers
	virtual std::ostream& instance(std::ostream& o) const;
	std::istream& name(std::istream& i);
	virtual std::istream& instance(std::istream& i);
	virtual std::ostream& robdd(std::ostream& o) const;
	virtual std::ostream& module(std::ostream& o) const = 0;
	std::ostream& checkMsg(std::ostream& o, const Pin* pn) const;
	std::ostream& reasonMsg(std::ostream& o, const Pin* pn);
	std::ostream& reasonMsg(std::ostream& o, const Pin* pn, bool constant);

public: // Checks
	bool valid() const { return !name().empty(); }
	bool valid() { return !name().empty(); }
	virtual bool isPort() { return false; }
	bool check(std::ostream& o) const;
	bool loops(std::ostream& o) const;
	void resetLoopcount() const;

protected: // Accessors
	std::string& name() { return m_name; }
	Pin* output() { return m_output; }
	_Pins& inputs() { return m_inputs; }
	Netlist* netlist() { return m_netlist; }
	int propagate() { return m_propagate; }
	void output(Pin* o) { m_output = o; }
	void propagate(int p) { m_propagate = p; }

protected: // Data
	std::string			m_name;
	Pin*				m_output;
	_Pins				m_inputs;
	Netlist*			m_netlist;
	int					m_propagate;
};

////////////////////////////////////////////////////////////////////////////////
// NOT gate
////////////////////////////////////////////////////////////////////////////////
class Not : public BaseGate {
public: // Constructors/Destructors
	explicit Not(Netlist* nlst) : BaseGate(nlst) {
		createOutputPin(N_o);
		createInputPin(N_i);
	}
	Not(const std::string& n, Netlist* nlst) : BaseGate(n, nlst) {
		createOutputPin(N_o);
		createInputPin(N_i);
	}
	~Not() {}

public: // Utility
	// ROBDD functions
	bool buildrobdd(bdd::Manager& m);

public: // Writers
	std::ostream& module(std::ostream& o) const {
		o << N_Not;
		return o;
	}
};

////////////////////////////////////////////////////////////////////////////////
// 2 input AND gate
////////////////////////////////////////////////////////////////////////////////
class And2 : public BaseGate {
public: // Constructors/Destructors
	explicit And2(Netlist* nlst) : BaseGate(nlst) {
		createOutputPin(N_o);
		createInputPin(N_i1);
		createInputPin(N_i2);
	}
	And2(const std::string& n, Netlist* nlst) : BaseGate(n, nlst) {
		createOutputPin(N_o);
		createInputPin(N_i1);
		createInputPin(N_i2);
	}
	~And2() {}

public: // Utility
	// ROBDD functions
	bool buildrobdd(bdd::Manager& m);

public: // Writers
	std::ostream& module(std::ostream& o) const {
		o << N_And2;
		return o;
	}
};

////////////////////////////////////////////////////////////////////////////////
// 2 input OR gate
////////////////////////////////////////////////////////////////////////////////
class Or2 : public BaseGate {
public: // Constructors/Destructors
	explicit Or2(Netlist* nlst) : BaseGate(nlst) {
		createOutputPin(N_o);
		createInputPin(N_i1);
		createInputPin(N_i2);
	}
	Or2(const std::string& n, Netlist* nlst) : BaseGate(n, nlst) {
		createOutputPin(N_o);
		createInputPin(N_i1);
		createInputPin(N_i2);
	}
	~Or2() {}

public: // Utility
	// ROBDD functions
	bool buildrobdd(bdd::Manager& m);

public: // Writers
	std::ostream& module(std::ostream& o) const {
		o << N_Or2;
		return o;
	}
};

////////////////////////////////////////////////////////////////////////////////
// 2 input OR gate
////////////////////////////////////////////////////////////////////////////////
class Xor2 : public BaseGate {
public: // Constructors/Destructors
	explicit Xor2(Netlist* nlst) : BaseGate(nlst) {
		createOutputPin(N_o);
		createInputPin(N_i1);
		createInputPin(N_i2);
	}
	Xor2(const std::string& n, Netlist* nlst) : BaseGate(n, nlst) {
		createOutputPin(N_o);
		createInputPin(N_i1);
		createInputPin(N_i2);
	}
	~Xor2() {}

public: // Utility
	// ROBDD functions
	bool buildrobdd(bdd::Manager& m);
	bool optimize(bdd::Manager& m, _PinQueue& _q);

public: // Writers
	std::ostream& module(std::ostream& o) const {
		o << N_Xor2;
		return o;
	}
};

////////////////////////////////////////////////////////////////////////////////
// Zero cell
////////////////////////////////////////////////////////////////////////////////
class Zero : public BaseGate {
public: // Constructors/Destructors
	explicit Zero(Netlist* nlst) : BaseGate(nlst) {
		createOutputPin(N_o);
	}
	Zero(const std::string& n, Netlist* nlst) : BaseGate(n, nlst) {
		createOutputPin(N_o);
	}
	~Zero() {}

public: // Utility
	// ROBDD functions
	bool buildrobdd(bdd::Manager& m);

public: // Writers
	std::ostream& module(std::ostream& o) const {
		o << N_Zero;
		return o;
	}
};

////////////////////////////////////////////////////////////////////////////////
// One cell
////////////////////////////////////////////////////////////////////////////////
class One : public BaseGate {
public: // Constructors/Destructors
	explicit One(Netlist* nlst) : BaseGate(nlst)  {
		createOutputPin(N_o);
	}
	One(const std::string& n, Netlist* nlst) : BaseGate(n, nlst) {
		createOutputPin(N_o);
	}
	~One() {}

public: // Utility
	// ROBDD functions
	bool buildrobdd(bdd::Manager& m);

public: // Writers
	std::ostream& module(std::ostream& o) const {
		o << N_One;
		return o;
	}
};

}


#endif /* GATE_HPP_ */
