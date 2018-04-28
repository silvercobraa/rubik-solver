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

using namespace std;

string read_moves() {
	string moves;
	getline(cin, moves);
	return moves;
}

int main(int argc, char const *argv[]) {
	Node* root = new Node();
	root->state = solved_state;
	root->parent = NULL;
	root->cost = 0;
	root->action = NO_ACTION;

	string moves = read_moves();
	scramble(root, moves);
	print(*root);
	bfs(root);
	// generate_all_states(root);
	return 0;
}
