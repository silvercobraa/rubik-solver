#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <stack>

#define NO_ACTION -1

// Cada estado esta representado por piezas, y cada pieza por los 3 colores que
// la componen. El estado está representado como un entero de 64 bits, donde cada
// grupo de 9 bits corresponde a una pieza, y cada subgrupo de 3 bits corresponde
// a un color. De las 8 piezas del cubo, 7 se mueven y una es fija. Esta última
// define el sistema de referencia usado para determinar si el cubo está resuelto
// o no. El sistema de referencia es:
//   Eje x: caras derecha (+) e izquierda (-).
//   Eje y: caras superior (+) e inferior (-).
//   Eje z: caras frontal (+) y trasera (-).
// La pieza fija es la izquierda inferior trasera, por lo tanto las acciones
// posible son: R, R', U, U', F y F'
// Así, las demás acciones no necesitan ser representadas, ya que equivalen a
// alguna de las anteriormente descritas, mas un cambio en el sistema de referencia.

// Definición de colores
#define BLUE 0L
#define RED 1L
#define YELLOW 2L
#define GREEN 3L
#define ORANGE 4L
#define WHITE 5L

// Mascaras que entregan los bits pertenecientes a una pieza determinada. Ej: RUF_MASK entrega
// los 9 bits correspondientes a la esquina derecha superior frontal (cuando se hace el & con el estado actual y se hace el desplazamiento correspondiente).
#define RUF 0L
#define RUB 9L
#define RDF 18L
#define RDB 27L
#define LUF 36L
#define LUB 45L
#define LDF 54L

// esta mascara me da los 3 bits menos significativos
#define MAGIC 0x7UL

// esto me da los offset de los colores en cada coordenada en una pieza
#define X 0L
#define Y 3L
#define Z 6L

// El estado se considerará resuelto con la convención de que los colores sean:
// R: BLUE
// U: RED
// F: YELLOW
// L: GREEN
// D: ORANGE
// B: WHITE
const uint64_t solved_state =
	(BLUE << (X + RUF)) |
	(RED << (Y + RUF)) |
	(YELLOW << (Z + RUF)) |

	(BLUE << (X + RUB)) |
	(RED << (Y + RUB)) |
	(WHITE << (Z + RUB)) |

	(BLUE << (X + RDF)) |
	(ORANGE << (Y + RDF)) |
	(YELLOW << (Z + RDF)) |

	(BLUE << (X + RDB)) |
	(ORANGE << (Y + RDB)) |
	(WHITE << (Z + RDB)) |

	(GREEN << (X + LUF)) |
	(RED << (Y + LUF)) |
	(YELLOW << (Z + LUF)) |

	(GREEN << (X + LUB)) |
	(RED << (Y + LUB)) |
	(WHITE << (Z + LUB)) |

	(GREEN << (X + LDF)) |
	(ORANGE << (Y + LDF)) |
	(YELLOW << (Z + LDF))
;

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

