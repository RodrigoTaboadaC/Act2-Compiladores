#include "verificarExprAritmetica.hpp"
#include "verificarIdentificador.hpp"   // esIdentificadorValido
#include "verificarDeclaracion.hpp"     // esNumero
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool validarExprAritmetica(const vector<string>& tokens, size_t& i) {
    const size_t start = i;

    bool valida = true;           // bandera de validación basica
    int balance = 0;              // balance de paréntesis
    bool esperandoOperando = true;    //es true si se espera un operador
    bool operadorVisto = false;      // al menos un operando visto
    // Vector para saber si dentro de cada '(' hubo expresion válida antes de cerrar
    vector<bool> exprValidaEnParentesis;

    while (i < tokens.size()) {
        const string& expresion = tokens[i];

        //Se termina la expresión antes de ';' 
        if (expresion == ";" && balance == 0 && !esperandoOperando) {
            break;
        }

        // Paréntesis de apertura
        if (expresion == "(") {
            if (!esperandoOperando) { //si no se esperaba un operador, es error
                valida = false;
                break;
            }
            balance++;
            exprValidaEnParentesis.push_back(false); //el false indica que se abre paréntesis sin expr aún
            i++;
            continue;
        }

        // Paréntesis de cierre
        if (expresion == ")") {
            balance--;
            if (balance < 0) { //si se cierra sin abrir parentesis antes, es error
                valida = false;
                break;
            }
            // No se puede cerrar si se esperaba operando (p.ej., "(1+ )" o "()")
            if (esperandoOperando) {
                valida = false;
                break;
            }
            //antes que se cierre parentesis debió de haber al menos un operando
            if (!exprValidaEnParentesis.empty()) {//si hay paréntesis abiertos
                bool hubo = exprValidaEnParentesis.back();
                exprValidaEnParentesis.pop_back();
                if (!hubo) {//si no hubo operando dentro, es error
                    valida = false; // paréntesis vacíos "()"
                    break;
                }
            }
            i++;
            continue;
        }

        // Operadores aritméticos
        if (expresion == "+" || expresion == "-" || expresion == "*" || expresion == "/") {

            if (esperandoOperando) {
                // si se esperaba operando, solo + o - son válidos, +1 o -1 por ej.
                if (expresion == "+" || expresion == "-") {
                    i++;         
                    continue;     // seguimos esperando operando
                } else {
                    valida = false;
                    break;
                }
            } else {
                esperandoOperando = true; //despues del operador, esperamos operando
                i++;
                continue;
            }
        }

        // Número entero
        if (esNumero(expresion)) {
            operadorVisto = true;
            if (!exprValidaEnParentesis.empty()) {
                exprValidaEnParentesis.back() = true;
            }
            esperandoOperando = false; // tras operando, esperamos operador
            i++;
            continue;
        }

        // Identificador válido
        if (esIdentificadorValido(expresion)) {
            cout << "Identificador: " << expresion << endl;
            operadorVisto = true;
            if (!exprValidaEnParentesis.empty()){
                exprValidaEnParentesis.back() = true;
            }
            esperandoOperando = false;
            i++;
            continue;
        }

        // Si hay otro token no reconocido en esta gramática es error
        valida = false;
        break;
    }

    //Debe haber al menos un operando en total
    //Paréntesis completamente balanceados
    //No debe terminar esperando operando (no puede acabar en operador)
    //Si hay paréntesis abiertos, cada uno debe tener al menos un operando dentro
    
    if (!(valida && operadorVisto && balance == 0 && !esperandoOperando && exprValidaEnParentesis.empty())) {
        i = start;
        return false;
    } //si no se cumple alguna condición, es inválida

    return true;
}
