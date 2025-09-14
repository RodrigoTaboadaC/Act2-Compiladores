#include "verificarIdentificador.hpp"
#include <set>
#include <cctype>
#include <string>

// Todo lo interno queda encapsulado en un namespace anónimo
namespace {

bool esLetra(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
}

bool esDigito(char c) {
    return std::isdigit(static_cast<unsigned char>(c));
}

bool esCaracterValidoIdentificador(char c) {
    return esLetra(c) || esDigito(c) || c == '_';
}

// Lista de palabras reservadas en C++
const std::set<std::string> keywords = {
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
// pero no se desean aceptar como identificadores de usuario
const std::set<std::string> std_library_words = {
    "std", "cout", "cin", "endl", "cerr", "clog", "string", "vector", "map",
    "set", "list", "array", "tuple", "pair", "printf", "scanf", "malloc",
    "free", "new", "delete", "sizeof", "typedef", "typename"
};

constexpr std::size_t MAX_ID_LEN = 1024;

} // namespace

bool esIdentificadorValido(const std::string& token) {
    if (token.empty()) return false;

    // No permitir palabras reservadas
    if (keywords.find(token) != keywords.end()) return false;

    // No permitir nombres comunes de la stdlib (criterio del usuario)
    if (std_library_words.find(token) != std_library_words.end()) return false;

    // Debe iniciar con letra o '_'
    if (!esLetra(token[0])) return false;

    // Resto: letras, dígitos o '_'
    for (size_t i = 1; i < token.size(); ++i) {
        if (!esCaracterValidoIdentificador(token[i])) return false;
    }

    // Límite razonable de longitud
    if (token.size() > MAX_ID_LEN) return false;

    return true;
}
