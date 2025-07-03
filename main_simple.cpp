#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <thread>
#include <future>
#include <mutex>
#include <memory>
#include <locale>
#include <codecvt>
#include <cstdint>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

using namespace std;
using namespace std::chrono;

// Optimización: Variables globales para evitar reasignaciones
static const size_t MAX_THREADS = std::thread::hardware_concurrency();
static const size_t BUFFER_SIZE = 65536; // 64KB buffer

// Tablas de lookup precalculadas fuera de la clase para C++11
static const char TABLA_ENCRIPT_LOWER[26] = {
    'd','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','a','b','c'
};
static const char TABLA_ENCRIPT_UPPER[26] = {
    'D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C'
};
static const char TABLA_ENCRIPT_DIGITS[10] = {'9','8','7','6','5','4','3','2','1','0'};

static const char TABLA_DECRYPT_LOWER[26] = {
    'x','y','z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w'
};
static const char TABLA_DECRYPT_UPPER[26] = {
    'X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W'
};
static const char TABLA_DECRYPT_DIGITS[10] = {'9','8','7','6','5','4','3','2','1','0'};

// Constantes SHA256 (primeras 64 raíces cúbicas de los primeros 64 números primos)
static const uint32_t SHA256_K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

class FileProcessor {
private:
    string archivoOriginal;
    int numCopias;
    mutable mutex logMutex;
    
    // Función de encriptación ULTRA-OPTIMIZADA con tablas de lookup
    inline string encriptar(const string& texto) noexcept {
        string resultado;
        resultado.reserve(texto.length());
        
        const char* data = texto.data();
        const size_t size = texto.size();
        
        for (size_t i = 0; i < size; ++i) {
            const char c = data[i];
            if (c >= 'a' && c <= 'z') {
                resultado += TABLA_ENCRIPT_LOWER[c - 'a'];
            } else if (c >= 'A' && c <= 'Z') {
                resultado += TABLA_ENCRIPT_UPPER[c - 'A'];
            } else if (c >= '0' && c <= '9') {
                resultado += TABLA_ENCRIPT_DIGITS[c - '0'];
            } else {
                resultado += c;
            }
        }
        return resultado;
    }
    
    // Función de desencriptación ULTRA-OPTIMIZADA con tablas de lookup
    inline string desencriptar(const string& texto) noexcept {
        string resultado;
        resultado.reserve(texto.length());
        
        const char* data = texto.data();
        const size_t size = texto.size();
        
        for (size_t i = 0; i < size; ++i) {
            const char c = data[i];
            if (c >= 'a' && c <= 'z') {
                resultado += TABLA_DECRYPT_LOWER[c - 'a'];
            } else if (c >= 'A' && c <= 'Z') {
                resultado += TABLA_DECRYPT_UPPER[c - 'A'];
            } else if (c >= '0' && c <= '9') {
                resultado += TABLA_DECRYPT_DIGITS[c - '0'];
            } else {
                resultado += c;
            }
        }
        return resultado;
    }
    
    // Implementación completa de SHA256 (sin librerías externas)
    inline string generarHashSimple(const string& texto) {
        return sha256(texto);
    }

private:
    
    // Funciones auxiliares SHA256
    static inline uint32_t rotr(uint32_t x, int n) {
        return (x >> n) | (x << (32 - n));
    }
    
    static inline uint32_t ch(uint32_t x, uint32_t y, uint32_t z) {
        return (x & y) ^ (~x & z);
    }
    
    static inline uint32_t maj(uint32_t x, uint32_t y, uint32_t z) {
        return (x & y) ^ (x & z) ^ (y & z);
    }
    
    static inline uint32_t sig0(uint32_t x) {
        return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
    }
    
    static inline uint32_t sig1(uint32_t x) {
        return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
    }
    
    static inline uint32_t gamma0(uint32_t x) {
        return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
    }
    
    static inline uint32_t gamma1(uint32_t x) {
        return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
    }
    
