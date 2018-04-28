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


static bool dfs(set<string>& visited, Node* node, int depth, int max_depth) {
	if (depth > max_depth) {
		return false;
	}
	if (goal_test(node)) {
		cout << "REACHED GOAL STATE" << endl;
		solution(node);
		return true;
	}
	cout << depth << ' ' << visited.size() <<  endl;
	for (int act = 0; act < actions.size(); act++) {
		Node* child = child_node(node, act);
		if (dfs(visited, child, depth + 1, max_depth)) {
			return true;
		}
	}
	return false;
}


bool dfs(Node* root, int depth, int max_depth) {
	set<string> visited;
	return dfs(visited, root, depth, max_depth);
}


bool bfs(Node* root) {
	queue<Node*> q;
	set<string> v;
	q.push(root);

	while(!q.empty()) {
		cout << v.size() << endl;
		Node* parent = q.front();
		q.pop();
		// si el estado del nodo actual ya fue visitado, lo ignoramos
		if (v.find(parent->state) != v.end()) {
			continue;
		}
		v.insert(parent->state);
		if (goal_test(parent)) {
			cout << "REACHED GOAL STATE" << endl;
			solution(parent);
			return true;
		}
		for (int act = 0; act < actions.size(); act++) {
			Node* child = child_node(parent, act);
			if (v.find(child->state) == v.end()) {
				q.push(child);
			}
		}
	}
	cout << "DIDN'T REACH GOAL STATE" << endl;
	return false;
}


bool A_star(Node* root) {
	set<pair<double,Node*>> q;
	set<string> v; // los estados visitados

	q.insert({0, root});

	while(!q.empty()) {
		cout << v.size() << endl;
		// cout << q.size() << endl;
		Node* parent = (*q.begin()).second;
		q.erase(q.begin());
		v.insert(parent->state);
		if (goal_test(parent)) {
			cout << "REACHED GOAL STATE" << endl;
			solution(parent);
			return true;
		}
		// if (v.find(parent.state) != v.end()) {
		// 	continue;
		// }
		for (int act = 0; act < actions.size(); act++) {
			Node* child = child_node(parent, act);
			if (v.find(child->state) == v.end()) {
				// DECOMENTAR ACA PARA BÚSQUEDA GREEDY
				// q.insert({heuristic(child), child});
				// DESCOMENTAR ACA PARA BÚSCQUEDA A*
				q.insert({child->cost + heuristic(child), child});
			}
		}
	}
	cout << "DIDN'T REACH GOAL STATE" << endl;
	return false;
}


// iterative deepening search
void ids(Node* root) {
	int max_depth = 1;
	bool reached_goal_state = false;
	while (!reached_goal_state) {
		reached_goal_state = dfs(root, 0, max_depth);
		max_depth += 1;
	}
}


// Se puede almacenar el arbol completo BFS en memoria, pero los caminos no.
// Sin embargo, podemos usar el disco...
void generate_all_states(Node* root) {
	set<string> s;
	queue<Node*> q;
	q.push(root);
	while (!q.empty()) {
		auto parent = q.front();
		q.pop();
		if (visited(s, parent)) {
			continue;
		}
		if (s.size() % 1000 == 0) {
			cerr << s.size() << endl;
		}
		s.insert(parent->state);
		// cout << s.size() << endl;
		cout << parent->state << ' ';
		solution(parent);
		for (int act = 0; act < actions.size(); act++) {
			Node* child = child_node(parent, act);
			// if (!visited(s, child)) {
				q.push(child);
			// }
		}
	}
	cout << s.size() << endl;
}



#endif /* end of include guard: SEARCH_HPP */
