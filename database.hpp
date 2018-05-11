#ifndef PATTERN_DATABASE_H
#define PATTERN_DATABASE_H

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <fstream>

using namespace std;

static bool PATTERN_DATABASE_LOADED = false;
static std::map<std::string,std::string> pattern_database;

static bool visited(unordered_set<string>& s, Node* n)
{
	return s.find(n->state) != s.end();
}


// Se puede almacenar el arbol completo BFS en memoria, pero los caminos no.
// Sin embargo, podemos usar el disco...
void generate_pattern_database(Node* root) {
	unordered_set<string> s;
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
		cout << parent->state;
		for (Node* aux = parent; aux->action != NO_ACTION; aux = aux->parent) {
			cout << ' ' << action_name[reverse_action[aux->action]];
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
	if (pattern_database.find(node->state) != pattern_database.end()) {
		std::cout << pattern_database[node->state] << endl;
		return;
	}
	// Esto no debería ocurrir, nunca.
	// Si entramos acá, es porque, o bien faltan estados en la base de datos,
	// hay un bug en alguna de las rotaciones de cubo completo, o
	// los prefijos están mal ingresados.
	cout << "No se encontró la solución :(" << endl;
}


#endif /* end of include guard: PATTERN_DATABASE_H */