    // Implementación completa de SHA256
    string sha256(const string& texto) {
        // Valores iniciales de hash
        uint32_t h[8] = {
            0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
            0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
        };
        
        // Preparar mensaje
        vector<uint8_t> message(texto.begin(), texto.end());
        uint64_t original_length = message.size();
        
        // Padding
        message.push_back(0x80);
        while ((message.size() % 64) != 56) {
            message.push_back(0x00);
        }
        
        // Agregar longitud original en bits (big-endian)
        uint64_t bit_length = original_length * 8;
        for (int i = 7; i >= 0; --i) {
            message.push_back((bit_length >> (i * 8)) & 0xFF);
        }
        
        // Procesar mensaje en chunks de 512 bits
        for (size_t chunk = 0; chunk < message.size(); chunk += 64) {
            uint32_t w[64];
            
            // Copiar chunk a w[0..15]
            for (int i = 0; i < 16; ++i) {
                w[i] = (message[chunk + i * 4] << 24) |
                       (message[chunk + i * 4 + 1] << 16) |
                       (message[chunk + i * 4 + 2] << 8) |
                       (message[chunk + i * 4 + 3]);
            }
            
            // Extender w[16..63]
            for (int i = 16; i < 64; ++i) {
                w[i] = gamma1(w[i - 2]) + w[i - 7] + gamma0(w[i - 15]) + w[i - 16];
            }
            
            // Inicializar variables de trabajo
            uint32_t a = h[0], b = h[1], c = h[2], d = h[3];
            uint32_t e = h[4], f = h[5], g = h[6], h_var = h[7];
            
            // Compresión principal
            for (int i = 0; i < 64; ++i) {
                uint32_t t1 = h_var + sig1(e) + ch(e, f, g) + SHA256_K[i] + w[i];
                uint32_t t2 = sig0(a) + maj(a, b, c);
                
                h_var = g;
                g = f;
                f = e;
                e = d + t1;
                d = c;
                c = b;
                b = a;
                a = t1 + t2;
            }
            
            // Agregar hash comprimido al hash actual
            h[0] += a; h[1] += b; h[2] += c; h[3] += d;
            h[4] += e; h[5] += f; h[6] += g; h[7] += h_var;
        }
        
        // Convertir a string hexadecimal
        stringstream ss;
        for (int i = 0; i < 8; ++i) {
            ss << hex << setfill('0') << setw(8) << h[i];
        }
        
        return ss.str();
    }
    
    // Función de lectura ULTRA-OPTIMIZADA con buffer personalizado
    string leerArchivo(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo, ios::binary | ios::ate);
        if (!archivo.is_open()) {
            throw runtime_error("No se pudo abrir el archivo: " + nombreArchivo);
        }
        
        const auto tamaño = archivo.tellg();
        archivo.seekg(0, ios::beg);
        
        string contenido;
        contenido.resize(tamaño);
        
        // Lectura directa optimizada
        archivo.read(&contenido[0], tamaño);
        archivo.close();
        
        return contenido;
    }
    
    // Función de escritura ULTRA-OPTIMIZADA con buffer
    void escribirArchivo(const string& nombreArchivo, const string& contenido) {
        ofstream archivo(nombreArchivo, ios::binary);
        if (!archivo.is_open()) {
            throw runtime_error("No se pudo crear el archivo: " + nombreArchivo);
        }
        
        // Buffer optimizado para mejor rendimiento
        archivo.rdbuf()->pubsetbuf(nullptr, BUFFER_SIZE);
        archivo.write(contenido.data(), contenido.size());
        archivo.close();
    }
    
    // Función para configurar la consola para UTF-8 (Windows)
    void configurarConsola() {
        #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        _setmode(_fileno(stdout), _O_U8TEXT);
        _setmode(_fileno(stdin), _O_U8TEXT);
        #endif
    }
    
    // Función para log thread-safe con soporte UTF-8
    void log(const string& mensaje) const {
        lock_guard<mutex> lock(logMutex);
        #ifdef _WIN32
        // En Windows, usar wcout para mejor soporte de caracteres especiales
        wstring_convert<codecvt_utf8<wchar_t>> converter;
        wcout << converter.from_bytes(mensaje) << endl;
        #else
        cout << mensaje << endl;
        #endif
    }

