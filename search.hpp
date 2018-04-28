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
#include <fstream>

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

static bool visited(set<string>& s, Node* n)
{
	string n1 = z(n->state);
	string n2 = z_(n->state);
	string n3 = z(n1);

	string n4 = x(n->state);
	string n5 = z(n4);
	string n6 = z_(n4);
	string n7 = z(n5);

	string n8 = x_(n->state);
	string n9 = z(n8);
	string n10 = z_(n8);
	string n11 = z(n9);

	string n12 = x(n4);
	string n13 = z(n12);
	string n14 = z_(n12);
	string n15 = z(n13);

	string n16 = y(n->state);
	string n17 = z(n16);
	string n18 = z_(n16);
	string n19 = z(n17);

	string n20 = y_(n->state);
	string n21 = z(n20);
	string n22 = z_(n20);
	string n23 = z(n21);
	vector<string> v = {n->state, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14, n15, n16, n17, n18, n19, n20, n21, n22, n23};
	 for (auto state: v) {
	 	if (s.find(state) != s.end()) {
			// cout << "true" << endl;
			return true;
		}
	}
	// cout << "false" << endl;
	return false;
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


void load_pattern_database(std::string pattern_database_file, std::map<std::string,std::string>& pd) {
	string s;
	std::ifstream file(pattern_database_file);
	file >> s;
	pd[s] = "ALREADY SOLVED";
	std::string state, moves;
	while (file >> state >> moves) {
		pd[state] = moves;
		// cout << pd.size() << endl;
	}
}


void pattern_database_search(std::string pattern_database_file, Node* node) {
	static bool pattern_database_loaded = false;
	static std::map<std::string,std::string> pattern_database;
	if (!pattern_database_loaded) {
		load_pattern_database(pattern_database_file, pattern_database);
		pattern_database_loaded = true;
	}
	if (pattern_database.find(node->state) != pattern_database.end()) {
		cout << "SOLUTION:" << endl;
		cout << pattern_database[node->state] << endl;
	}
	else {
		// como manejamos las simetrias?
		// agrego cada simetria a la base de datos (x24 el tamalño actual)?
		// o genero las rotaciones y busco 24 veces en la BD?
		cout << ":(" << endl;
	}

}

#endif /* end of include guard: SEARCH_HPP */
