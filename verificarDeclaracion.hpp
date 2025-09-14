#pragma once
#include <string>
#include <vector>
using namespace std;
// Valida: DECL → tipo ID ;  |  DECL → tipo ID = expr ;

bool validarDeclaracion(const vector<string>& tokens,size_t& i);

bool esNumero(const string& s);
