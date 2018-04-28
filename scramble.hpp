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
		root->state = action_by_name[move](root->state);
	}
	return;
}

Node random_scramble(Node root, int moves) {
	// TODO: implementar esta wea
	Node scrambled;
	return scrambled;
}

#endif /* end of include guard: SCRAMBLE_HPP */
