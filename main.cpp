#include <iostream>

#include "search.hpp"
#include "scramble.hpp"
#include "database.hpp"

using namespace std;

string read_moves() {
	string moves;
	getline(cin, moves);
	return moves;
}


int main(int argc, char const *argv[]) {
	if (argc < 2) {
		cout << "Ingresar algoritmo de busqueda" << endl;
		exit(EXIT_FAILURE);
	}
	Node* root = make_root();
	Heuristic h = heuristic2;
	scramble(root, read_moves());
	// random_scramble(root, 5);
	string argument(argv[1]);
	if (argument == "bfs") {
		bfs(root);
	}
	else if (argument == "ids") {
		ids(root);
	}
	else if (argument == "astar") {
		a_star(root, h);
	}
	else if (argument == "greedy") {
		greedy(root, h);
	}
	else if (argument == "pd") {
		load_pattern_database("compact_pattern_database.txt");
		cout << "Pattern database loaded" << endl;
		pattern_database_search("compact_pattern_database.txt", root);
	}
	else if (argument == "pdgen") {
		root->state = solved_state;
		generate_pattern_database(root);
	}
	else {
		cout << "what" << endl;
	}
	return 0;
}
