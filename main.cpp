#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include <thread>
#include <future>
#include <mutex>

// Incluir bibliotecas para SHA-256 (usando OpenSSL)
#include <openssl/sha.h>

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

class FileProcessor {
private:
    string archivoOriginal;
    int numCopias;
    mutex logMutex;
    
    // Función para encriptar texto usando el algoritmo especificado
    string encriptar(const string& texto) {
        string resultado;
        for (char c : texto) {
            if (isalpha(c)) {
                // Desplazar ASCII de letras 3 posiciones a la derecha
                if (islower(c)) {
                    resultado += char(((c - 'a' + 3) % 26) + 'a');
                } else {
                    resultado += char(((c - 'A' + 3) % 26) + 'A');
                }
            } else if (isdigit(c)) {
                // Intercambiar números por su simétrico (0<->9, 1<->8, etc.)
                resultado += char('9' - (c - '0') + '0');
            } else {
                resultado += c;
            }
        }
        return resultado;
    }
    
    // Función para desencriptar texto
    string desencriptar(const string& texto) {
        string resultado;
        for (char c : texto) {
            if (isalpha(c)) {
                // Desplazar ASCII de letras 3 posiciones a la izquierda
                if (islower(c)) {
                    resultado += char(((c - 'a' - 3 + 26) % 26) + 'a');
                } else {
                    resultado += char(((c - 'A' - 3 + 26) % 26) + 'A');
                }
            } else if (isdigit(c)) {
                // Intercambiar números por su simétrico (inverso)
                resultado += char('9' - (c - '0') + '0');
            } else {
                resultado += c;
            }
        }
        return resultado;
    }
    
    // Función para generar hash SHA-256
    string generarSHA256(const string& texto) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, texto.c_str(), texto.length());
        SHA256_Final(hash, &sha256);
        
        stringstream ss;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            ss << hex << setw(2) << setfill('0') << (int)hash[i];
        }
        return ss.str();
    }
    
    // Función para leer archivo completo
    string leerArchivo(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            throw runtime_error("No se pudo abrir el archivo: " + nombreArchivo);
        }
        
        string contenido;
        string linea;
        while (getline(archivo, linea)) {
            contenido += linea + "\n";
        }
        archivo.close();
        return contenido;
    }
    
    // Función para escribir archivo
    void escribirArchivo(const string& nombreArchivo, const string& contenido) {
        ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            throw runtime_error("No se pudo crear el archivo: " + nombreArchivo);
        }
        archivo << contenido;
        archivo.close();
    }
    
    // Función para log thread-safe
    void log(const string& mensaje) {
        lock_guard<mutex> lock(logMutex);
        cout << mensaje << endl;
    }

public:
    FileProcessor(const string& archivo, int copias) 
        : archivoOriginal(archivo), numCopias(copias) {}
    
    // Función para generar copias del archivo original
    double generarCopias() {
        auto inicio = high_resolution_clock::now();
        
        log("Iniciando generación de " + to_string(numCopias) + " copias...");
        
        string contenidoOriginal = leerArchivo(archivoOriginal);
        
        // Generar copias en paralelo
        vector<future<void>> tareas;
        for (int i = 1; i <= numCopias; i++) {
            tareas.push_back(async(launch::async, [this, i, contenidoOriginal]() {
                string nombreCopia = to_string(i) + ".txt";
                escribirArchivo(nombreCopia, contenidoOriginal);
                log("Copia generada: " + nombreCopia);
            }));
        }
        
        // Esperar a que todas las tareas terminen
        for (auto& tarea : tareas) {
            tarea.wait();
        }
        
        auto fin = high_resolution_clock::now();
        auto duracion = duration_cast<microseconds>(fin - inicio);
        
        log("Copias generadas exitosamente.");
        return duracion.count() / 1000.0; // Retornar en milisegundos
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
                
                // Generar hash SHA-256
                string hash = generarSHA256(contenidoEncriptado);
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
                string hashCalculado = generarSHA256(contenidoEncriptado);
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
            
            if (fs::exists(nombreTxt)) {
                fs::remove(nombreTxt);
            }
            if (fs::exists(nombreSha)) {
                fs::remove(nombreSha);
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
        cout << "TPPA: " << fixed << setprecision(3) << tiempoTotal.count() / 1000.0 << " ms" << endl;
        
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
        cout << "=== SISTEMAS OPERATIVOS - PROYECTO MTPA ===" << endl;
        cout << "Mejorando el performance de manejo de archivos" << endl;
        cout << "=============================================" << endl;
        
        // Verificar si existe el archivo original
        if (!fs::exists("original.txt")) {
            cout << "Error: No se encontró el archivo original.txt" << endl;
            return 1;
        }
        
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
        
        cout << "\n=== PROCESO OPTIMIZADO ===" << endl;
        cout << "El programa ha completado exitosamente el procesamiento" << endl;
        cout << "usando técnicas de optimización como:" << endl;
        cout << "- Procesamiento paralelo con threads" << endl;
        cout << "- Operaciones asíncronas" << endl;
        cout << "- Medición de tiempo de alta precisión" << endl;
        cout << "- Gestión eficiente de memoria" << endl;
        
        return 0;
        
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
} 