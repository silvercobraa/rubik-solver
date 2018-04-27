#ifndef SCRAMBLE_HPP
#define SCRAMBLE_HPP

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <string>
#include <algorithm>
#include <random>

void scramble(Node* root, std::string moves) {
	auto ss = std::stringstream(moves);
	std::string move;
	while (ss >> move) {
		std::cout << move << endl;
		// root = action_by_name[move](root);
		root->state = action_by_name[move](root->state);
		std::cout << root->state << endl;
	}
	return;
}

Node scramble(Node root, int moves) {
	// TODO: implementar esta wea
	Node scrambled;
	return scrambled;
}

#endif /* end of include guard: SCRAMBLE_HPP */
