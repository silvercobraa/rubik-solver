#ifndef NODE_HPP
#define NODE_HPP

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

typedef uint64_t State;

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


#endif /* end of include guard: NODE_HPP */
