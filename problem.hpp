#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <string>
#include <vector>
#include <map>
#include <algorithm>

#define CORNER 3

// Este es el offset que tiene en la representación estado cada dirección.
// Por ejemplo, si el estado comienza con rwb, significa que la pieza 0 tiene
// colores rojo, blanco y azul en las coordenadas x, y y z respectivamente.
enum Offset {X, Y, Z};

// los colores del cubo resuelto, en el orden R U F L D B
std::vector<char> color = {'b', 'r', 'y', 'g', 'o', 'w'};

struct Node {
	// este string contiene 3x8 caracteres, agrupados de a 3 consecutivos. Cada
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
	//     7: left-down-back
	// Cada caracter puede ser uno de los siguientes:
	//     r: rojo
	//     b: azul
	//     w: blanco
	//     o: naranjo
	//     g: verde
	//     y: amarillo
	std::string state;
	struct Node* parent; // el nodo que se visitó antes que éste
	int cost;
	int action;
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

std::string L(std::string& state) {
	return x_rotation(state, 4, 6, 7, 5);
}

// L'
std::string L_(std::string& state) {
	return x_rotation(state, 5, 7, 6, 4);
}

std::string U(std::string& state) {
	return y_rotation(state, 0, 4, 5, 1);
}

// U'
std::string U_(std::string& state) {
	return y_rotation(state, 1, 5, 4, 0);
}

std::string D(std::string& state) {
	return y_rotation(state, 2, 3, 7, 6);
}

// U'
std::string D_(std::string& state) {
	return y_rotation(state, 6, 7, 3, 2);
}


std::string F(std::string& state) {
	return z_rotation(state, 0, 2, 6, 4);
}

// F'
std::string F_(std::string& state) {
	return z_rotation(state, 4, 6, 2, 0);
}

std::string B(std::string& state) {
	return z_rotation(state, 1, 5, 7, 3);
}

// B'
std::string B_(std::string& state) {
	return z_rotation(state, 3, 7, 5, 1);
}

// ROTACIONES DE CUBO COMPETO

std::string x(std::string& state) {
	std::string new_state = state;
	// resto equivale a R
	swap4(new_state, 0*CORNER + Z, 1*CORNER + Y, 3*CORNER + Z, 2*CORNER + Y);
	swap4(new_state, 0*CORNER + X, 1*CORNER + X, 3*CORNER + X, 2*CORNER + X);
	swap4(new_state, 0*CORNER + Y, 1*CORNER + Z, 3*CORNER + Y, 2*CORNER + Z);
	// esto equivale a L'
	swap4(new_state, 4*CORNER + Z, 5*CORNER + Y, 7*CORNER + Z, 6*CORNER + Y);
	swap4(new_state, 4*CORNER + X, 5*CORNER + X, 7*CORNER + X, 6*CORNER + X);
	swap4(new_state, 4*CORNER + Y, 5*CORNER + Z, 7*CORNER + Y, 6*CORNER + Z);
	return new_state;
}

std::string x_(std::string& state) {
	std::string new_state = state;
	// esto equivale a R'
	swap4(new_state, 2*CORNER + Z, 3*CORNER + Y, 1*CORNER + Z, 0*CORNER + Y);
	swap4(new_state, 2*CORNER + X, 3*CORNER + X, 1*CORNER + X, 0*CORNER + X);
	swap4(new_state, 2*CORNER + Y, 3*CORNER + Z, 1*CORNER + Y, 0*CORNER + Z);
	// esto equivale a L
	swap4(new_state, 6*CORNER + Z, 7*CORNER + Y, 5*CORNER + Z, 4*CORNER + Y);
	swap4(new_state, 6*CORNER + X, 7*CORNER + X, 5*CORNER + X, 4*CORNER + X);
	swap4(new_state, 6*CORNER + Y, 7*CORNER + Z, 5*CORNER + Y, 4*CORNER + Z);
	return new_state;
}

std::string y(std::string& state)
{
	std::string new_state = state;
	// esto equivale a U
	swap4(new_state, 0*CORNER + X, 4*CORNER + Z, 5*CORNER + X, 1*CORNER + Z);
	swap4(new_state, 0*CORNER + Y, 4*CORNER + Y, 5*CORNER + Y, 1*CORNER + Y);
	swap4(new_state, 0*CORNER + Z, 4*CORNER + X, 5*CORNER + Z, 1*CORNER + X);
	// esto equivale a D'
	swap4(new_state, 2*CORNER + X, 6*CORNER + Z, 7*CORNER + X, 3*CORNER + Z);
	swap4(new_state, 2*CORNER + Y, 6*CORNER + Y, 7*CORNER + Y, 3*CORNER + Y);
	swap4(new_state, 2*CORNER + Z, 6*CORNER + X, 7*CORNER + Z, 3*CORNER + X);
	return new_state;
}

std::string y_(std::string& state)
{
	std::string new_state = state;
	// esto equivale a U'
	swap4(new_state, 1*CORNER + X, 5*CORNER + Z, 4*CORNER + X, 0*CORNER + Z);
	swap4(new_state, 1*CORNER + Y, 5*CORNER + Y, 4*CORNER + Y, 0*CORNER + Y);
	swap4(new_state, 1*CORNER + Z, 5*CORNER + X, 4*CORNER + Z, 0*CORNER + X);
	// esto equivale a D
	swap4(new_state, 3*CORNER + X, 7*CORNER + Z, 6*CORNER + X, 2*CORNER + Z);
	swap4(new_state, 3*CORNER + Y, 7*CORNER + Y, 6*CORNER + Y, 2*CORNER + Y);
	swap4(new_state, 3*CORNER + Z, 7*CORNER + X, 6*CORNER + Z, 2*CORNER + X);
	return new_state;
}

std::string z(std::string& state)
{
	std::string new_state = state;
	// este movimiento equivale a F
	swap4(new_state, 0*CORNER + X, 2*CORNER + Y, 6*CORNER + X, 4*CORNER + Y);
	swap4(new_state, 0*CORNER + Z, 2*CORNER + Z, 6*CORNER + Z, 4*CORNER + Z);
	swap4(new_state, 0*CORNER + Y, 2*CORNER + X, 6*CORNER + Y, 4*CORNER + X);
	// este movimiento equivale a B'
	swap4(new_state, 1*CORNER + X, 3*CORNER + Y, 7*CORNER + X, 5*CORNER + Y);
	swap4(new_state, 1*CORNER + Z, 3*CORNER + Z, 7*CORNER + Z, 5*CORNER + Z);
	swap4(new_state, 1*CORNER + Y, 3*CORNER + X, 7*CORNER + Y, 5*CORNER + X);
	return new_state;
}

std::string z_(std::string& state)
{
	std::string new_state = state;
	// este movimiento equivale a F'
	swap4(new_state, 4*CORNER + X, 6*CORNER + Y, 2*CORNER + X, 0*CORNER + Y);
	swap4(new_state, 4*CORNER + Z, 6*CORNER + Z, 2*CORNER + Z, 0*CORNER + Z);
	swap4(new_state, 4*CORNER + Y, 6*CORNER + X, 2*CORNER + Y, 0*CORNER + X);
	// este movimiento equivale a B
	swap4(new_state, 5*CORNER + X, 7*CORNER + Y, 3*CORNER + X, 1*CORNER + Y);
	swap4(new_state, 5*CORNER + Z, 7*CORNER + Z, 3*CORNER + Z, 1*CORNER + Z);
	swap4(new_state, 5*CORNER + Y, 7*CORNER + X, 3*CORNER + Y, 1*CORNER + X);
	return new_state;
}



static std::string solved_state = "bry""brw""boy""bow""gry""grw""goy""gow";

// retorna la cantidad de piezas en posición incorrecta, dividida por 4
double heuristic(Node& node) {
	static std::vector<std::string> solved = {"bry", "brw", "boy", "bow", "gry", "grw", "goy", "gow"};
	int sum = 0;
	for (int i = 0; i < solved.size(); i++) {
		std::string sub = node.state.substr(i*3, 3);
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

// el cubo se considera cuando se tienen los siguientes colores:
//     azul en la cara frontal
//     verde en la cara trasera
//     blanco en la cara derecha
//     amarillo  en la cara izquierda
//     rojo en la cara superior
//     naranjo en la cara inferior
bool goal_test(Node& node) {
	return node.state == solved_state;

}

typedef std::string (*Action)(std::string&);

std::vector<std::string> action_name = {
	"F", "F'", "B", "B'",
	"R", "R'", "L", "L'",
	"U", "U'", "D", "D'",
	//"x", "x'",
	//"y", "y'",
	//"z", "z'",
};

std::vector<Action> actions = {
	F, F_, B, B_,
	R, R_, L, L_,
	U, U_, D, D_,
	//x, x_,
	//y, y_,
	//z, z_,
};

// las rotaciones de 90 grados de una cara tienen costo 1, mientras que las
// rotaciones en 90 grados del cubo completo tienen costo 2 (pues equivale a girar
// 90 grados 2 caras opuestas)
std::vector<int> action_cost = {
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	//2, 2,
	//2, 2,
	//2, 2,
};

std::map<std::string, Action> action_by_name = {
	{"F", F}, {"F'", F_}, {"B", B}, {"B'", B_},
	{"R", R}, {"R'", R_}, {"L", L}, {"L'", L_},
	{"U", U}, {"U'", U_}, {"D", D}, {"D'", D_},
	//{"x", x}, {"x'", x_},
	//{"y", y}, {"y'", y_},
	//{"z", z}, {"z'", z_},
};

Node child_node(Node& parent, int action_id) {
	Node child;
	child.state = actions[action_id](parent.state);
	child.action = action_id;
	child.parent = NULL;
	child.cost = parent.cost + action_cost[action_id];
	return child;
}

void print(Node& node) {
	std::cout << "   +--+" << '\n';
	std::cout << "   |" << node.state[7*CORNER + Z] << node.state[3*CORNER + Z] << '|' << std::endl;
	std::cout << "   |" << node.state[5*CORNER + Z] << node.state[1*CORNER + Z] << '|' << std::endl;
	std::cout << "   +--+" << '\n';
	std::cout << "   |" << node.state[5*CORNER + Y] << node.state[1*CORNER + Y] << '|' << std::endl;
	std::cout << "   |" << node.state[4*CORNER + Y] << node.state[0*CORNER + Y] << '|' << std::endl;
	std::cout << "+--+--+--+" << '\n';
	std::cout << '|' << node.state[5*CORNER + X] << node.state[4*CORNER + X];
	std::cout << '|' << node.state[4*CORNER + Z] << node.state[0*CORNER + Z];
	std::cout << '|' << node.state[0*CORNER + X] << node.state[1*CORNER + X] << '|' << std::endl;
	std::cout << '|' << node.state[7*CORNER + X] << node.state[6*CORNER + X];
	std::cout << '|' << node.state[6*CORNER + Z] << node.state[2*CORNER + Z];
	std::cout << '|' << node.state[2*CORNER + X] << node.state[3*CORNER + X] << '|' << std::endl;
	std::cout << "+--+--+--+" << '\n';
	std::cout << "   |" << node.state[6*CORNER + Y] << node.state[2*CORNER + Y] << '|' << std::endl;
	std::cout << "   |" << node.state[7*CORNER + Y] << node.state[3*CORNER + Y] << '|' << std::endl;
	std::cout << "   +--+" << '\n';
	std::cout << std::endl;
}

#endif /* end of include guard: PROBLEM_HPP */
