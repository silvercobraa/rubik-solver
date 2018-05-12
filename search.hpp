#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <iostream>
#include <queue>
#include <set>
#include <unordered_set>
#include <unordered_map>

#include "node.hpp"
#include "actions.hpp"
#include "heuristics.hpp"

// a que costo la búsqueda debe detenerse
#define A_STAR_CUTOFF 15.0
#define GREEDY_CUTOFF 30.0

// este set debe ser ordenado
typedef std::set<std::pair<double,Node*>> PriorityQueue;
typedef std::unordered_set<State> Set;
typedef std::queue<Node*> Queue;


static bool dfs(Set& visited, Node* parent, int depth, int max_depth) {
	if (depth > max_depth) {
		return false;
	}
	if (goal_test(parent)) {
		puts("REACHED GOAL STATE");
		solution(parent);
		return true;
	}
	std::cout << depth << ' ' << visited.size() <<  std::endl;
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
	Set visited;
	return dfs(visited, root, depth, max_depth);
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


bool bfs(Node* root) {
	Queue q;
	Set v;
	q.push(root);

	while(!q.empty()) {
		std::cout << v.size() << std::endl;
		Node* parent = q.front();
		q.pop();
		// si el estado del nodo actual ya fue visitado, lo ignoramos
		if (v.find(parent->state) != v.end()) {
			continue;
		}
		v.insert(parent->state);
		if (goal_test(parent)) {
			puts("REACHED GOAL STATE");
			solution(parent);
			return true;
		}
		for (int act = 0; act < actions.size(); act++) {
			if (act == reverse_action[parent->action]) {
				continue;
			}
			Node* child = child_node(parent, act);
			if (v.find(child->state) == v.end()) {
				q.push(child);
			}
		}
	}
	std::cout << "DIDN'T REACH GOAL STATE" << std::endl;
	return false;
}


// búsqueda generica. root es el nodo raiz y f es la función de evaluación
template <typename Lambda>
bool search(Node* root, Lambda f, double cutoff) {
	PriorityQueue frontier;
	Set visited; // los estados visitados

	frontier.insert({0, root});

	while(!frontier.empty()) {
		std::cout << visited.size() << std::endl;
		// std::cout << frontier.size() << std::endl;
		Node* parent = (*frontier.begin()).second;
		frontier.erase(frontier.begin());
		if (parent->cost > cutoff) {
			continue;
		}
		if (visited.find(parent->state) != visited.end()) {
			continue;
		}
		visited.insert(parent->state);
		if (goal_test(parent)) {
			puts("REACHED GOAL STATE");
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
	std::cout << "DIDN'T REACH GOAL STATE" << std::endl;
	return false;
}


bool greedy(Node* root, Heuristic h) {
	// no tengo ni la más pálida idea de porque necesito colocar un ampersand para que funcione
	auto lambda = [&](Node* n){return h(n);};
	return search(root, lambda, GREEDY_CUTOFF);
}


bool a_star(Node* root, Heuristic h) {
	auto lambda = [&](Node* n){return n->cost + h(n);};
	return search(root, lambda, A_STAR_CUTOFF);
}



#endif /* end of include guard: SEARCH_HPP */
