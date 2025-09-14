#include "verificarIdentificador.hpp"
#include <set>
#include <cctype>
#include <string>

using namespace std;

bool esLetra(char c) {
    return isalpha(static_cast<unsigned char>(c)) || c == '_';
}

bool esDigito(char c) {
    return isdigit(static_cast<unsigned char>(c));
}

bool esCaracterValidoIdentificador(char c) {
    return esLetra(c) || esDigito(c) || c == '_';
}

// Lista de palabras reservadas en C++
const set<string> keywords = {
    "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor",
    "bool", "break", "case", "catch", "char", "char16_t", "char32_t", "class",
    "compl", "const", "constexpr", "const_cast", "continue", "decltype",
    "default", "delete", "do", "double", "dynamic_cast", "else", "enum",
    "explicit", "export", "extern", "false", "float", "for", "friend", "goto",
    "if", "inline", "int", "long", "mutable", "namespace", "new", "noexcept",
    "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private",
    "protected", "public", "register", "reinterpret_cast", "return", "short",
    "signed", "sizeof", "static", "static_assert", "static_cast", "struct",
    "switch", "template", "this", "thread_local", "throw", "true", "try",
    "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual",
    "void", "volatile", "wchar_t", "while", "xor", "xor_eq"
};

// Palabras comunes de la biblioteca estándar que no son keywords
const set<string> std_library_words = {
    "std", "cout", "cin", "endl", "cerr", "clog", "string", "vector", "map",
    "set", "list", "array", "tuple", "pair", "printf", "scanf", "malloc",
    "free", "new", "delete", "sizeof", "typedef", "typename"
};

bool esIdentificadorValido(const string& token) {
    // Verificar si está vacío
    if (token.empty()) return false;
    
    // Verificar si es palabra reservada
    if (keywords.find(token) != keywords.end()) return false;
    
    // Verificar si es palabra común de biblioteca
    if (std_library_words.find(token) != std_library_words.end()) return false;
    
    // El primer carácter debe ser letra o guión bajo
    if (!esLetra(token[0])) return false;
    
    // Todos los caracteres deben ser válidos para identificadores
    for (size_t i = 1; i < token.size(); i++) {
        if (!esCaracterValidoIdentificador(token[i])) {
            return false;
        }
    }

    // Verificar longitud máxima (aunque C++ no tiene límite estricto, ponemos uno razonable)
    if (token.size() > 1024) return false;

    return true;
}

