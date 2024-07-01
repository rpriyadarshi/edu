/*
 * node.hpp
 *
 *  Created on: Feb 1, 2009
 *      Author: rohit rpiyadarshi
 */

#ifndef NODE_HPP_
#define NODE_HPP_

#include <cstring>
#include <utility>
#include <iostream>
#include <assert.h>
#include <limits>

namespace bdd {

////////////////////////////////////////////////////////////////////////////////
// Node base class that provides the interface
////////////////////////////////////////////////////////////////////////////////
class NodeBase {
public: // Constructors/ Destructors
	NodeBase() {}
	~NodeBase() {}

public: // Accessors
	virtual unsigned int value() const = 0;
	virtual unsigned int index() const = 0;
	virtual const NodeBase* one() const = 0;
	virtual const NodeBase* zero() const = 0;

public: // Operators
	virtual bool operator==(const NodeBase& n) const = 0;

public: // Utility functions
	virtual bool isSink() const = 0;
	virtual std::ostream& dump(std::ostream& o) const = 0;

private: // Data
};

////////////////////////////////////////////////////////////////////////////////
// Node variant that is a terminal node
////////////////////////////////////////////////////////////////////////////////
class Sink : public NodeBase {
public: // Constructors/Destructors
	Sink() : m_value(0) {}
	Sink(unsigned int v) : m_value(v) {}
	~Sink() {}

public: // Accessors
	unsigned int value() const { return m_value; }
	unsigned int index() const { return std::numeric_limits<int>::max(); }
	const NodeBase* one() const { assert(0); return 0; }
	const NodeBase* zero() const { assert(0); return 0; }

public: // Operators
	bool operator==(const NodeBase& n) const {
		return value() == n.value();			// value compare
	}

public: // Utility functions
	bool isSink() const { return true; }
	std::ostream& dump(std::ostream& o) const;

private: // Accessors
	unsigned int value() { return m_value; }

private: // Data
	unsigned int	m_value;
};

////////////////////////////////////////////////////////////////////////////////
// Non terminal node
////////////////////////////////////////////////////////////////////////////////
class Node : public NodeBase {
public: // Constructor/Destructor
	Node() : m_index(0), m_one(0), m_zero(0) {}
	Node(unsigned int i, const NodeBase* o, const NodeBase* z) :
		m_index(i), m_one(o), m_zero(z) {}
	~Node() {}

public: // Accessors
	unsigned int value() const { return std::numeric_limits<int>::max(); }
	unsigned int index() const { return m_index; }
	const NodeBase* one() const { return m_one; }
	const NodeBase* zero() const { return m_zero; }

public: // Operators
	bool operator==(const NodeBase& n) const {
		return index() == n.index() &&	// value compare
			one() == n.one() &&			// pointer compare
			zero() == n.zero();			// pointer compare
	}

public: // Utility functions
	bool isSink() const { return false; }
	std::ostream& dump(std::ostream& o) const;

private: // Accessors
	unsigned int index() { return m_index; }
	const NodeBase* one() { return m_one; }
	const NodeBase* zero() { return m_zero; }

private: // Data
	unsigned int 		m_index;
	const NodeBase* 	m_one;
	const NodeBase* 	m_zero;
};

};

inline std::ostream&
operator<<(std::ostream& o, const bdd::NodeBase& n) {
	return n.dump(o);
}

#endif /* NODE_HPP_ */
