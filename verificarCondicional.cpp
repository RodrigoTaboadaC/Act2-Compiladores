// verificarCondicionales.cpp
#include "verificarIdentificador.hpp"   // esIdentificadorValido
#include "verificarDeclaracion.hpp"     // esNumero
#include <vector>
#include <string>
#include <iostream>
using namespace std;

// if ( expr relop expr ) { ... } else { ... }
// relop ∈ { <, >, ==, != }
bool validarCondicional(const vector<string>& tokens, size_t& i) {
    size_t start = i;
    bool valido = true;

    if (i >= tokens.size() || tokens[i] != "if") { i = start; return false; }
    i++;

    if (i >= tokens.size() || tokens[i] != "(") { i = start; return false; }
    i++;

    // expr relop expr  (expr: identificador válido o número)
    auto esExprSimple = [&](const string& s){
        return esIdentificadorValido(s) || esNumero(s);
    };

    if (!(i + 2 < tokens.size() && esExprSimple(tokens[i]))) { i = start; return false; }
    string expr1 = tokens[i], relop = tokens[i+1], expr2 = tokens[i+2];

    bool opValido = (relop == "<" || relop == ">" || relop == "==" || relop == "!=");
    if (!opValido || !esExprSimple(expr2)) { i = start; return false; }
    i += 3;

    if (i >= tokens.size() || tokens[i] != ")") { i = start; return false; }
    i++;

    if (i >= tokens.size() || tokens[i] != "{") { i = start; return false; }
    // Consumir hasta la primera '}' (misma lógica que tu versión original)
    while (i < tokens.size() && tokens[i] != "}") i++;
    if (i >= tokens.size() || tokens[i] != "}") { i = start; return false; }
    i++;

    if (i >= tokens.size() || tokens[i] != "else") { i = start; return false; }
    i++;

    if (i >= tokens.size() || tokens[i] != "{") { i = start; return false; }
    while (i < tokens.size() && tokens[i] != "}") i++;
    if (i >= tokens.size() || tokens[i] != "}") { i = start; return false; }
    i++;

    cout << "Valido: Si cumple la gramatica condicional." << endl;
    return valido;
}
