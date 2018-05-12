#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include <vector>
#include <unordered_map>
#include <stack>

#include "node.hpp"

#define NO_ACTION -1

typedef uint64_t (*Action)(uint64_t);

// esta es la base de todos los movimientos
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

uint64_t R(uint64_t state) {
	return x_rotation(state, RUF, RUB, RDB, RDF);
}

// R'
uint64_t R_(uint64_t state) {
	return x_rotation(state, RDF, RDB, RUB, RUF);
}

uint64_t U(uint64_t state) {
	return y_rotation(state, RUF, LUF, LUB, RUB);
}

// U'
uint64_t U_(uint64_t state) {
	return y_rotation(state, RUB, LUB, LUF, RUF);
}

uint64_t F(uint64_t state) {
	return z_rotation(state, RUF, RDF, LDF, LUF);
}

// F'
uint64_t F_(uint64_t state) {
	return z_rotation(state, LUF, LDF, RDF, RUF);
}


std::vector<std::string> action_name = {
	"F", "F'",
	"R", "R'",
	"U", "U'",
};

std::vector<Action> actions = {
	F, F_,
	R, R_,
	U, U_,
};

// este vector contiene el indece de la acción inversa de la acción i.
// por ejemplo, la acción F (posicion 0) tiene como inversa la acción F' (posición 1)
std::vector<int> reverse_action = {
	1, 0,
	3, 2,
	5, 4,
};

// las rotaciones de 90 grados de una cara tienen costo 1, mientras que las
// rotaciones en 90 grados del cubo completo tienen costo 2 (pues equivale a girar
// 90 grados 2 caras opuestas)
std::vector<int> action_cost = {
	1, 1,
	1, 1,
	1, 1,
};

std::unordered_map<std::string, Action> action_by_name = {
	{"F", F}, {"F'", F_},
	{"R", R}, {"R'", R_},
	{"U", U}, {"U'", U_},
};


Node* make_root() {
	Node* root = new Node();
	root->state = solved_state;
	root->parent = NULL;
	root->cost = 0;
	root->action = NO_ACTION;
	return root;
}


Node* child_node(Node* parent, int action_id) {
	Node* child = new Node();
	child->state = actions[action_id](parent->state);
	child->action = action_id;
	child->parent = parent;
	child->cost = parent->cost + action_cost[action_id];
	return child;
}


void solution(Node* node) {
	std::stack<int> stk;
	while (node->action != NO_ACTION) {
		stk.push(node->action);
		node = node->parent;
	}
	while (!stk.empty()) {
		std::cout << action_name[stk.top()];
		stk.pop();
	}
	std::cout << std::endl;
}

#endif /* end of include guard: ACTIONS_HPP */
