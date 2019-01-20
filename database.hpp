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
		// printf("%lx", parent->state);
		std::cout << ' ';
		for (Node* aux = parent; aux->action != NULL_ACTION; aux = aux->parent) {
			std::cout << actions[actions[aux->action].inverse].name;
		}
		std::cout << std::endl;
		for (auto action : actions) {
			Node* child = child_node(parent, action);
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


// retorna un string con la secuencia de acciones,
// un mensaje de que ya esta resuelto el cubo (si lo está)
// o un string vacio si no se encontro la solucion.
std::string pattern_database_search(std::string pattern_database_file, Node* node) {

	if (!PATTERN_DATABASE_LOADED) {
		load_pattern_database(pattern_database_file);
	}
	if (pattern_database.find(node->state) != pattern_database.end()) {
		return pattern_database[node->state];
	}
	// Esto no debería ocurrir, nunca.
	// Si entramos acá, es porque, o bien faltan estados en la base de datos,
	// hay un bug en alguna de las rotaciones de cubo completo, o
	// los prefijos están mal ingresados.
	return "";
}


#endif /* end of include guard: PATTERN_DATABASE_H */
