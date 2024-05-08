#include "Regla.h"

const int UNICO = 0;
const int CONJUNCION = 1;
const int DISYUNCION = 2;

Regla::Regla(string nom, vector<string> alpha, string beta, int conjuncion_disyuncion, float certeza) {
	nombre = nom;
	a = alpha;
	b = beta;
	and_or = conjuncion_disyuncion;
	fc = certeza;

}
Regla::~Regla() {}

string Regla::getNombre() {return nombre;}

vector<string> Regla::getA() {return a;}

string Regla::getPosA(int pos) {
	return a.at(pos);
}

int Regla::getAndOr() { return and_or;}

string Regla::getB() {return b;}

float Regla::getFc() {return fc;}