#include <fstream>
#include <string>
#include "Parseador.h"
#define ESPACIO " "
#define ESPACIO_TAM 1

//Función auxiliar para trocear una cadena por sus espacios.
vector<string> obtenerPalabras(string s) {
    size_t pos_start = 0, pos_end, delim_len = ESPACIO_TAM;
    string token;
    vector<string> res;

    while ((pos_end = s.find(ESPACIO, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

/*Funcion que construye la base de hechos.
Abre el fichero de BH y lo va recorriendo, obteniendo los hechos y
parseandolos uno a uno recolectando el literal y su FC, para almacenarlos
en un vector de objetos Hecho. Por ultimo lee el objetivo del problema y lo almacena.*/
vector<Hecho> Parseador::construirBh(string pathBh) {
    vector<Hecho> bh;
    ifstream fileBH(pathBh);
    string lineaAct;
    getline(fileBH, lineaAct);
    int numHechos = stoi(lineaAct);

    for (int i = 0; i < numHechos; i++) {
        getline(fileBH, lineaAct);
        vector<string> palabras = obtenerPalabras(lineaAct);
        string literal = palabras[0].substr(0, (palabras[0].size() - 1));
        float factorCerteza = stof(palabras[1].substr(3, (palabras[1].size())));
        Hecho h(literal, factorCerteza);
        bh.push_back(h);
    }
    getline(fileBH, lineaAct);
    getline(fileBH, lineaAct);
    objetivo = lineaAct;
    fileBH.close();
    return bh;
}

/*Funcion que construye la base de conocimientos.
Abre el fichero de BC y lo va recorriendo, obteniendo las reglas y
parseandolas una a una recolectando toda la información, para almacenarlas
en un vector de objetos Regla*/
vector<Regla> Parseador::construirBc(string pathBc) {
    vector<Regla> bc;
    ifstream fileBC(pathBc);
    string lineaAct;
    getline(fileBC, lineaAct);
    int numReglas = stoi(lineaAct);

    for (int i = 0; i < numReglas; i++) {
        getline(fileBC, lineaAct);
        vector<string> palabras = obtenerPalabras(lineaAct);
        vector<string> literalesAlpha;
        string literalBeta;
        bool beta = false;
        int and_or = UNICO;
        for (unsigned int i = 2; i < (palabras.size()-1); i++)
        {
            if (palabras[i].compare("Entonces") == 0) {
                beta = true;
            }
            else if (palabras[i].compare("y") == 0) {
                and_or = CONJUNCION;
            }
            else if (palabras[i].compare("o") == 0) {
                and_or = DISYUNCION;
            }
            else {
                if (beta) {
                    literalBeta = palabras[i].substr(0, (palabras[i].size() - 1));
                }
                else {
                    literalesAlpha.push_back(palabras[i]);
                }
            }
        }
        float factorCerteza = stof(palabras[palabras.size() - 1].substr(3, (palabras[palabras.size() - 1].size())));
        string nombre = palabras[0].substr(0, palabras[0].size() - 1);
        Regla r(nombre, literalesAlpha, literalBeta, and_or, factorCerteza);
        bc.push_back(r);
    }
    fileBC.close();
    return bc;
}

Parseador::Parseador(string pathBc, string pathBh) {
    objetivo = "";
    bc = construirBc(pathBc);
    bh = construirBh(pathBh);
}

Parseador::~Parseador(){}

vector<Hecho> Parseador::getBh() {return bh;}

vector<Regla> Parseador::getBc() {return bc;}

string Parseador::getObjetivo() {return objetivo;}
