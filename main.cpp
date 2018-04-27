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
	string initial_state = solved_state;
	Node root {initial_state, NULL};
	string moves = read_moves();
	root = scramble(root, moves);
	print(root);
	A_star(root);
	// sleep(5);
	// ids(root);
	// bfs(root);
	// dfs(root, 0, 3);
	// for (auto s: visited) {
	// 	cout << s << ' ';
	// }
	//set<string> s;
	// generate_all_states(root);
	// for (auto x: actions) {
	// 	auto n1 = x.second(root);
	// 	for (auto y: actions) {
	// 		auto n2 = y.second(n1);
	// 		for (auto z: actions) {
	// 			auto n3 = z.second(n2);
	// 			cout << heuristic(n3) << '\t' << n2.state << endl;
	// 		}
	// 	}
	// }
	return 0;
}
