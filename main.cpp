#include <iostream>
#include <map>

#include "search.hpp"
#include "scramble.hpp"
#include "database.hpp"

using namespace std;

string read_moves() {
	string moves;
	getline(cin, moves);
	return moves;
}

uint64_t string_to_state(string s) {
	static map<char, uint64_t> m = {
		{'b', BLUE},
		{'r', RED},
		{'y', YELLOW},
		{'g', GREEN},
		{'o', ORANGE},
		{'w', WHITE},
	};
	uint64_t result = 0UL;
	for (int i = 0; i < 21; i += 3) {
		uint64_t x = m[s[i]];
		uint64_t y = m[s[i + 1]];
		uint64_t z = m[s[i + 2]];
		uint64_t piece = (x << X) | (y << Y) | (z << Z);
		result |= piece << (3*i);
	}
	return result;
}


int main(int argc, char const *argv[]) {
	if (argc < 2) {
		puts("SE DEBE INGRESAR EL ALGORITMO DE BUSQUEDA EN argv[1]");
		puts("(OPCIONES: bfs, ids, astar, greedy, pd, pdgen)");
		exit(EXIT_FAILURE);
	}
	Node* root = make_root();

	// Heuristic h = heuristic_bad_pieces;
	Heuristic h = heuristic_manhattan_distance;

	root->state = string_to_state("bow""rwb""gwr""ygo""gry""oby""bry");
	// scramble(root, read_moves());
	// random_scramble(root, 5);

	string argument(argv[1]);
	if (argument == "bfs") {
		puts("EJECUTANDO BUSQUEDA EN ANCHURA...");
		bfs(root);
	}
	else if (argument == "ids") {
		puts("EJECUTANDO BUSQUEDA EN PROFUNDIDAD...");
		ids(root);
	}
	else if (argument == "astar") {
		puts("EJECUTANDO BUSQUEDA A*...");
		a_star(root, h);
	}
	else if (argument == "greedy") {
		puts("EJECUTANDO BUSQUEDA GREEDY...");
		greedy(root, h);
	}
	else if (argument == "hpa") {
		puts("EJECUTANDO BUSQUEDA HPA...");
		hpa(root, h);
	}
	else if (argument == "pd") {
		puts("CARGANDO BASE DE DATOS...");
		load_pattern_database("compact_pattern_database.txt");
		puts("...BASE DE DATOS CARGADA");
		pattern_database_search("compact_pattern_database.txt", root);
	}
	else if (argument == "pdgen") {
		cerr << "GENERANDO BASE DE DATOS..." << endl;
		root->state = solved_state;
		generate_pattern_database(root);
	}
	else {
		puts("ORDEN NO RECONOCIDA");
	}
	return 0;
}
