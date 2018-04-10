#ifndef ROTATIONS_HPP
#define ROTATIONS_HPP

#include <string>

#define X 0
#define Y 1
#define Z 2
#define CORNER 3

struct Node {
	// este string contiene 3x8 caracteres, agrupados de a 3 consecutivos. Cada
	// grupo de 3 caracteres corresponde a una esquina del cubo, y cada uno de
	// dichos caracteres corresponden a los colores en la cara ortogonal al eje
	// 'x' (front-back), 'y' (left-right), y 'z' (up-down), respectivamente.
	// Las esquinas están numeradas de la siguiente forma:
	//     0: back-left-up
	//     1: front-left-up
	//     2: front-right-up
	//     3: back-right-up
	//     4: back-right-down
	//     5: front-right-down
	//     6: front-left-down
	//     7: back-left-down
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


typedef Node (*Action)(Node&);

// intercambia 4 posiciones en 'circular fashion'
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

Node F(Node& node) {
	return x_rotation(node, 1, 2, 6, 5);
}

// F'
Node F_(Node& node) {
	return x_rotation(node, 5, 6, 2, 1);
}

Node B(Node& node) {
	return x_rotation(node, 3, 0, 7, 4);
}

// B'
Node B_(Node& node) {
	return x_rotation(node, 4, 7, 0, 3);
}


Node R(Node& node) {
	return y_rotation(node, 2, 3, 4, 5);
}

// R'
Node R_(Node& node) {
	return y_rotation(node, 5, 4, 3, 2);
}

Node L(Node& node) {
	return y_rotation(node, 0, 1, 6, 7);
}

// L'
Node L_(Node& node) {
	return y_rotation(node, 7, 6, 1, 0);
}

Node U(Node& node) {
	return z_rotation(node, 0, 3, 2, 1);
}

// U'
Node U_(Node& node) {
	return z_rotation(node, 1, 2, 3, 0);
}

Node D(Node& node) {
	return z_rotation(node, 4, 7, 6, 5);
}

// U'
Node D_(Node& node) {
	return z_rotation(node, 5, 6, 7, 4);
}

Node x(Node& node) {
	return
}

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
