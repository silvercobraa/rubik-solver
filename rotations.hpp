#ifndef ROTATIONS_HPP
#define ROTATIONS_HPP

#include <string>
#include <vector>
#include <map>

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
static Node x_rotation(Node& node, int c0, int c1, int c2, int c3)
{
	std::string aux = node.state;
	swap4(aux, c0*CORNER + Z, c1*CORNER + Y, c2*CORNER + Z, c3*CORNER + Y);
	swap4(aux, c0*CORNER + X, c1*CORNER + X, c2*CORNER + X, c3*CORNER + X);
	swap4(aux, c0*CORNER + Y, c1*CORNER + Z, c2*CORNER + Y, c3*CORNER + Z);
	return {aux, &node};
}

// rotaciones sobre el eje y
static Node y_rotation(Node& node, int c0, int c1, int c2, int c3)
{
	std::string aux = node.state;
	swap4(aux, c0*CORNER + X, c1*CORNER + Z, c2*CORNER + X, c3*CORNER + Z);
	swap4(aux, c0*CORNER + Y, c1*CORNER + Y, c2*CORNER + Y, c3*CORNER + Y);
	swap4(aux, c0*CORNER + Z, c1*CORNER + X, c2*CORNER + Z, c3*CORNER + X);
	return {aux, &node};
}

// rotaciones sobre el eje z
static Node z_rotation(Node& node, int c0, int c1, int c2, int c3)
{
	std::string aux = node.state;
	swap4(aux, c0*CORNER + X, c1*CORNER + Y, c2*CORNER + X, c3*CORNER + Y);
	swap4(aux, c0*CORNER + Z, c1*CORNER + Z, c2*CORNER + Z, c3*CORNER + Z);
	swap4(aux, c0*CORNER + Y, c1*CORNER + X, c2*CORNER + Y, c3*CORNER + X);
	return {aux, &node};
}


Node R(Node& node) {
	return x_rotation(node, 0, 1, 3, 2);
}

// R'
Node R_(Node& node) {
	return x_rotation(node, 2, 3, 1, 0);
}

Node L(Node& node) {
	return x_rotation(node, 4, 6, 7, 5);
}

// L'
Node L_(Node& node) {
	return x_rotation(node, 5, 7, 6, 4);
}

Node U(Node& node) {
	return y_rotation(node, 0, 4, 5, 1);
}

// U'
Node U_(Node& node) {
	return y_rotation(node, 1, 5, 4, 0);
}

Node D(Node& node) {
	return y_rotation(node, 2, 3, 7, 6);
}

// U'
Node D_(Node& node) {
	return y_rotation(node, 6, 7, 3, 2);
}


Node F(Node& node) {
	return z_rotation(node, 0, 2, 6, 4);
}

// F'
Node F_(Node& node) {
	return z_rotation(node, 4, 6, 2, 0);
}

Node B(Node& node) {
	return z_rotation(node, 1, 5, 7, 3);
}

// B'
Node B_(Node& node) {
	return z_rotation(node, 3, 7, 1, 5);
}

static std::string solved_state = "bry""brw""boy""bow""gry""grw""goy""gow";
;
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

typedef Node (*Action)(Node&);

std::map<std::string, Action> actions = {
	{"F", F},
	{"F'", F_},
	{"B", B},
	{"B'", B_},
	{"R", R},
	{"R'", R_},
	{"L", L},
	{"L'", L_},
	{"U", U},
	{"U'", U_},
	{"D", D},
	{"D'", D_},
};


void print(Node& node) {
	std::cout << "   +--+" << '\n';
	std::cout << "   |" << node.state[7*CORNER + X] << node.state[4*CORNER + X] << '|' << std::endl;
	std::cout << "   |" << node.state[0*CORNER + X] << node.state[3*CORNER + X] << '|' << std::endl;
	std::cout << "   +--+" << '\n';
	std::cout << "   |" << node.state[0*CORNER + Z] << node.state[3*CORNER + Z] << '|' << std::endl;
	std::cout << "   |" << node.state[1*CORNER + Z] << node.state[2*CORNER + Z] << '|' << std::endl;
	std::cout << "+--+--+--+" << '\n';
	std::cout << '|' << node.state[0*CORNER + Y] << node.state[1*CORNER + Y];
	std::cout << '|' << node.state[1*CORNER + X] << node.state[2*CORNER + X];
	std::cout << '|' << node.state[2*CORNER + Y] << node.state[3*CORNER + Y] << '|' << std::endl;
	std::cout << '|' << node.state[7*CORNER + Y] << node.state[6*CORNER + Y];
	std::cout << '|' << node.state[6*CORNER + X] << node.state[5*CORNER + X];
	std::cout << '|' << node.state[5*CORNER + Y] << node.state[4*CORNER + Y] << '|' << std::endl;
	std::cout << "+--+--+--+" << '\n';
	std::cout << "   |" << node.state[6*CORNER + Z] << node.state[5*CORNER + Z] << '|' << std::endl;
	std::cout << "   |" << node.state[7*CORNER + Z] << node.state[4*CORNER + Z] << '|' << std::endl;
	std::cout << "   +--+" << '\n';
	std::cout << std::endl;
}

#endif /* end of include guard: ROTATIONS_HPP */
