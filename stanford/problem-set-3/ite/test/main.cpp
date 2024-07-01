/*
 * main.cpp
 *
 *  Created on: Jan 30, 2009
 *      Author: rohit rpiyadarshi
 */

#include "node.hpp"
#include "manager.hpp"

const bdd::NodeBase*
createVarROBDD(bdd::Manager& m, unsigned int v, const bdd::NodeBase* o,
		const bdd::NodeBase* z) {
	const bdd::NodeBase* n = m.createNode(v, o, z);
	m.uniqueTable().insert(v, o, z, n);
	return n;
}

const bdd::NodeBase*
createVarROBDD(bdd::Manager& m, unsigned int v) {
	const bdd::NodeBase* n = createVarROBDD(m, v, m.one(), m.zero());
	return n;
}

const bdd::NodeBase*
createAndROBDD(bdd::Manager& m, unsigned int i, unsigned int j) {
	const bdd::NodeBase* b = createVarROBDD(m, j);
	const bdd::NodeBase* a = createVarROBDD(m, i, b, m.zero());
	return a;
}

const bdd::NodeBase*
createAndUniqueTableROBDD(bdd::Manager& m, unsigned int i, unsigned int j) {
	const bdd::NodeBase* b =
		m.uniqueTable().findOrCreateAndInsert(j, m.one(), m.zero());
	const bdd::NodeBase* a =
		m.uniqueTable().findOrCreateAndInsert(i, b, m.zero());
	return a;
}

void
testBasic() {
	bdd::Manager m;
	const bdd::NodeBase* x = createAndROBDD(m, 1, 2);
	std::cout << *x << std::endl;
	std::cout << m << std::endl;

	const bdd::NodeBase* y = createAndUniqueTableROBDD(m, 1, 2);
	std::cout << *y << std::endl;
	std::cout << m << std::endl;
}

void
testIte() {
	bdd::Manager m;
	// Variable creation.
	const bdd::NodeBase* x1 = createVarROBDD(m, 1);
	const bdd::NodeBase* x2 = createVarROBDD(m, 2);
	const bdd::NodeBase* x3 = createVarROBDD(m, 3);
	//const bdd::NodeBase* x4 = createVarROBDD(m, 4);
	//const bdd::NodeBase* x5 = createVarROBDD(m, 5);

	// ROBDD creation
	const bdd::NodeBase* nu1 = m.ite(x1, x2, false);
	const bdd::NodeBase* nu4 = m.ite(nu1, true, x3);

	std::cout << *nu1 << std::endl;
	std::cout << *nu4 << std::endl;
	std::cout << m << std::endl;
}

void
testIte2() {
	bdd::Manager m;

	// ROBDD creation
	const bdd::NodeBase* nu1 = m.iteAnd(1, 2);
	const bdd::NodeBase* nu4 = m.iteOr(nu1, 3);

	std::cout << *nu1 << std::endl;
	std::cout << *nu4 << std::endl;
	std::cout << m << std::endl;
}

//using namespace __gnu_cxx;
int main (int argc, const char** argv) {
	testBasic();
	testIte();
	testIte2();
}
