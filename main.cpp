#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <string>
#include <algorithm>
#include <random>

#include "search.hpp"
#include "scramble.hpp"
#include "database.hpp"

using namespace std;

string read_moves() {
	string moves;
	getline(cin, moves);
	return moves;
}

Node* make_root() {
	Node* root = new Node();
	root->state = solved_state;
	root->parent = NULL;
	root->cost = 0;
	root->action = NO_ACTION;
	return root;
}


int main(int argc, char const *argv[]) {
	if (argc < 2) {
		cout << "Ingresar algoritmo de busqueda" << endl;
		exit(EXIT_FAILURE);
	}
	Node* root = make_root();
	Heuristic h = heuristic2;
	// root->state = "rgy""brw""ogy""bow""rby""grw""oby";
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
		load_pattern_database("pattern_database.txt");
		cout << "Pattern database loaded" << endl;
		pattern_database_search("pattern_database.txt", root);
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
