#include "Hecho.h"

Hecho::Hecho(string hecho, float certeza) {
	h = hecho;
	fc = certeza;

}
Hecho::~Hecho() {}

string Hecho::getH() {
	return h;
}

float Hecho::getFc() {
	return fc;
}