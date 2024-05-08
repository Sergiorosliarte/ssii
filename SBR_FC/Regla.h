/* Clase que representa una regla de la base de conocimientos.
* Contiene un vector de literales que representa los antecedentes,
* un literal que representa el consecuente, un valor entero que almacena si
* solo hay un antecedente, o en caso de que haya varios si es una conjución
* o una disyunción de literales. Por ultimo, almacena también el valor del FC
* de la regla.
*/

#ifndef REGLA_H
#define REGLA_H
#include <iostream>
#include <vector>
using namespace std;

extern const int UNICO;
extern const int CONJUNCION;
extern const int DISYUNCION;

class Regla {
private:
	string nombre;
	vector<string> a;
	string b;
	int and_or;
	float fc;

public:
	
	Regla(string nombre, vector<string> alpha, string beta, int conjuncion_disyuncion, float certeza);
	~Regla();
	string getNombre();
	vector<string> getA();
	string getPosA(int pos);
	int getAndOr();
	string getB();
	float getFc();
};

#endif
