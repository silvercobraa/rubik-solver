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
	dfs(root, 0, 4);

	Node* child = child_node(root, 0);
	printf("%p %p %p\n", root, child, child->parent);
	// ids(root);
	// bfs(root);
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
