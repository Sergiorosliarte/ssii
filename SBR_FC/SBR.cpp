#include <iostream>
#include <fstream>
#include <list>
#include <cmath>
#include "Hecho.h"
#include "Regla.h"
#include "Parseador.h"
#define DEF 10
using namespace std;

const int CASO_UNO = 1;
const int CASO_DOS = 2;
const int CASO_TRES = 3;

//Función para dar formato a un vector de literales, para imprimirlos en el log.
string vectorToString(int caso, vector<string> literales) {
	string op = " ";
	if (caso == CONJUNCION)
		op += "y ";
	else if (caso == DISYUNCION)
		op += "o ";

	string res = literales[0];
	for (unsigned int i = 1; i < literales.size(); i++)
		res = res + op + literales[i];

	return res;
}

//Función de equiparación. Obtiene todas las reglas cuyo consecuente es meta y las devuelve.
list<Regla> equiparar(vector<Regla> bc, string meta) {
	list<Regla> aplicables;
	for (unsigned int i = 0; i < bc.size(); i++) {
		if (bc[i].getB().compare(meta) == 0)
			aplicables.push_back(bc[i]);
	}
	return aplicables;
}

//Devuelve la siguiente regla a ser analizada.
Regla resolver(list<Regla> cc) {
	return cc.front();
}

//Comprueba si un literal esta contenido en la BH.
//Si es así, devuelve su FC, de lo contrario devuelve NULL.
float contenido(vector<Hecho> bh, string meta) {
	for (unsigned int i = 0; i < bh.size(); i++) {
		if (bh[i].getH().compare(meta) == 0)
			return bh[i].getFc();
	}
	return 12.0f;
}

//Función que combina dos FC, según el caso que sea, 1, 2 o 3.
float combinarParFCs(float f1, float f2, int caso, int and_or) {
	switch (caso) {
	case CASO_UNO:
		if (and_or == CONJUNCION)
			return min(f1, f2);
		else if (and_or == DISYUNCION)
			return max(f1, f2);
		else return f1;
		break;

	case CASO_DOS:
		if (f1 >= 0.0f && f2 >= 0.0f)
			return f1 + (f2 * (1 - f1));
		else if (f1 <= 0.0f && f2 <= 0.0f)
			return f1 + (f2 * (1 + f1));
		else return (f1 + f2) / (1 - min(abs(f1), abs(f2)));
		break;

	case CASO_TRES:
		return (max(0.0f, f1) * f2);
		break;

	default:
		return 10.0f;
		break;
	}
}

//Función auxiliar a combinarParFCs para cuando se hayan de combinar mas de 2 FC o solo 1.
float combinarFCs(vector<float> fcs, int caso, int and_or = DEF) {
	switch (fcs.size()) {
	case 0:
		return -1.0f;
		break;
	case 1:
		return fcs[0];
		break;
	case 2:
		return combinarParFCs(fcs[0], fcs[1], caso, and_or);
	default:
		float res = fcs[0];
		for (unsigned int i = 1; i < fcs.size(); i++)
			res = combinarParFCs(res, fcs[i], caso, and_or);
		return res;
		break;
	}
}

/* Función principal del motor de inferencia. Primero comprueba si la meta esta en la BH.
* Si es así devuelve su FC. De lo contrario crea el cc equiparando la BC y la meta.
* Después, para cada regla en el cc, obtiene todos sus antecedentes y los verifica.
* Después de verificarlos unifica sus FC (por el caso 1).
* Después, aplica el caso 3, para obtener el FC del consecuente de esta regla, y lo almacena.
* Cuando termina con todas las reglas unifica sus FC por el caso 2, y devuelve el resultado.
*/
float verificar(vector<Hecho> bh, vector<Regla> bc, string meta, ofstream *ficheroLog) {
	float f = contenido(bh, meta);
	if (f <= 2.0f)
		return f;

	list<Regla> cc = equiparar(bc, meta);
	int cc_size = (int) cc.size();
	vector<float> fcs_reg;
	for (int i = 0; i < cc_size; i++)
	{
		Regla r = resolver(cc);
		cc.pop_front();
		*ficheroLog << r.getNombre() << ": Si " << vectorToString(r.getAndOr(),r.getA())
			<<" Entonces " << r.getB() << ", FC=" << r.getFc() << endl;
		vector<string> nuevasMetas = r.getA();
		vector<float> fcs_lit;
		for (unsigned int j = 0; j < nuevasMetas.size(); j++) {
			string nMeta = nuevasMetas[j];
			float resNMeta = verificar(bh, bc, nMeta, ficheroLog);
			if (contenido(bh, nMeta) > 2.0f)
				bh.push_back(Hecho(nMeta, resNMeta));
			fcs_lit.push_back(resNMeta);
		}
		float caso1 = combinarFCs(fcs_lit, CASO_UNO, r.getAndOr());
			*ficheroLog << "\tCaso 1: " << vectorToString(r.getAndOr(), nuevasMetas) << ", FCs=" << fcs_lit[0];
			if (fcs_lit.size() >= 2) {
				for (unsigned int i = 1; i < fcs_lit.size(); i++)
					*ficheroLog << ", " << fcs_lit[i];
			}
			*ficheroLog << ". Resultado=" << caso1 << endl;
		vector<float> aux;
		aux.push_back(caso1);
		aux.push_back(r.getFc());
		float caso3 = (combinarFCs(aux, CASO_TRES));
		*ficheroLog << "\tCaso 3: " << r.getB() << ", FC=" << caso1 << " * " << r.getFc() << " = " << caso3 << endl;
		fcs_reg.push_back(caso3);
	}
	float caso2 = combinarFCs(fcs_reg, CASO_DOS);
	if (fcs_reg.size() >= 2)
		*ficheroLog << "Caso 2: " << meta << ", FC=" << caso2 << endl;
	else *ficheroLog << meta << ", FC=" << caso2 << endl;
	return caso2;
}

//Funcion auxiliar para obtener los nombres de los ficheros de BC y BH, sin extensiones ni ruta.
string obtenerNombreFichero(string path) {
	unsigned int posicionContrabarra = (int) path.find_last_of("\\");

	string nombreFichero;
	if (posicionContrabarra != string::npos)
		nombreFichero = path.substr(posicionContrabarra + 1);

	else nombreFichero = path;

	unsigned int posicionPunto = (int) nombreFichero.find_last_of(".");
	if (posicionPunto != string::npos)
		nombreFichero = nombreFichero.substr(0, posicionPunto);

	return nombreFichero;
}

//Funcion principal, construye la base de hechos y de conocimientos, crea el fichero de Log y llama a verificar.
int main(int argc, char *argv[]) {
	Parseador parseadorEntrada(argv[1], argv[2]);
	vector<Hecho> bh = parseadorEntrada.getBh();
	vector<Regla> bc = parseadorEntrada.getBc();
	string nombreBC = obtenerNombreFichero(argv[1]);
	string nombreBH = obtenerNombreFichero(argv[2]);
	string nombreFicheroLog = nombreBC + "_" + nombreBH + ".txt";
	ofstream ficheroLog(nombreFicheroLog);
	ficheroLog << "Base de conocimientos : " << argv[1] << endl;
	ficheroLog << "Base de hechos : " << argv[2] << endl;
	ficheroLog << "Objetivo : " << parseadorEntrada.getObjetivo() << endl << endl;
	float solucion = verificar(bh, bc, parseadorEntrada.getObjetivo(), &ficheroLog);
	ficheroLog << "\nObjetivo: " << parseadorEntrada.getObjetivo() << ",FC=" << solucion;
	ficheroLog.close();
	return 0;
}
