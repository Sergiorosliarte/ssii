/* Clase que representa un hecho de la base de hechos.
* Almacena el literal del hecho y el factor de certeza que tiene asociado.
*/

#ifndef HECHO_H
#define HECHO_H
#include <iostream>
using namespace std;

class Hecho {
private:
	string h;
	float fc;

public:
	Hecho(string hecho, float certeza);
	~Hecho();
	string getH();
	float getFc();

};

#endif
