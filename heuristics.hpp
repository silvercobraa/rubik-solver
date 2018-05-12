#ifndef HEURISTICS_HPP
#define HEURISTICS_HPP

#include <unordered_map>

typedef double (*Heuristic)(Node*);

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


#endif /* end of include guard: HEURISTICS_HPP */
