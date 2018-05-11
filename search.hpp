#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <queue>
#include <fstream>

#include "problem.hpp"

using namespace std;


static bool dfs(set<string>& visited, Node* parent, int depth, int max_depth) {
	if (depth > max_depth) {
		return false;
	}
	if (goal_test(parent)) {
		cout << "REACHED GOAL STATE" << endl;
		solution(parent);
		return true;
	}
	cout << depth << ' ' << visited.size() <<  endl;
	for (int act = 0; act < actions.size(); act++) {
		if (act == reverse_action[parent->action]) {
			continue;
		}
		Node* child = child_node(parent, act);
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
	unordered_set<string> v;
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
			// if (act == reverse_action[parent->action]) {
			// 	continue;
			// }
			Node* child = child_node(parent, act);
			if (v.find(child->state) == v.end()) {
				q.push(child);
			}
		}
	}
	cout << "DIDN'T REACH GOAL STATE" << endl;
	return false;
}


// búsqueda generica. root es el nodo raiz y f es la función de evaluación
template <typename Lambda>
bool search(Node* root, Lambda f) {
	set<pair<double,Node*>> frontier;
	set<string> visited; // los estados visitados

	frontier.insert({0, root});

	while(!frontier.empty()) {
		cout << visited.size() << endl;
		// cout << frontier.size() << endl;
		Node* parent = (*frontier.begin()).second;
		frontier.erase(frontier.begin());
		// if (visited.find(parent->state) != visited.end()) {
		// 	continue;
		// }
		visited.insert(parent->state);
		if (goal_test(parent)) {
			cout << "REACHED GOAL STATE" << endl;
			solution(parent);
			return true;
		}
		for (int act = 0; act < actions.size(); act++) {
			if (act == reverse_action[parent->action]) {
				continue;
			}
			Node* child = child_node(parent, act);
			if (visited.find(child->state) == visited.end()) {
				frontier.insert({f(child), child});
			}
		}
	}
	cout << "DIDN'T REACH GOAL STATE" << endl;
	return false;
}


typedef double (*Heuristic)(Node*);

bool greedy(Node* root, Heuristic h) {
	// no tengo ni la más pálida idea de porque necesito colocar un ampersand para que funcione
	auto lambda = [&](Node* n){return h(n);};
	return search(root, lambda);
}


bool a_star(Node* root, Heuristic h) {
	auto lambda = [&](Node* n){return n->cost + h(n);};
	return search(root, lambda);
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


#endif /* end of include guard: SEARCH_HPP */