public:
    FileProcessor(const string& archivo, int copias) 
        : archivoOriginal(archivo), numCopias(copias) {}
    
    // Función ULTRA-OPTIMIZADA para generar copias
    double generarCopias() {
        const auto inicio = high_resolution_clock::now();
        
        log("Iniciando generación de " + to_string(numCopias) + " copias...");
        
        const string contenidoOriginal = std::move(leerArchivo(archivoOriginal));
        
        // Optimización: Usar pool de threads controlado
        vector<future<void>> tareas;
        tareas.reserve(numCopias);
        
        const size_t threads_activos = min(static_cast<size_t>(numCopias), MAX_THREADS);
        
        for (int i = 1; i <= numCopias; i++) {
            tareas.emplace_back(async(launch::async, [this, i, &contenidoOriginal]() {
                const string nombreCopia = to_string(i) + ".txt";
                escribirArchivo(nombreCopia, contenidoOriginal);
                log("Copia generada: " + nombreCopia);
            }));
            
            // Control de threads: esperar si tenemos demasiados activos
            if (i % threads_activos == 0) {
                for (auto& tarea : tareas) {
                    if (tarea.valid()) tarea.wait();
                }
                tareas.clear();
                tareas.reserve(min(numCopias - i, static_cast<int>(threads_activos)));
            }
        }
        
        // Esperar tareas restantes
        for (auto& tarea : tareas) {
            if (tarea.valid()) tarea.wait();
        }
        
        const auto fin = high_resolution_clock::now();
        const auto duracion = duration_cast<microseconds>(fin - inicio);
        
        log("Copias generadas exitosamente.");
        return duracion.count() / 1000.0;
    }
    
    // Función para encriptar archivos y generar hash
    double encriptarYGenerarHash() {
        auto inicio = high_resolution_clock::now();
        
        log("Iniciando encriptación y generación de hash...");
        
        vector<future<void>> tareas;
        for (int i = 1; i <= numCopias; i++) {
            tareas.push_back(async(launch::async, [this, i]() {
                string nombreArchivo = to_string(i) + ".txt";
                string contenido = leerArchivo(nombreArchivo);
                
                // Encriptar contenido
                string contenidoEncriptado = encriptar(contenido);
                escribirArchivo(nombreArchivo, contenidoEncriptado);
                
                // Generar hash simple
                string hash = generarHashSimple(contenidoEncriptado);
                string nombreHash = to_string(i) + ".sha";
                escribirArchivo(nombreHash, hash);
                
                log("Procesado: " + nombreArchivo + " -> " + nombreHash);
            }));
        }
        
        // Esperar a que todas las tareas terminen
        for (auto& tarea : tareas) {
            tarea.wait();
        }
        
        auto fin = high_resolution_clock::now();
        auto duracion = duration_cast<microseconds>(fin - inicio);
        
        log("Encriptación y hash completados.");
        return duracion.count() / 1000.0; // Retornar en milisegundos
    }
    
    // Función para validar hash y desencriptar
    double validarYDesencriptar() {
        auto inicio = high_resolution_clock::now();
        
        log("Iniciando validación de hash y desencriptación...");
        
        vector<future<bool>> tareas;
        for (int i = 1; i <= numCopias; i++) {
            tareas.push_back(async(launch::async, [this, i]() {
                string nombreArchivo = to_string(i) + ".txt";
                string nombreHash = to_string(i) + ".sha";
                
                // Leer archivo encriptado y hash
                string contenidoEncriptado = leerArchivo(nombreArchivo);
                string hashEsperado = leerArchivo(nombreHash);
                
                // Validar hash
                string hashCalculado = generarHashSimple(contenidoEncriptado);
                bool hashValido = (hashCalculado == hashEsperado);
                
                if (hashValido) {
                    // Desencriptar contenido
                    string contenidoDesencriptado = desencriptar(contenidoEncriptado);
                    escribirArchivo(nombreArchivo, contenidoDesencriptado);
                    log("Validado y desencriptado: " + nombreArchivo);
                } else {
                    log("ERROR: Hash inválido para " + nombreArchivo);
                }
                
                return hashValido;
            }));
        }
        
        // Esperar a que todas las tareas terminen y contar errores
        int errores = 0;
        for (auto& tarea : tareas) {
            if (!tarea.get()) {
                errores++;
            }
        }
        
        auto fin = high_resolution_clock::now();
        auto duracion = duration_cast<microseconds>(fin - inicio);
        
        log("Validación completada. Errores encontrados: " + to_string(errores));
        return duracion.count() / 1000.0; // Retornar en milisegundos
    }
    
    // Función para comparar archivos con el original
    double compararConOriginal() {
        auto inicio = high_resolution_clock::now();
        
        log("Iniciando comparación con archivo original...");
        
        string contenidoOriginal = leerArchivo(archivoOriginal);
        
        vector<future<bool>> tareas;
        for (int i = 1; i <= numCopias; i++) {
            tareas.push_back(async(launch::async, [this, i, contenidoOriginal]() {
                string nombreArchivo = to_string(i) + ".txt";
                string contenidoArchivo = leerArchivo(nombreArchivo);
                
                bool esIgual = (contenidoArchivo == contenidoOriginal);
                if (esIgual) {
                    log("Archivo " + nombreArchivo + " coincide con el original");
                } else {
                    log("ERROR: Archivo " + nombreArchivo + " NO coincide con el original");
                }
                
                return esIgual;
            }));
        }
        
        // Esperar a que todas las tareas terminen y contar errores
        int errores = 0;
        for (auto& tarea : tareas) {
            if (!tarea.get()) {
                errores++;
            }
        }
        
        auto fin = high_resolution_clock::now();
        auto duracion = duration_cast<microseconds>(fin - inicio);
        
        log("Comparación completada. Errores encontrados: " + to_string(errores));
        return duracion.count() / 1000.0; // Retornar en milisegundos
    }
    
    // Función para limpiar archivos temporales
    void limpiarArchivos() {
        log("Limpiando archivos temporales...");
        
        for (int i = 1; i <= numCopias; i++) {
            string nombreTxt = to_string(i) + ".txt";
            string nombreSha = to_string(i) + ".sha";
            
            // Verificar si existen antes de eliminar
            ifstream checkTxt(nombreTxt);
            if (checkTxt.good()) {
                checkTxt.close();
                remove(nombreTxt.c_str());
            }
            
            ifstream checkSha(nombreSha);
            if (checkSha.good()) {
                checkSha.close();
                remove(nombreSha.c_str());
            }
        }
        
        log("Archivos temporales eliminados.");
    }
    
    // Función principal que ejecuta todo el proceso
    void ejecutarProceso() {
        auto tiempoInicio = high_resolution_clock::now();
        
        cout << "=== PROCESO BASE ===" << endl;
        cout << "TI: " << chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch()).count() << " ms" << endl;
        
        double tiempo1 = generarCopias();
        cout << "Tiempo 01: " << fixed << setprecision(3) << tiempo1 << " ms" << endl;
        
        double tiempo2 = encriptarYGenerarHash();
        cout << "Tiempo 02: " << fixed << setprecision(3) << tiempo2 << " ms" << endl;
        
        double tiempo3 = validarYDesencriptar();
        cout << "Tiempo 03: " << fixed << setprecision(3) << tiempo3 << " ms" << endl;
        
        double tiempo4 = compararConOriginal();
        cout << "Tiempo 04: " << fixed << setprecision(3) << tiempo4 << " ms" << endl;
        
        auto tiempoFin = high_resolution_clock::now();
        auto tiempoTotal = duration_cast<microseconds>(tiempoFin - tiempoInicio);
        
        cout << "TFIN: " << chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch()).count() << " ms" << endl;
        cout << "TPPA: " << fixed << setprecision(3) << tiempoTotal.count() / 1000.0 / numCopias << " ms" << endl;
        
        double tiempoTotalProceso = tiempo1 + tiempo2 + tiempo3 + tiempo4;
        cout << "TT: " << fixed << setprecision(3) << tiempoTotalProceso << " ms" << endl;
        
        cout << "\nNota: TI=hora de comienzo, TPPA=tiempo promedio por archivo, " 
             << "TFIN=hora finalización, TT=tiempo total" << endl;
        
        // Limpiar archivos temporales
        limpiarArchivos();
    }
};

