#ifndef SCRAMBLE_HPP
#define SCRAMBLE_HPP

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <string>
#include <algorithm>

#include "problem.hpp"

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
		cout << action_name[r] << ' ';
		root->state = actions[r](root->state);
	}
	cout << endl;
	return;
}

#endif /* end of include guard: SCRAMBLE_HPP */
