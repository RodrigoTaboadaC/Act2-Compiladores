#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <set>
#include "verificarIdentificador.hpp"
#include "verificarDeclaracion.hpp"
#include "verificarCondicional.hpp"
#include "verificarExprAritmetica.hpp"

using namespace std;

vector<string> tokenizarArchivo(const string& ruta) {
    ifstream archivo(ruta);
    vector<string> tokens;
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo." << endl;
        return tokens;
    }

    char c;
    string palabra, operador;

    while (archivo.get(c)) {
        if (c == '\n' || c == '\r') {
            continue;//ignorar saltos de línea
        }
        //Espacios y tabulaciones cortan tokens
        if (c == ' ' || c == '\t') {
            if (!palabra.empty()) {
                tokens.push_back(palabra);
                palabra.clear();
            }
            continue;
        }
        
        // '=' y '!' (para ==, !=)
        else if (c == '=' || c == '!') {
            if (!palabra.empty()) { // se guarda en un token los caracteres previos
                tokens.push_back(palabra);
                palabra.clear();
            }

            if (archivo.peek() == '=') {
                char next; 
                operador.clear();          
                archivo.get(next);
                operador += c;
                operador += next;
                tokens.push_back(operador);
            } else {
                string op(1, c);
                tokens.push_back(op);
            }
            continue;
        }

        // Separadores y operadores
        if (c == '(' || c == ')'|| c == '{' || c == '}' || c == ';' || c == '+' || c == '-' ||
            c == '*' || c == '/' || c == '<' || c == '>') {
            if (!palabra.empty()) {
                tokens.push_back(palabra);
                palabra.clear();
            }
            string s(1, c);
            tokens.push_back(s);
            continue;
        }
        else{
            palabra += c;
        }
    }
    if (!palabra.empty()) {  
        tokens.push_back(palabra); // guardar la última palabra si existe
    }
    archivo.close();
    return tokens;
}

int main() {
    vector<string> tokens = tokenizarArchivo("archivo.txt");

    size_t i = 0;

    if (!validarDeclaracion(tokens, i)) {
        cout << "Invalido: no cumple la gramatica de declaraciones." << endl;
        return 0;
    }
    if (!validarCondicional(tokens, i)) {
        cout << "Invalido: no cumple la gramatica condicional." << endl;
        return 0;
    }
    if (i == tokens.size()) {
        cout << "Analisis completado, cumple con todas las gramaticas" << endl;
    }

    return 0;
}
