#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include <vector>
#include <unordered_map>

#include "node.hpp"

enum {R, U, F, F_, U_, R_, NULL_ACTION};

struct Action {
	uint64_t id;
	char name;
	int cost;
	uint64_t (*apply_to)(uint64_t);
	int inverse;
};

// esta es la base de todos los movimientos de 90 grados
static uint64_t swap4(uint64_t state, uint64_t off0, uint64_t off1, uint64_t off2, uint64_t off3) {
	// guardo los valores de los cubelets a rotar
	uint64_t new0 = (state >> off3) & MAGIC;
	uint64_t new1 = (state >> off0) & MAGIC;
	uint64_t new2 = (state >> off1) & MAGIC;
	uint64_t new3 = (state >> off2) & MAGIC;
	// borro los valores de los cubelets a rotar
	state &= ~(MAGIC << off0);
	state &= ~(MAGIC << off1);
	state &= ~(MAGIC << off2);
	state &= ~(MAGIC << off3);
	// actualizo los valores de los cubelets, rotados
	state |= new0 << off0;
	state |= new1 << off1;
	state |= new2 << off2;
	state |= new3 << off3;
	return state;
}

// rotaciones sobre el eje x
static uint64_t x_rotation(uint64_t state, uint64_t c0, uint64_t c1, uint64_t c2, uint64_t c3)
{
	state = swap4(state, c0 + Z, c1 + Y, c2 + Z, c3 + Y);
	state = swap4(state, c0 + X, c1 + X, c2 + X, c3 + X);
	state = swap4(state, c0 + Y, c1 + Z, c2 + Y, c3 + Z);
	return state;
}

// rotaciones sobre el eje y
static uint64_t y_rotation(uint64_t state, uint64_t c0, uint64_t c1, uint64_t c2, uint64_t c3)
{
	state = swap4(state, c0 + X, c1 + Z, c2 + X, c3 + Z);
	state = swap4(state, c0 + Y, c1 + Y, c2 + Y, c3 + Y);
	state = swap4(state, c0 + Z, c1 + X, c2 + Z, c3 + X);
	return state;
}

// rotaciones sobre el eje z
static uint64_t z_rotation(uint64_t state, uint64_t c0, uint64_t c1, uint64_t c2, uint64_t c3)
{
	state = swap4(state, c0 + X, c1 + Y, c2 + X, c3 + Y);
	state = swap4(state, c0 + Z, c1 + Z, c2 + Z, c3 + Z);
	state = swap4(state, c0 + Y, c1 + X, c2 + Y, c3 + X);
	return state;
}

uint64_t r(uint64_t state) {
	return x_rotation(state, RUF, RUB, RDB, RDF);
}

// R'
uint64_t r_(uint64_t state) {
	return x_rotation(state, RDF, RDB, RUB, RUF);
}

uint64_t u(uint64_t state) {
	return y_rotation(state, RUF, LUF, LUB, RUB);
}

// U'
uint64_t u_(uint64_t state) {
	return y_rotation(state, RUB, LUB, LUF, RUF);
}

uint64_t f(uint64_t state) {
	return z_rotation(state, RUF, RDF, LDF, LUF);
}

// F'
uint64_t f_(uint64_t state) {
	return z_rotation(state, LUF, LDF, RDF, RUF);
}

// identidad
uint64_t identity(uint64_t state) {
	return state;
}

// explicacion;
// R = 001, R' = 110, MAGIC = 111
// R = 001, R' = 110, MAGIC = 111
int reverse_action(int action) {
	return MAGIC - action;
};

std::unordered_map<std::string, uint64_t (*)(uint64_t)> action_by_name = {
	{"R", r}, {"R'", r_},
	{"U", u}, {"U'", u_},
	{"F", f}, {"F'", f_},
};

// las rotaciones de 90 grados de una cara tienen costo 1, mientras que las
// rotaciones en 90 grados del cubo completo tienen costo 2 (pues equivale a girar
// 90 grados 2 caras opuestas)
std::vector<Action> actions {
	{R, 'R', 1, r, R_},
	{U, 'U', 1, u, U_},
	{F, 'F', 1, f, F_},
	{F_, 'f', 1, f_, F},
	{U_, 'u', 1, u_, U},
	{R_, 'r', 1, r_, R},
};

Node* make_root() {
	Node* root = new Node();
	root->state = solved_state;
	root->parent = NULL;
	root->cost = 0;
	root->action = NULL_ACTION;
	// root->depth = 0;
	return root;
}


Node* child_node(Node* parent, Action action) {
	Node* child = new Node();
	child->state = action.apply_to(parent->state);
	child->action = action.id;
	child->parent = parent;
	child->cost = parent->cost + action.cost;
	// child->depth = parent->depth + 1;
	return child;
}


#endif /* end of include guard: ACTIONS_HPP */