int main() {
    try {
        // Configurar consola para UTF-8 y caracteres especiales
        #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        #endif
        
        cout << "=== SISTEMAS OPERATIVOS - PROYECTO MTPA ===" << endl;
        cout << "Mejorando el performance de manejo de archivos" << endl;
        cout << "Versión ULTRA-OPTIMIZADA con SHA256 REAL" << endl;
        cout << "Threads disponibles: " << MAX_THREADS << endl;
        cout << "=============================================" << endl;
        
        // Verificar si existe el archivo original
        ifstream checkFile("original.txt");
        if (!checkFile.good()) {
            cout << "Error: No se encontró el archivo original.txt" << endl;
            return 1;
        }
        checkFile.close();
        
        int numCopias;
        cout << "Ingrese el número de copias a generar (máximo 50): ";
        cin >> numCopias;
        
        if (numCopias < 1 || numCopias > 50) {
            cout << "Error: El número de copias debe estar entre 1 y 50" << endl;
            return 1;
        }
        
        // Crear procesador de archivos
        FileProcessor procesador("original.txt", numCopias);
        
        // Ejecutar el proceso
        procesador.ejecutarProceso();
        
        cout << "\n=== PROCESO ULTRA-OPTIMIZADO ===" << endl;
        cout << "El programa ha completado exitosamente el procesamiento" << endl;
        cout << "usando técnicas de optimización avanzadas:" << endl;
        cout << "- Procesamiento paralelo con pool de threads controlado" << endl;
        cout << "- Tablas de lookup precalculadas para encriptación" << endl;
        cout << "- Hash SHA256 REAL implementado desde cero" << endl;
        cout << "- I/O optimizado con buffers personalizados" << endl;
        cout << "- Move semantics y referencias optimizadas" << endl;
        cout << "- Gestión de memoria con reserve/resize" << endl;
        cout << "- Soporte completo para caracteres UTF-8" << endl;
        
        // Mostrar diferencia de rendimiento
        cout << "\nDF: Se muestra la diferencia en performance" << endl;
        cout << "PM: XX% (porcentaje de mejora obtenido)" << endl;
        cout << "\nNota: DF= diferencia entre tiempo final menos inicial" << endl;
        cout << "y PM = porcentaje de mejora" << endl;
        
        return 0;
        
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
} 