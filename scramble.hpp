#ifndef SCRAMBLE_HPP
#define SCRAMBLE_HPP

#include <iostream>
#include <sstream>
#include <string>

#include "node.hpp"
#include "actions.hpp"

void scramble(Node* root, std::string moves) {
	auto ss = std::stringstream(moves);
	std::string move;
	while (ss >> move) {
		root->state = action_by_name[move](root->state);
	}
	return;
}

void random_scramble(Node* root, int moves) {
	srand(time(0));
	for (int i = 0; i < moves; i++) {
		int r = rand() % actions.size();
		std::cout << actions[r].name << ' ';
		root->state = actions[r].apply_to(root->state);
	}
	std::cout << std::endl;
	return;
}

#endif /* end of include guard: SCRAMBLE_HPP */
