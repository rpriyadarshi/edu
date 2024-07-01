/*
 * manager.cpp
 *
 *  Created on: Feb 1, 2009
 *      Author: rohit rpiyadarshi
 */

#include "manager.hpp"
#include "boolfunc.hpp"
#include "node.hpp"
#include <assert.h>

namespace bdd {

////////////////////////////////////////////////////////////////////////////////
// Functions for class UniqueTable
////////////////////////////////////////////////////////////////////////////////
const BoolFunc*
UniqueTable::createBoolFunc(unsigned int v, const NodeBase* t,
		const NodeBase* e) {
	assert(t);
	assert(e);
	return new BoolFunc(v, t, e);
}

const Sink*
UniqueTable::createSink(unsigned int v) {
	return new Sink(v);
}

const Node*
UniqueTable::createNode(unsigned int i, const NodeBase* o,
		const NodeBase* z) {
	return new Node(i, o, z);
}

const NodeBase*
UniqueTable::find(const BoolFunc* bf) const {
	_Table::const_iterator itr = table().find(bf);
	if (itr != table().end() && (*itr).second) {
		return (*itr).second;
	}
	return 0;
}

const NodeBase*
UniqueTable::find(unsigned int v, const NodeBase* t,
		const NodeBase* e) const {
	BoolFunc bf(v, t, e);
	return find(&bf); // no return value optimization
}

const NodeBase*
UniqueTable::findOrCreateAndInsert(unsigned int v, const NodeBase* t,
		const NodeBase* e) {
	const NodeBase* i = find(v, t, e);
	if (!i) {
		i = createNode(v, t, e);
		insert(v, t, e, i);
	}
	return i;
}

const BoolFunc*
UniqueTable::findFunc(const BoolFunc* bf) const {
	_Table::const_iterator itr = table().find(bf);
	if (itr != table().end()) {
		return (*itr).first;
	}
	return 0;
}

const BoolFunc*
UniqueTable::findFunc(unsigned int v, const NodeBase* t,
		const NodeBase* e) const {
	BoolFunc bf(v, t, e);
	return findFunc(&bf); // no return value optimization
}

void
UniqueTable::insert(const BoolFunc* bf, const NodeBase* i) {
	table().insert(bf->pair(i));
}

void
UniqueTable::insert(unsigned int v, const NodeBase* t, const NodeBase* e,
		const NodeBase* i) {
	const BoolFunc* bf = createBoolFunc(v, t, e);
	insert(bf, i);
}

std::ostream&
UniqueTable::dump(std::ostream& o) const {
	o << "UNIQUE_TABLE {" << std::endl;
	_Table::const_iterator itr = table().begin();
	for (table().begin(); itr != table().end(); ++itr) {
		o << (*(*itr).second) << std::endl;
	}
	o << "}" << std::endl;
	return o;
}

////////////////////////////////////////////////////////////////////////////////
// Functions for class ComputedTable
////////////////////////////////////////////////////////////////////////////////
const BoolFuncTuple*
ComputedTable::createBoolFuncTuple(const NodeBase* f, const NodeBase* g,
		const NodeBase* h) {
	assert(f);
	assert(g);
	assert(h);
	return new BoolFuncTuple(f, g, h);
}

const NodeBase*
ComputedTable::find(const BoolFuncTuple* bft) const {
	_Table::const_iterator itr = table().find(bft);
	if (itr != table().end() && (*itr).second) {
		return (*itr).second;
	}
	return 0;
}

const NodeBase*
ComputedTable::find(const NodeBase* f, const NodeBase* g,
		const NodeBase* h) const {
	BoolFuncTuple bf(f, g, h);
	return find(&bf); // no return value optimization
}

void
ComputedTable::insert(const BoolFuncTuple* bf, const NodeBase* i) {
	table().insert(bf->pair(i));
}

void
ComputedTable::insert(const NodeBase* f, const NodeBase* g, const NodeBase* h,
		const NodeBase* i) {
	const BoolFuncTuple* bft = createBoolFuncTuple(f, g, h);
	insert(bft, i);
}

std::ostream&
ComputedTable::dump(std::ostream& o) const {
	o << "COMPUTED_TABLE {" << std::endl;
	_Table::const_iterator itr = table().begin();
	for (table().begin(); itr != table().end(); ++itr) {
		o << (*(*itr).second) << std::endl;
	}
	o << "}" << std::endl;
	return o;
}

////////////////////////////////////////////////////////////////////////////////
// Functions for class Manager
////////////////////////////////////////////////////////////////////////////////
Manager::Manager() {
	m_one = createSink(1);
	m_zero = createSink(0);
}

Manager::~Manager() {
	delete one();
	delete zero();
}

const BoolFunc*
Manager::createBoolFunc(unsigned int v, const NodeBase* t,
		const NodeBase* e) const {
	assert(t);
	assert(e);
	return new BoolFunc(v, t, e);
}

const Sink*
Manager::createSink(unsigned int v) const {
	return new Sink(v);
}

const Node*
Manager::createNode(unsigned int i, const NodeBase* o,
		const NodeBase* z) const {
	assert(o);
	assert(z);
	return new Node(i, o, z);
}

unsigned int
Manager::findTopVar(const NodeBase* f, const NodeBase* g,
		const NodeBase* h) const {
	//Let v be top variable of {F, G, H};
	// Select top variable based on variable ordering. TBD
	unsigned int x = f->index() < g->index() ? f->index() : g->index();
	unsigned int y = x < h->index() ? x : h->index();
	return y;
}

// Terminal cases
// ITE(1, F, G) = F
// ITE(0, G, F) = F
// ITE(F, 1, 0) = F
// ITE(G, F, F) = F
const NodeBase*
Manager::terminalCase(const NodeBase* f, const NodeBase* g,
		const NodeBase* h) const {
	const NodeBase* r = 0;
	if (f == one()) {
		r = g;
	}
	if (f == zero()) {
		r = h;
	}
	if (g == one() && h == zero()) {
		r = f;
	}
	if (g == h) {
		r = g;
	}
	return r;
}

const NodeBase*
Manager::cofactorThen(unsigned int topVar, const NodeBase* f) {
	return f->index() == topVar?f->one():f;
}

const NodeBase*
Manager::cofactorElse(unsigned int topVar, const NodeBase* f) {
	return f->index() == topVar?f->zero():f;
}

const NodeBase*
Manager::ite(const NodeBase* f, const NodeBase* g, const NodeBase* h) {
	const NodeBase* result = terminalCase(f, g, h);
	if (result) {
		return result;
	}

	result = computedTable().find(f, g, h);
	if (result) {
		return result;
	}

	unsigned int topVar = findTopVar(f, g, h);
	const NodeBase* t = ite(cofactorThen(topVar, f),
			cofactorThen(topVar, g), cofactorThen(topVar, h));
	const NodeBase* e = ite(cofactorElse(topVar, f),
			cofactorElse(topVar, g), cofactorElse(topVar, h));
	if (t == e) {
		return t;
	}
	const NodeBase* r = uniqueTable().findOrCreateAndInsert(topVar, t, e);
	computedTable().insert(f, g, h, r);
	return r;
}


const NodeBase*
Manager::ite(const NodeBase* f, const NodeBase* g, bool hval) {
	const NodeBase* h = hval?one():zero();
	return ite(f, g, h);
}

const NodeBase*
Manager::ite(const NodeBase* f, bool gval, const NodeBase* h) {
	const NodeBase* g = gval?one():zero();
	return ite(f, g, h);
}

const NodeBase*
Manager::ite(const NodeBase* f, bool gval, bool hval) {
	const NodeBase* g = gval?one():zero();
	const NodeBase* h = hval?one():zero();
	return ite(f, g, h);
}

const NodeBase*
Manager::ite(bool fval, const NodeBase* g, const NodeBase* h) {
	const NodeBase* f = fval?one():zero();
	return ite(f, g, h);
}

const NodeBase*
Manager::ite(bool fval, const NodeBase* g, bool hval) {
	const NodeBase* f = fval?one():zero();
	const NodeBase* h = hval?one():zero();
	return ite(f, g, h);
}

const NodeBase*
Manager::ite(bool fval, bool gval, const NodeBase* h) {
	const NodeBase* f = fval?one():zero();
	const NodeBase* g = gval?one():zero();
	return ite(f, g, h);
}

const NodeBase*
Manager::ite(bool fval, bool gval, bool hval){
	const NodeBase* f = fval?one():zero();
	const NodeBase* g = gval?one():zero();
	const NodeBase* h = hval?one():zero();
	return ite(f, g, h);
}

std::ostream&
Manager::dump(std::ostream& o) const {
	o << (*one()) << std::endl;
	o << (*zero()) << std::endl;
	o << uniqueTable() << std::endl;
	o << computedTable() << std::endl;
	return o;
}

const bdd::NodeBase*
Manager::buildVar(unsigned int v, const bdd::NodeBase* o,
		const bdd::NodeBase* z) {
	return uniqueTable().findOrCreateAndInsert(v, o, z);
}

const bdd::NodeBase*
Manager::buildVar(unsigned int v) {
	const bdd::NodeBase* n = buildVar(v, one(), zero());
	return n;
}

const bdd::NodeBase*
Manager::buildAnd(unsigned int i, unsigned int j) {
	const bdd::NodeBase* b = buildVar(j);
	const bdd::NodeBase* a = buildVar(i, b, zero());
	return a;
}

const bdd::NodeBase*
Manager::buildOr(unsigned int i, unsigned int j) {
	const bdd::NodeBase* b = buildVar(j);
	const bdd::NodeBase* a = buildVar(i, one(), b);
	return a;
}

}
