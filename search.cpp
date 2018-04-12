#include <iostream>
#include <sstream>
#include <unistd.h>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <chrono>
#include <random>
#include <queue>

#include "rotations.hpp"

using namespace std;


set<string> visited;

void print_trace(Node* node) {
	while (node != NULL) {
		print(*node);
		node = node->parent;
	}
}

bool dfs(Node& node, int depth, int max_depth) {
	// print(node);
	cout << node.state << endl;
	cout << "depth: " << depth << endl;
	if (depth > max_depth) {
		return false;
	}
	if (visited.find(node.state) != visited.end()) {
		return false;
	}
	if (goal_test(node)) {
		cout << "REACHED GOAL STATE" << endl;
		print_trace(&node);
		return true;
		// exit(EXIT_SUCCESS);
	}
	// cout << "visiting " << node.state << endl;
	visited.insert(node.state);
	cout << visited.size() <<  endl;

	// act.first: el nombre de la acción
	// act.second: el puntero a la función
	for (auto act : actions) {
		Node child_node = act.second(node);
		if (dfs(child_node, depth + 1, max_depth)) {
			cout << act.first << endl;
			return true;
		}
	}
	return false;
}

// TODO: imprimir correctamente la traza de movimientos: "move[s]"
void print_trace(map<string, string>& parent, map<string, string>& move, string s) {
	while (s != "") {
		// cout << s << endl;
		Node n = {s, NULL};
		cout << move[s] << " ";
		s = parent[s];
		// print(n);
	}
	cout << endl;
}


bool bfs(Node& node) {
	queue<Node> q;
	set<string> v; // los estados visitados
	map<string, string> move; // el movimiento con el cual se llego a este estado
	map<string, string> parent_state; // el estado anterior

	q.push(node);
	parent_state[node.state] = "";
	move[node.state] = "";

	while(!q.empty()) {
		cout << v.size() << endl;
		Node parent = q.front();
		q.pop();
		v.insert(parent.state);
		if (goal_test(parent)) {
			cout << "REACHED GOAL STATE" << endl;
			print_trace(parent_state, move, parent.state);
			return true;
		}
		for (auto act : actions) {
			auto child = act.second(parent);
			if (v.find(child.state) == v.end()) {
				move[child.state] = act.first;
				// cout << child.state << " -> " << act.first << endl;
				parent_state[child.state] = parent.state;
				// print(parent);
				// cout << act.first << endl;
				// print(child);
				q.push(child);
			}
		}
	}
	cout << "DIDN'T REACH GOAL STATE" << endl;
	return false;
}

// iterative deepening search
void ids(Node& root) {
	int max_depth = 1;
	bool goal_state = false;
	while (!goal_state) {
		goal_state = dfs(root, 0, max_depth);
		max_depth += 1;
		visited.clear();
	}
}

Node scramble(Node root, string moves) {
	auto ss = stringstream(moves);
	string move;
	while (ss >> move) {
		cout << move << endl;
		root = actions[move](root);
		cout << root.state << endl;
	}
	return root;
}

void generate_all_states(Node& n) {
	set<string> s;
	queue<Node> q;
	q.push(n);
	while (!q.empty()) {
		auto parent = q.front();
		q.pop();
		s.insert(parent.state);
		cout << s.size() << endl;
		for (auto act: actions) {
			auto child = act.second(parent);
			if (s.find(child.state) == s.end()) {
				q.push(child);
			}
		}
	}
}

int main(int argc, char const *argv[]) {
	string initial_state = solved_state;
	Node root {initial_state, NULL};
	root = scramble(root, "D  L  R'  F'  R ");
	print(root);
	// sleep(5);
	// ids(root);
	bfs(root);
	// dfs(root, 0, 4);
	// for (auto s: visited) {
	// 	cout << s << ' ';
	// }
	//set<string> s;
	//generate_all_states(root);
	return 0;
}