struct Node {
	uint64_t state;
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


double heuristic(Node* node) {
	// static std::vector<std::string> solved = {"bry", "brw", "boy", "bow", "gry", "grw", "goy"};
	// int sum = 0;
	// for (int i = 0; i < solved.size(); i++) {
	// 	std::string sub = node->state.substr(i*3, 3);
	// 	std::sort(solved[i].begin(), solved[i].end());
	// 	bool correct_position = false;
	// 	do {
	// 		if (sub == solved[i]) {
	// 			correct_position = true;
	// 			break;
	// 		}
	// 	} while(std::next_permutation(solved[i].begin(), solved[i].end()));
	// 	if (!correct_position) {
	// 		sum++;
	// 	}
	// }
	// return sum / 4.0;
}

// esta matriz me dice por cada pieza y por cada posición a que distancia
// (de manhattan) esta de su posición correcta
// me gustaria hacerlo más elegante...
static std::unordered_map<uint64_t, std::vector<int>> manhattan_distance = {
	{(BLUE << X) | (RED << Y) | (YELLOW << Z), {0, 1, 1, 2, 1, 2, 2, 3}},
	{(BLUE << X) | (RED << Y) | (WHITE << Z), {1, 0, 2, 1, 2, 1, 3, 2}},
	{(BLUE << X) | (ORANGE << Y) | (YELLOW << Z), {1, 2, 0, 1, 2, 3, 1, 2}},
	{(BLUE << X) | (ORANGE << Y) | (WHITE << Z), {2, 1, 1, 0, 3, 2, 2, 1}},
	{(GREEN << X) | (RED << Y) | (YELLOW << Z), {1, 2, 2, 3, 0, 1, 1, 2}},
	{(GREEN << X) | (RED << Y) | (WHITE << Z), {2, 1, 3, 2, 1, 0, 2, 1}},
	{(GREEN << X) | (ORANGE << Y) | (YELLOW << Z), {2, 3, 1, 2, 1, 2, 0, 1}},
	// {"gow", {3, 2, 2, 1, 2, 1, 1, 0}},
	{(RED << X) | (YELLOW << Y) | (BLUE << Z), {0, 1, 1, 2, 1, 2, 2, 3}},
	{(RED << X) | (WHITE << Y) | (BLUE << Z), {1, 0, 2, 1, 2, 1, 3, 2}},
	{(ORANGE << X) | (YELLOW << Y) | (BLUE << Z), {1, 2, 0, 1, 2, 3, 1, 2}},
	{(ORANGE << X) | (WHITE << Y) | (BLUE << Z), {2, 1, 1, 0, 3, 2, 2, 1}},
	{(RED << X) | (YELLOW << Y) | (GREEN << Z), {1, 2, 2, 3, 0, 1, 1, 2}},
	{(RED << X) | (WHITE << Y) | (GREEN << Z), {2, 1, 3, 2, 1, 0, 2, 1}},
	{(ORANGE << X) | (YELLOW << Y) | (GREEN << Z), {2, 3, 1, 2, 1, 2, 0, 1}},

	{(YELLOW << X) | (BLUE << Y) | (RED << Z), {0, 1, 1, 2, 1, 2, 2, 3}},
	{(WHITE << X) | (BLUE << Y) | (RED << Z), {1, 0, 2, 1, 2, 1, 3, 2}},
	{(YELLOW << X) | (BLUE << Y) | (ORANGE << Z), {1, 2, 0, 1, 2, 3, 1, 2}},
	{(WHITE << X) | (BLUE << Y) | (ORANGE << Z), {2, 1, 1, 0, 3, 2, 2, 1}},
	{(YELLOW << X) | (GREEN << Y) | (RED << Z), {1, 2, 2, 3, 0, 1, 1, 2}},
	{(WHITE << X) | (GREEN << Y) | (RED << Z), {2, 1, 3, 2, 1, 0, 2, 1}},
	{(YELLOW << X) | (GREEN << Y) | (ORANGE << Z), {2, 3, 1, 2, 1, 2, 0, 1}},

	{(RED << X) | (BLUE << Y) | (YELLOW << Z), {0, 1, 1, 2, 1, 2, 2, 3}},
	{(RED << X) | (BLUE << Y) | (WHITE << Z), {1, 0, 2, 1, 2, 1, 3, 2}},
	{(ORANGE << X) | (BLUE << Y) | (YELLOW << Z), {1, 2, 0, 1, 2, 3, 1, 2}},
	{(ORANGE << X) | (BLUE << Y) | (WHITE << Z), {2, 1, 1, 0, 3, 2, 2, 1}},
	{(RED << X) | (GREEN << Y) | (YELLOW << Z), {1, 2, 2, 3, 0, 1, 1, 2}},
	{(RED << X) | (GREEN << Y) | (WHITE << Z), {2, 1, 3, 2, 1, 0, 2, 1}},
	{(ORANGE << X) | (GREEN << Y) | (YELLOW << Z), {2, 3, 1, 2, 1, 2, 0, 1}},

	{(BLUE << X) | (YELLOW << Y) | (RED << Z), {0, 1, 1, 2, 1, 2, 2, 3}},
	{(BLUE << X) | (WHITE << Y) | (RED << Z), {1, 0, 2, 1, 2, 1, 3, 2}},
	{(BLUE << X) | (YELLOW << Y) | (ORANGE << Z), {1, 2, 0, 1, 2, 3, 1, 2}},
	{(BLUE << X) | (WHITE << Y) | (ORANGE << Z), {2, 1, 1, 0, 3, 2, 2, 1}},
	{(GREEN << X) | (YELLOW << Y) | (RED << Z), {1, 2, 2, 3, 0, 1, 1, 2}},
	{(GREEN << X) | (WHITE << Y) | (RED << Z), {2, 1, 3, 2, 1, 0, 2, 1}},
	{(GREEN << X) | (YELLOW << Y) | (ORANGE << Z), {2, 3, 1, 2, 1, 2, 0, 1}},

	{(YELLOW << X) | (RED << Y) | (BLUE << Z), {0, 1, 1, 2, 1, 2, 2, 3}},
	{(WHITE << X) | (RED << Y) | (BLUE << Z), {1, 0, 2, 1, 2, 1, 3, 2}},
	{(YELLOW << X) | (ORANGE << Y) | (BLUE << Z), {1, 2, 0, 1, 2, 3, 1, 2}},
	{(WHITE << X) | (ORANGE << Y) | (BLUE << Z), {2, 1, 1, 0, 3, 2, 2, 1}},
	{(YELLOW << X) | (RED << Y) | (GREEN << Z), {1, 2, 2, 3, 0, 1, 1, 2}},
	{(WHITE << X) | (RED << Y) | (GREEN << Z), {2, 1, 3, 2, 1, 0, 2, 1}},
	{(YELLOW << X) | (ORANGE << Y) | (GREEN << Z), {2, 3, 1, 2, 1, 2, 0, 1}},
};



// esta heurística usa la distancia de manhattan de todas las piezas dividida por 4
double heuristic2(Node* node) {
	int sum = 0;
	uint64_t state = node->state;
	for (int i = 0; i < 7; i++) {
		uint64_t mask = ~((~0UL) << 9);
		uint64_t piece = state & mask;
		state >>= 9;
		sum += manhattan_distance[piece][i];
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

typedef uint64_t (*Action)(uint64_t);

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
