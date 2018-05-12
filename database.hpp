#ifndef PATTERN_DATABASE_H
#define PATTERN_DATABASE_H

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <unordered_map>

#include "search.hpp"

static bool PATTERN_DATABASE_LOADED = false;
static std::unordered_map<State,std::string> pattern_database;

static bool visited(Set& s, Node* n)
{
	return s.find(n->state) != s.end();
}


// Se puede almacenar el arbol completo BFS en memoria, pero los caminos no.
// Sin embargo, podemos usar el disco...
void generate_pattern_database(Node* root) {
	Set s;
	Queue q;
	q.push(root);
	while (!q.empty()) {
		auto parent = q.front();
		q.pop();
		if (visited(s, parent)) {
			continue;
		}
		// para cachar mas o menos como va imprimo cada 1000 nodos visitados
		if (s.size() % 1000 == 0) {
			std::cerr << s.size() << std::endl;
		}
		s.insert(parent->state);
		// std::cout << s.size() << std::endl;
		std::cout << parent->state;
		std::cout << ' ';
		for (Node* aux = parent; aux->action != NO_ACTION; aux = aux->parent) {
			std::cout << action_name[reverse_action[aux->action]];
		}
		std::cout << std::endl;
		for (int act = 0; act < actions.size(); act++) {
			Node* child = child_node(parent, act);
			if (!visited(s, child)) {
				q.push(child);
			}
		}
	}
}


void load_pattern_database(std::string pattern_database_file) {
	State s;
	std::ifstream file(pattern_database_file);
	file >> s;
	pattern_database[s] = "ALREADY SOLVED";
	State state;
	std::string moves;
	while (file >> state >> moves) {
		pattern_database[state] = moves;
		// std::cout << pattern_database.size() << std::endl;
	}
	PATTERN_DATABASE_LOADED = true;
}


void pattern_database_search(std::string pattern_database_file, Node* node) {

	if (!PATTERN_DATABASE_LOADED) {
		load_pattern_database(pattern_database_file);
	}
	if (pattern_database.find(node->state) != pattern_database.end()) {
		std::cout << pattern_database[node->state] << std::endl;
		return;
	}
	// Esto no debería ocurrir, nunca.
	// Si entramos acá, es porque, o bien faltan estados en la base de datos,
	// hay un bug en alguna de las rotaciones de cubo completo, o
	// los prefijos están mal ingresados.
	std::cout << "No se encontró la solución :(" << std::endl;
}


#endif /* end of include guard: PATTERN_DATABASE_H */
