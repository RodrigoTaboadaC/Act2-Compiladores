
#include "verificarDeclaracion.hpp"
#include "verificarIdentificador.hpp"   
#include "verificarExprAritmetica.hpp"  
#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

static bool esTipo(const string& s) {
    return s == "int" || s == "float" || s == "char" || s == "double" || s == "string";
}

bool esNumero(const string& s) {
    if (s.empty()) return false;
    size_t i = 0;
    if (s[0] == '+' || s[0] == '-') i = 1;
    if (i >= s.size()) return false;
    for (; i < s.size(); ++i) {
        if (!isdigit(static_cast<unsigned char>(s[i]))) return false;
    }
    return true;
}

// DECL → tipo ID ;
// DECL → tipo ID = expr ;
bool validarDeclaracion(const vector<string>& tokens, size_t& i) {
    size_t start = i;

    // tipo
    if (i >= tokens.size() || !esTipo(tokens[i])) return false;
    string tipo = tokens[i++];

    // ID
    if (i >= tokens.size() || !esIdentificadorValido(tokens[i])) { i = start; return false; }
    string id = tokens[i++];

    // Declaración simple
    if (i < tokens.size() && tokens[i] == ";") {
        ++i;
        return true;
    }

    // Declaración con inicialización -> tipo ID = expr ;
    if (i < tokens.size() && tokens[i] == "=") {
        ++i;
        // Validar la expresión aritmética
        if (!validarExprAritmetica(tokens, i)) { 
            i = start; 
            return false; }
        // Verificar ';' al final
        if (i >= tokens.size() || tokens[i] != ";") { 
            i = start; 
            return false; 
        }
        ++i;
        return true;
    }

    // No coincidió con ninguna forma válida
    i = start;
    return false;
}
