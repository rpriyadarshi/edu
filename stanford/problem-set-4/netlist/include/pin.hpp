/*
 * pin.hpp
 *
 *  Created on: Feb 16, 2009
 *      Author: rohit
 */

#ifndef PIN_HPP_
#define PIN_HPP_

#include <string>
#include <deque>

namespace bdd {
class NodeBase;
class Manager;
}

namespace nl {
class Net;
class BaseGate;
class Pin;

typedef std::deque<Pin*> _PinQueue;
const unsigned int MaxLoops(1024);

class Pin {
public: // Typedefs
	typedef std::pair<std::string, Pin*> _Pair;

public: // Enums
	enum Type { Unknown, Input, Output, Inout };

public: // Constructors/Destructors
	explicit Pin(const std::string& n) :
		m_name(n), m_type(Unknown), m_net(0), m_gate(0), m_loopcount(0) {}
	Pin(const std::string& n, enum Type t, BaseGate* g) :
		m_name(n), m_type(t), m_net(0), m_gate(g), m_loopcount(0) {}
	~Pin() {}

public: // Accessors
	const std::string& name() const { return m_name; }
	const enum Type type() const { return m_type; }
	const Net* net() const { return m_net; }
	const BaseGate* gate() const { return m_gate; }
	const bdd::NodeBase* bdd() const { return m_bdd; }
	unsigned int loopcount() const { return m_loopcount; }

public: // Utility
	_Pair pair() { return _Pair(name(), this); }
	void connect(Net* n) { net(n); }
	void connect(Net* n, bool is_driver) { net(n); type(is_driver?Output:Input); }
	void disconnect(Net* n) { net(0); /*type(Unknown);*/ }
	void bdd(const bdd::NodeBase* n) { m_bdd = n; }
	void incrLoopcount() { loopcount(loopcount()+1); }
	void decrLoopcount() { loopcount(loopcount()-1); }
	void resetLoopcount() { loopcount(0); }
	bool hasLoop() const { return loopcount()>MaxLoops; }
	bool loops(std::ostream& o);

	// ROBDD functions
	const bdd::NodeBase* createBdd(bdd::Manager& m, unsigned int index);
	bool robdd(bdd::Manager& m, _PinQueue& _q);

public: // Writers
	std::ostream& instance(std::ostream& o) const;
	std::ostream& direction(std::ostream& o) const;
	std::ostream& instance(const std::string& pre, std::ostream& o) const;
	std::ostream& robdd(std::ostream& o) const;

private: // Accessors
	std::string& name() { return m_name; }
	enum Type type() { return m_type; }
	Net* net() { return m_net; }
	BaseGate* gate() { return m_gate; }
	unsigned int loopcount() { return m_loopcount; }

	void type(enum Type t) { m_type = t; }
	void net(Net* n) { m_net = n; }
	void gate(BaseGate* g) { m_gate = g; }
	void loopcount(int l) { m_loopcount = l; }

private: // Data
	std::string				m_name;
	enum Type				m_type;
	Net*					m_net;
	BaseGate*				m_gate;
	const bdd::NodeBase*	m_bdd;
	unsigned int			m_loopcount;
};
}

#endif /* PIN_HPP_ */
