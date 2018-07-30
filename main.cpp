#include <iostream>
#include <map>

#include <unistd.h>

#include "search.hpp"
#include "scramble.hpp"
#include "database.hpp"

using namespace std;

string read_moves() {
	string moves;
	getline(cin, moves);
	return moves;
}

uint64_t string_to_state(string s) {
	static map<char, uint64_t> m = {
		{'b', BLUE},
		{'r', RED},
		{'y', YELLOW},
		{'g', GREEN},
		{'o', ORANGE},
		{'w', WHITE},
	};
	uint64_t result = 0UL;
	for (int i = 0; i < 21; i += 3) {
		uint64_t x = m[s[i]];
		uint64_t y = m[s[i + 1]];
		uint64_t z = m[s[i + 2]];
		uint64_t piece = (x << X) | (y << Y) | (z << Z);
		result |= piece << (3*i);
	}
	return result;
}


// las fotos seran capturadas en orden BLFRUD
// deberia documentar mejor esto...
string faces_to_corners(string s) {
	map<char, int> count;
	for (auto c: s) {
		count[c]++;
	}
	for (auto p: count) {
		if (p.second != 4) {
			cout << "ERROR: HAY " << p.second << " CUADROS DE COLOR '" << p.first << "'\n";
			exit(EXIT_FAILURE);
		}
	}
	string result = string();
	result.resize(21);
	result[5] = s[0];
	result[17] = s[1];
	result[11] = s[2];
	// result[23] = s[3]; // este es el z de la esquina LDB

	result[15] = s[4];
	result[12] = s[5];
	// result[21] = s[6];  // este es el x de la esquina LDB
	result[18] = s[7];

	result[14] = s[8];
	result[2] = s[9];
	result[20] = s[10];
	result[8] = s[11];

	result[0] = s[12];
	result[3] = s[13];
	result[6] = s[14];
	result[9] = s[15];

	result[13] = s[16];
	result[16] = s[17];
	result[1] = s[18];
	result[4] = s[19];

	result[7] = s[20];
	result[10] = s[21];
	// result[19] = s[22];  // este es el y de la esquina LDB
	result[19] = s[22];
	// cout << result << endl;
	return result;
}

int main(int argc, char const *argv[]) {
	if (argc < 2) {
		cerr << "SE DEBE INGRESAR EL ALGORITMO DE BUSQUEDA EN argv[1]" << endl;
		cerr << "(OPCIONES: bfs, ids, astar, greedy, pd, pdgen)" << endl;
		exit(EXIT_FAILURE);
	}
	Node* root = make_root();

	// Heuristic h = heuristic_bad_pieces;
	Heuristic h = heuristic_manhattan_distance;
	string s;
	clock_t begin;
	clock_t end;

	string argument(argv[1]);

	if (argument != "pd") {
		cerr << "HOLA HUMANO, SOY UNA INTELIGENCIA ARTIFICIAL QUE RESUELVE CUBOS DE RUBIK 2x2x2" << endl;
		cerr << "INGRESA EL CUBO QUE DESEES RESOLVER: " << endl;
		cin >> s;
		root->state = string_to_state(faces_to_corners(s));
	}

	if (argument == "bfs") {
		puts("EJECUTANDO BUSQUEDA EN ANCHURA...");
		begin = clock();
		bfs(root);
		end = clock();
	}
	else if (argument == "ids") {
		puts("EJECUTANDO BUSQUEDA EN PROFUNDIDAD...");
		begin = clock();
		ids(root);
		end = clock();
	}
	else if (argument == "astar") {
		puts("EJECUTANDO BUSQUEDA A*...");
		begin = clock();
		a_star(root, h);
		end = clock();
	}
	else if (argument == "greedy") {
		puts("EJECUTANDO BUSQUEDA GREEDY...");
		begin = clock();
		greedy(root, h);
		end = clock();
	}
	else if (argument == "hpa") {
		puts("EJECUTANDO BUSQUEDA HPA...");
		begin = clock();
		hpa(root, h);
		end = clock();
	}
	else if (argument == "pd") {
		cerr << endl << "HOLA HUMANO," << endl;
		cerr << "SOY UNA INTELIGENCIA ARTIFICIAL QUE RESUELVE CUBOS RUBIK 2x2x2" << endl;
		cerr << endl << "POR FAVOR ESPERE UNOS SEGUNDOS..." << endl;
		load_pattern_database("pd.txt");
		cerr << "...CARGA COMPLETADA" << endl;
		cerr << endl << "INGRESA EL CUBO QUE DESEAS RESOLVER: " << endl;
		while ((cin >> s)) {
			root->state = string_to_state(faces_to_corners(s));
			string ans = pattern_database_search("pd.txt", root);
			if (ans != "") {
				cerr << endl << "SOLUCION: " << ans << endl << endl;
				cerr << "ESE CUBO ESTUVO MUY FACIL. POR FAVOR INGRESE UNO MAS DIFICIL" << endl;
			}
			else {
				cerr << "NO SE ENCONTRO SOLUCION. REVISA QUE EL ESTADO DEL CUBO ESTE BIEN INGRESADO" << endl;
			}
		}
		cerr << "INGRESA EL CUBO QUE DESEES RESOLVER: " << endl;
	}
	else if (argument == "pdgen") {
		cerr << "GENERANDO BASE DE DATOS..." << endl;
		root->state = solved_state;
		generate_pattern_database(root);
	}
	else {
		puts("ORDEN NO RECONOCIDA");
	}
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "TIEMPO: " << elapsed_secs << " s" << endl;
	return 0;
}
