#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <stack>

#define CORNER 3
#define NO_ACTION -1

// Este es el offset que tiene en la representación estado cada dirección.
// Por ejemplo, si el estado comienza con rwb, significa que la pieza 0 tiene
// colores rojo, blanco y azul en las coordenadas x, y y z respectivamente.
enum Offset {X, Y, Z};

struct Node {
	// este string contiene 3x7 caracteres, agrupados de a 3 consecutivos. Cada
	// grupo de 3 caracteres corresponde a una esquina del cubo, y cada uno de
	// dichos caracteres corresponden a los colores en la cara ortogonal al eje
	// 'x' (right-left), 'y' (up-down), y 'z' (front-back), respectivamente.
	// Las esquinas están numeradas de la siguiente forma:
	//     0: right-up-front
	//     1: right-up-back
	//     2: right-down-front
	//     3: right-down-back
	//     4: left-up-front
	//     5: left-up-back
	//     6: left-down-front
	// La esquina left-down-back no se representa, ya que esta define el sistema de referencia
	// Cada caracter puede ser uno de los siguientes:
	//     r: rojo
	//     b: azul
	//     w: blanco
	//     o: naranjo
	//     g: verde
	//     y: amarillo
	std::string state;
	int cost;
	int action;
	struct Node* parent; // el nodo que se visitó antes que éste
	bool operator< (const Node& node) const {
		return state < node.state;
	}
	bool operator== (const Node& node) const {
		return state == node.state;
	}
};

// Intercambia 4 posiciones en 'circular fashion'. Es la base de todos los movimientos
// p1 <- p0
// p2 <- p1
// p3 <- p2
// p0 <- p3
static void swap4(std::string& state, int p0, int p1, int p2, int p3) {
	int aux1 = state[p1];
	int aux2 = state[p3];
	state[p1] = state[p0];
	state[p3] = state[p2];
	state[p0] = aux2;
	state[p2] = aux1;
}

// rotaciones sobre el eje x
static std::string x_rotation(std::string& state, int c0, int c1, int c2, int c3)
{
	std::string new_state = state;
	swap4(new_state, c0*CORNER + Z, c1*CORNER + Y, c2*CORNER + Z, c3*CORNER + Y);
	swap4(new_state, c0*CORNER + X, c1*CORNER + X, c2*CORNER + X, c3*CORNER + X);
	swap4(new_state, c0*CORNER + Y, c1*CORNER + Z, c2*CORNER + Y, c3*CORNER + Z);
	return new_state;
}

// rotaciones sobre el eje y
static std::string y_rotation(std::string& state, int c0, int c1, int c2, int c3)
{
	std::string new_state = state;
	swap4(new_state, c0*CORNER + X, c1*CORNER + Z, c2*CORNER + X, c3*CORNER + Z);
	swap4(new_state, c0*CORNER + Y, c1*CORNER + Y, c2*CORNER + Y, c3*CORNER + Y);
	swap4(new_state, c0*CORNER + Z, c1*CORNER + X, c2*CORNER + Z, c3*CORNER + X);
	return new_state;
}

// rotaciones sobre el eje z
static std::string z_rotation(std::string& state, int c0, int c1, int c2, int c3)
{
	std::string new_state = state;
	swap4(new_state, c0*CORNER + X, c1*CORNER + Y, c2*CORNER + X, c3*CORNER + Y);
	swap4(new_state, c0*CORNER + Z, c1*CORNER + Z, c2*CORNER + Z, c3*CORNER + Z);
	swap4(new_state, c0*CORNER + Y, c1*CORNER + X, c2*CORNER + Y, c3*CORNER + X);
	return new_state;
}

// ROTACIONES DE CARAS EN 90 GRADOS

std::string R(std::string& state) {
	return x_rotation(state, 0, 1, 3, 2);
}

// R'
std::string R_(std::string& state) {
	return x_rotation(state, 2, 3, 1, 0);
}

std::string U(std::string& state) {
	return y_rotation(state, 0, 4, 5, 1);
}

// U'
std::string U_(std::string& state) {
	return y_rotation(state, 1, 5, 4, 0);
}

std::string F(std::string& state) {
	return z_rotation(state, 0, 2, 6, 4);
}

// F'
std::string F_(std::string& state) {
	return z_rotation(state, 4, 6, 2, 0);
}

// el estado resuelto es aquel que tiene las caras izquierda, inferior y trasera
// con los colores verde, naranjo y blanco, respectivamente
static std::string solved_state = "bry""brw""boy""bow""gry""grw""goy";

// retorna la cantidad de piezas en posición incorrecta, dividida por 4
double heuristic(Node* node) {
	static std::vector<std::string> solved = {"bry", "brw", "boy", "bow", "gry", "grw", "goy"};
	int sum = 0;
	for (int i = 0; i < solved.size(); i++) {
		std::string sub = node->state.substr(i*3, 3);
		std::sort(solved[i].begin(), solved[i].end());
		bool correct_position = false;
		do {
			if (sub == solved[i]) {
				correct_position = true;
				break;
			}
		} while(std::next_permutation(solved[i].begin(), solved[i].end()));
		if (!correct_position) {
			sum++;
		}
	}
	return sum / 4.0;
}

// esta matriz me dice por cada pieza y por cada posición a que distancia
// (de manhattan) esta de su posición correcta
static std::map<std::string, std::vector<int>> manhattan_distance = {
	{"bry", {0, 1, 1, 2, 1, 2, 2, 3}},
	{"brw", {1, 0, 2, 1, 2, 1, 3, 2}},
	{"boy", {1, 2, 0, 1, 2, 3, 1, 2}},
	{"bow", {2, 1, 1, 0, 3, 2, 2, 1}},
	{"gry", {1, 2, 2, 3, 0, 1, 1, 2}},
	{"grw", {2, 1, 3, 2, 1, 0, 2, 1}},
	{"goy", {2, 3, 1, 2, 1, 2, 0, 1}},
	// {"gow", {3, 2, 2, 1, 2, 1, 1, 0}},
};

// esta heurística usa la distancia de manhattan de todas las piezas dividida por 4
double heuristic2(Node* node) {
	int sum = 0;
	for (int i = 0; i < node->state.size(); i += 3) {
		std::string sub = node->state.substr(i, 3);
		std::sort(sub.begin(), sub.end());
		sum += manhattan_distance[sub][i/3];
	}
	return sum / 4.0;
}

// el cubo se considera cuando se tienen los siguientes colores:
//     azul en la cara frontal
//     verde en la cara trasera
//     blanco en la cara derecha
//     amarillo  en la cara izquierda
//     rojo en la cara superior
//     naranjo en la cara inferior
bool goal_test(Node* node) {
	return node->state == solved_state;

}

typedef std::string (*Action)(std::string&);

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

std::map<std::string, Action> action_by_name = {
	{"F", F}, {"F'", F_},
	{"R", R}, {"R'", R_},
	{"U", U}, {"U'", U_},
};


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
		// std::cout << action_name[node->action] << ' ';
		stk.push(node->action);
		node = node->parent;
	}
	while (!stk.empty()) {
		std::cout << action_name[stk.top()];
		stk.pop();
	}
	std::cout << std::endl;
}

#endif /* end of include guard: PROBLEM_HPP */
