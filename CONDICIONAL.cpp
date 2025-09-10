#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


void validarCondicional(){
    ifstream archivo("archivo.txt"); 
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo." << endl;
        return;
    }

    char c;
    string palabra = "";
    vector<string> tokens;

    while (archivo.get(c)) {
        if (c == '\n' || c == '\r') {
            continue; // ignorar saltos de línea
        }

        if (c == ' ' || c == '(' || c == ')' || c == '{' || c == '}' || c == ';') {
            // Se guarda una palabra en tokens si se encuentra un espacio o símbolo
            if (!palabra.empty()) {
                tokens.push_back(palabra);
                palabra.clear();
            }
            // Se guardan los separadores como tokens
            if (c == '(' || c == ')' || c == '{' || c == '}') {
                string separador(1, c);// se convierte char a string
                tokens.push_back(separador);
            }
        } 
        
        else if (c == '=' || c == '!') {
            if (!palabra.empty()) { // se guarda en un token los caracteres previos
                tokens.push_back(palabra);
                palabra.clear();
            }

            char next;
            if (archivo.peek() == '=') {// verifica si el siguiente carácter es '='
                archivo.get(next);
                string operador;
                operador += c;
                operador += next;
                tokens.push_back(operador);
            } else {
                string operador(1, c);
                tokens.push_back(operador);
            }
        } 
        else if (c == '<' || c == '>') {
            if (!palabra.empty()) {
                tokens.push_back(palabra);
                palabra.clear();
            }
            string operador(1, c);
            tokens.push_back(operador);
        }
        else { // Se va formando la palabra
            palabra += c; 
        }
    }
    if (!palabra.empty()) {  
        tokens.push_back(palabra); // guardar la última palabra si existe
    }
    archivo.close();


    // VERIFICANDO: if ( expr relop expr ) { instrucciones } else { instrucciones }
    bool valido = true;
    size_t i = 0;
    
    if (i >= tokens.size() || tokens[i] != "if") {
        valido = false;
    }
    i++;

    if (valido && (i >= tokens.size() || tokens[i] != "(")) {
        valido = false;
    }
    i++;

    // expr relop expr
    if (valido && i+2 < tokens.size()) {
        string expr1 = tokens[i];
        string relop = tokens[i+1];
        string expr2 = tokens[i+2];

        vector<string> relops = {"<", ">", "==", "!="};
        bool opValido = false;
        for (auto &r : relops) {// r toma el valor de cada elemento del vector relops
            if (relop == r) opValido = true;
        }
        if (!opValido) valido = false;

        i += 3;
    } else {
        valido = false;
    }

    if (valido && (i >= tokens.size() || tokens[i] != ")")) {
        valido = false;
    }
    i++;

    if (valido && (i >= tokens.size() || tokens[i] != "{")) {
        valido = false;
    }

    // Se salta las instrucciones del if hasta encontrar }
    
    while (valido && i < tokens.size() && tokens[i] != "}") i++;

    if (valido && (i >= tokens.size() || tokens[i] != "}")) {
        valido = false;
    }
    i++;

    if (valido && (i >= tokens.size() || tokens[i] != "else")) {
        valido = false;
    }
    i++;

    if (valido && (i >= tokens.size() || tokens[i] != "{")) {
        valido = false;
    }
    // Se salta las instrucciones del else hasta encontrar una }
    while (valido && i < tokens.size() && tokens[i] != "}")  i++;

    if (valido && (i >= tokens.size() || tokens[i] != "}")) {
        valido = false;
    }
    i++;

    //Resultado
    if (valido && i == tokens.size()) {
        cout << "Valido: Si cumple la gramatica condicional." << endl;
    } else {
        cout << "Invalido: no cumple la gramatica condicional" << endl;
    }
}

int main() {
    validarCondicional();
    return 0;
}

