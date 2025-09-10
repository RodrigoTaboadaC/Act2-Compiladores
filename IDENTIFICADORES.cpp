#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <vector>
#include <set>

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

// Palabras comunes de la biblioteca estándar que no son keywords pero no son identificadores válidos
const std::set<std::string> std_library_words = {
    "std", "cout", "cin", "endl", "cerr", "clog", "string", "vector", "map",
    "set", "list", "array", "tuple", "pair", "endl", "printf", "scanf", "malloc",
    "free", "new", "delete", "sizeof", "typedef", "typename"
};

bool esLetra(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
}

bool esDigito(char c) {
    return std::isdigit(static_cast<unsigned char>(c));
}

bool esCaracterValidoIdentificador(char c) {
    return esLetra(c) || esDigito(c) || c == '_';
}

bool esIdentificadorValido(const std::string& token) {
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

std::vector<std::string> extraerTokens(const std::string& linea) {
    std::vector<std::string> tokens;
    std::string token;
    bool enToken = false;
    bool enComentario = false;
    bool enString = false;
    bool enChar = false;
    
    for (size_t i = 0; i < linea.size(); i++) {
        char c = linea[i];
        
        // Manejar comentarios
        if (!enString && !enChar && c == '/' && i + 1 < linea.size() && linea[i+1] == '/') {
            break; // Comentario de línea, ignorar el resto
        }
        
        // Manejar strings
        if (!enComentario && c == '"' && !enChar) {
            enString = !enString;
            continue;
        }
        
        // Manejar caracteres
        if (!enComentario && c == '\'' && !enString) {
            enChar = !enChar;
            continue;
        }
        
        // Si estamos dentro de string o char, ignorar
        if (enString || enChar) continue;
        
        // Si es carácter válido para identificador, agregar al token
        if (esCaracterValidoIdentificador(c)) {
            if (!enToken) {
                enToken = true;
                token.clear();
            }
            token += c;
        } else {
            // Fin del token actual
            if (enToken) {
                if (!token.empty()) {
                    tokens.push_back(token);
                }
                enToken = false;
                token.clear();
            }
        }
    }
    
    // Agregar el último token si existe
    if (enToken && !token.empty()) {
        tokens.push_back(token);
    }
    
    return tokens;
}

int main() {
    std::cout << "🔍 Analizador de Identificadores C++\n";
    std::cout << "====================================\n\n";
    
    std::ifstream archivo("archivo.txt");
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo 'archivo.txt'" << std::endl;
        std::cerr << "Por favor, crea un archivo con código C++ para analizar.\n";
        return 1;
    }

    std::string linea;
    int numeroLinea = 0;
    std::vector<std::string> identificadoresValidos;
    std::vector<std::pair<std::string, int>> identificadoresInvalidos;

    while (std::getline(archivo, linea)) {
        numeroLinea++;
        
        // Extraer tokens de la línea
        std::vector<std::string> tokens = extraerTokens(linea);
        
        for (const auto& token : tokens) {
            if (esIdentificadorValido(token)) {
                identificadoresValidos.push_back(token);
            } else if (esLetra(token[0]) || token[0] == '_') {
                // Solo reportar como inválido si comienza como identificador pero no es válido
                identificadoresInvalidos.push_back({token, numeroLinea});
            }
        }
    }

    archivo.close();

    // Mostrar resultados
    std::cout << "📊 RESULTADOS DEL ANÁLISIS:\n";
    std::cout << "============================\n\n";
    
    std::cout << "✅ IDENTIFICADORES VÁLIDOS (" << identificadoresValidos.size() << "):\n";
    for (const auto& id : identificadoresValidos) {
        std::cout << "   ✔ " << id << std::endl;
    }
    
    std::cout << "\n❌ IDENTIFICADORES INVÁLIDOS (" << identificadoresInvalidos.size() << "):\n";
    for (const auto& [id, linea] : identificadoresInvalidos) {
        std::cout << "   ✗ Línea " << linea << ": " << id << std::endl;
    }
    
    std::cout << "\n📈 ESTADÍSTICAS:\n";
    std::cout << "   Total de identificadores válidos: " << identificadoresValidos.size() << std::endl;
    std::cout << "   Total de identificadores inválidos: " << identificadoresInvalidos.size() << std::endl;
    std::cout << "   Porcentaje de validez: " 
              << (identificadoresValidos.size() * 100.0 / 
                 (identificadoresValidos.size() + identificadoresInvalidos.size() + 1))
              << "%\n";

    return 0;
}