#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <queue>

#include "problem.hpp"

using namespace std;


bool dfs(set<string>& visited, vector<string>& path, Node& node, int depth, int max_depth) {
	// print(node);
	if (depth > max_depth) {
		return false;
	}
	// if (visited.find(node.state) != visited.end()) {
	// 	return false;
	// }
	if (goal_test(node)) {
		cout << "REACHED GOAL STATE" << endl;
		for (auto s: path) {
			cout << s << ' ';
		}
		cout << '\n';
		return true;
	}
	// cout << "visiting " << node.state << endl;
	// visited.insert(node.state);
	cout << depth << ' ' << visited.size() <<  endl;

	for (int act = 0; act < actions.size(); act++) {
		// Node child_node = actions[i](node);
		Node child = child_node(node, act);
		path.push_back(action_name[act]);
		if (dfs(visited, path, child, depth + 1, max_depth)) {
			return true;
		}
		path.pop_back();
	}
	return false;
}


bool dfs(Node& node, int depth, int max_depth) {
	set<string> visited;
	vector<string> path;
	return dfs(visited, path, node, depth, max_depth);
}


// TODO: imprimir correctamente la traza de movimientos: "move[s]"
void print_trace(map<string, string>& parent, map<string, string>& move, string s) {
	vector<string> path;
	while (s != "") {
		// cout << s << endl;
		Node n = {s, NULL};
		// cout << move[s] << " ";
		path.push_back(move[s]);
		s = parent[s];
		// print(n);
	}
	while (!path.empty()) {
		cout << path.back() << ' ';
		path.pop_back();
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
		for (int act = 0; act < actions.size(); act++) {
			// auto child = actions[i](parent);
			auto child = child_node(parent, act);
			if (v.find(child.state) == v.end()) {
				move[child.state] = action_name[act];
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

bool A_star(Node& node) {
	set<pair<double,Node>> q;
	set<string> v; // los estados visitados
	map<string, string> move; // el movimiento con el cual se llego a este estado
	map<string, string> parent_state; // el estado anterior

	q.insert({0, node});
	parent_state[node.state] = "";
	move[node.state] = "";

	while(!q.empty()) {
		cout << v.size() << endl;
		// cout << q.size() << endl;
		Node parent = (*q.begin()).second;
		// print(parent);
		q.erase(q.begin());
		v.insert(parent.state);
		if (goal_test(parent)) {
			cout << "REACHED GOAL STATE" << endl;
			print_trace(parent_state, move, parent.state);
			return true;
		}
		// if (v.find(parent.state) != v.end()) {
		// 	continue;
		// }
		for (int act = 0; act < actions.size(); act++) {
			// auto child = actions[i](parent);
			auto child = child_node(parent, act);
			if (v.find(child.state) == v.end()) {
				move[child.state] = action_name[act];
				// cout << child.state << " -> " << act.first << endl;
				parent_state[child.state] = parent.state;
				// print(parent);
				// cout << act.first << endl;
				// print(child);
				// DECOMENTAR ACA PARA BÚSQUEDA GREEDY
				// q.insert({heuristic(child), child});
				// DESCOMENTAR ACA PARA BÚSCQUEDA A*
				q.insert({child.cost + heuristic(child), child});
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
	}
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
		for (int act = 0; act < actions.size(); act++) {
			Node child = child_node(parent, act);
			if (s.find(child.state) == s.end()) {
				q.push(child);
			}
		}
	}
}


#endif /* end of include guard: SEARCH_HPP */
