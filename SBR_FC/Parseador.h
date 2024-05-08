/* Clase que recibe las rutas a los ficheros de base de conocimientos
* y de hechos y los abre y parsea para construir las bases de reglas
* y hechos que usará el algoritmo del programa principal, así como obtener
* también el objetivo del problema.
*/

#ifndef PARSEADOR_H
#define PARSEADOR_H
#include <iostream>
#include <vector>
#include "Hecho.h"
#include "Regla.h"

using namespace std;

class Parseador {
private:
	vector<Hecho> bh;
	vector<Regla> bc;
	string objetivo;
	vector<Hecho> construirBh(string pathBh);
	vector<Regla> construirBc(string pathBc);

public:
	Parseador(string pathBc, string pathBh);
	~Parseador();
	vector<Hecho> getBh();
	vector<Regla> getBc();
	string getObjetivo();
};

#endif