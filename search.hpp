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
void generate_pattern_database(Node* root) {
	set<string> s;
	queue<Node*> q;
	q.push(root);
	while (!q.empty()) {
		auto parent = q.front();
		q.pop();
		if (visited(s, parent)) {
			continue;
		}
		// para cachar mas o menos como va imprimo cada 1000 nodos visitados
		if (s.size() % 1000 == 0) {
			cerr << s.size() << endl;
		}
		s.insert(parent->state);
		// cout << s.size() << endl;
		cout << parent->state << ' ';
		for (Node* aux = parent; aux->action != NO_ACTION; aux = aux->parent) {
			cout << action_name[reverse_action[aux->action]];
		}
		cout << endl;
		for (int act = 0; act < actions.size(); act++) {
			Node* child = child_node(parent, act);
			// if (!visited(s, child)) {
				q.push(child);
			// }
		}
	}
}

static bool PATTERN_DATABASE_LOADED = false;
static std::map<std::string,std::string> pattern_database;

void load_pattern_database(std::string pattern_database_file) {
	string s;
	std::ifstream file(pattern_database_file);
	file >> s;
	pattern_database[s] = "ALREADY SOLVED";
	std::string state, moves;
	while (file >> state >> moves) {
		pattern_database[state] = moves;
		// cout << pattern_database.size() << endl;
	}
	PATTERN_DATABASE_LOADED = true;
}


void pattern_database_search(std::string pattern_database_file, Node* node) {

	if (!PATTERN_DATABASE_LOADED) {
		load_pattern_database(pattern_database_file);
	}
	string n = node->state;
	string n1 = z(n);
	string n2 = z_(n);
	string n3 = z(n1);

	string n4 = x(n);
	string n5 = z(n4);
	string n6 = z_(n4);
	string n7 = z(n5);

	string n8 = x_(n);
	string n9 = z(n8);
	string n10 = z_(n8);
	string n11 = z(n9);

	string n12 = x(n4);
	string n13 = z(n12);
	string n14 = z_(n12);
	string n15 = z(n13);

	string n16 = y(n);
	string n17 = z(n16);
	string n18 = z_(n16);
	string n19 = z(n17);

	string n20 = y_(n);
	string n21 = z(n20);
	string n22 = z_(n20);
	string n23 = z(n21);
	vector<string> v = {n, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14, n15, n16, n17, n18, n19, n20, n21, n22, n23};
	// este vector contiene las rotaciones de cubo completo previas que hay que
	// hacer para encontrar el estado en la base de datos.
	static vector<string> prefix = {
		"", "z", "z'", "zz",
		"x", "xz", "xz'", "xzz",
		"x", "z'x", "zx", "z'z'x",
		"xx", "xxz", "xxz'", "xxzz",
		"y", "yz", "yz'", "yzz",
		"y'", "y'z", "y'z'", "y'zz",
	};
	 for (int i = 0; i < v.size(); i++) {
		if (pattern_database.find(v[i]) != pattern_database.end()) {
			cout << prefix[i] << pattern_database[v[i]] << endl;
			return;
		}
	}
	// Esto no debería ocurrir, nunca.
	// Si entramos acá, es porque, o bien faltan estados en la base de datos,
	// hay un bug en alguna de las rotaciones de cubo completo, o
	// los prefijos están mal ingresados.
	cout << "No se encontró la solución :(" << endl;
}

#endif /* end of include guard: SEARCH_HPP */
