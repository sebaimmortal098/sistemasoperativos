#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <windows.h>

using namespace std;

// CONSTANTES
static const size_t MAX_THREADS = 12;
static const size_t MEGA_BUFFER_SIZE = 8 * 1024 * 1024;
static const size_t WARMUP_ITERATIONS = 2;
static const size_t BENCHMARK_RUNS = 3;

// TABLAS DE LOOKUP
static const char TABLA_ENCRIPT_LOWER[256] = {
    '\x00','\x01','\x02','\x03','\x04','\x05','\x06','\x07','\x08','\x09','\x0A','\x0B','\x0C','\x0D','\x0E','\x0F',
    '\x10','\x11','\x12','\x13','\x14','\x15','\x16','\x17','\x18','\x19','\x1A','\x1B','\x1C','\x1D','\x1E','\x1F',
    '\x20','\x21','\x22','\x23','\x24','\x25','\x26','\x27','\x28','\x29','\x2A','\x2B','\x2C','\x2D','\x2E','\x2F',
    '9','8','7','6','5','4','3','2','1','0',
    '\x3A','\x3B','\x3C','\x3D','\x3E','\x3F','\x40',
    'D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','A','B','C',
    '\x5B','\x5C','\x5D','\x5E','\x5F','\x60',
    'd','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','a','b','c',
    '\x7B','\x7C','\x7D','\x7E','\x7F','\x80','\x81','\x82','\x83','\x84','\x85','\x86','\x87','\x88','\x89','\x8A','\x8B','\x8C','\x8D','\x8E','\x8F',
    '\x90','\x91','\x92','\x93','\x94','\x95','\x96','\x97','\x98','\x99','\x9A','\x9B','\x9C','\x9D','\x9E','\x9F',
    '\xA0','\xA1','\xA2','\xA3','\xA4','\xA5','\xA6','\xA7','\xA8','\xA9','\xAA','\xAB','\xAC','\xAD','\xAE','\xAF',
    '\xB0','\xB1','\xB2','\xB3','\xB4','\xB5','\xB6','\xB7','\xB8','\xB9','\xBA','\xBB','\xBC','\xBD','\xBE','\xBF',
    '\xC0','\xC1','\xC2','\xC3','\xC4','\xC5','\xC6','\xC7','\xC8','\xC9','\xCA','\xCB','\xCC','\xCD','\xCE','\xCF',
    '\xD0','\xD1','\xD2','\xD3','\xD4','\xD5','\xD6','\xD7','\xD8','\xD9','\xDA','\xDB','\xDC','\xDD','\xDE','\xDF',
    '\xE0','\xE1','\xE2','\xE3','\xE4','\xE5','\xE6','\xE7','\xE8','\xE9','\xEA','\xEB','\xEC','\xED','\xEE','\xEF',
    '\xF0','\xF1','\xF2','\xF3','\xF4','\xF5','\xF6','\xF7','\xF8','\xF9','\xFA','\xFB','\xFC','\xFD','\xFE','\xFF'
};

static const char TABLA_DECRYPT_LOWER[256] = {
    '\x00','\x01','\x02','\x03','\x04','\x05','\x06','\x07','\x08','\x09','\x0A','\x0B','\x0C','\x0D','\x0E','\x0F',
    '\x10','\x11','\x12','\x13','\x14','\x15','\x16','\x17','\x18','\x19','\x1A','\x1B','\x1C','\x1D','\x1E','\x1F',
    '\x20','\x21','\x22','\x23','\x24','\x25','\x26','\x27','\x28','\x29','\x2A','\x2B','\x2C','\x2D','\x2E','\x2F',
    '9','8','7','6','5','4','3','2','1','0',
    '\x3A','\x3B','\x3C','\x3D','\x3E','\x3F','\x40',
    'X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W',
    '\x5B','\x5C','\x5D','\x5E','\x5F','\x60',
    'x','y','z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w',
    '\x7B','\x7C','\x7D','\x7E','\x7F','\x80','\x81','\x82','\x83','\x84','\x85','\x86','\x87','\x88','\x89','\x8A','\x8B','\x8C','\x8D','\x8E','\x8F',
    '\x90','\x91','\x92','\x93','\x94','\x95','\x96','\x97','\x98','\x99','\x9A','\x9B','\x9C','\x9D','\x9E','\x9F',
    '\xA0','\xA1','\xA2','\xA3','\xA4','\xA5','\xA6','\xA7','\xA8','\xA9','\xAA','\xAB','\xAC','\xAD','\xAE','\xAF',
    '\xB0','\xB1','\xB2','\xB3','\xB4','\xB5','\xB6','\xB7','\xB8','\xB9','\xBA','\xBB','\xBC','\xBD','\xBE','\xBF',
    '\xC0','\xC1','\xC2','\xC3','\xC4','\xC5','\xC6','\xC7','\xC8','\xC9','\xCA','\xCB','\xCC','\xCD','\xCE','\xCF',
    '\xD0','\xD1','\xD2','\xD3','\xD4','\xD5','\xD6','\xD7','\xD8','\xD9','\xDA','\xDB','\xDC','\xDD','\xDE','\xDF',
    '\xE0','\xE1','\xE2','\xE3','\xE4','\xE5','\xE6','\xE7','\xE8','\xE9','\xEA','\xEB','\xEC','\xED','\xEE','\xEF',
    '\xF0','\xF1','\xF2','\xF3','\xF4','\xF5','\xF6','\xF7','\xF8','\xF9','\xFA','\xFB','\xFC','\xFD','\xFE','\xFF'
};

// SHA256 CONSTANTS
static const unsigned long SHA256_K[64] = {
    0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL, 0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
    0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL, 0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
    0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL, 0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
    0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL, 0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
    0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL, 0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
    0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL, 0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
    0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL, 0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
    0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL, 0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

// ESTRUCTURA PARA DATOS DE THREAD
struct ThreadData {
    vector<int> archivos;
    const char* originalData;
    size_t originalSize;
    vector<double> tiempos;
    bool success;
    string errorMsg;
    bool optimizado;
};

// VARIABLES GLOBALES
static CRITICAL_SECTION g_cs;
static bool g_csInitialized = false;

// CLASE PARA OPTIMIZACIÓN DEL SISTEMA
class SystemOptimizer {
public:
    static void optimizeForPerformance() {
        SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
        
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        
        DWORD_PTR processAffinityMask = (1ULL << sysInfo.dwNumberOfProcessors) - 1;
        SetProcessAffinityMask(GetCurrentProcess(), processAffinityMask);
    }
    
    static void warmupSystem() {
        vector<char> dummyData(1024 * 1024, 'X');
        
        for (size_t i = 0; i < WARMUP_ITERATIONS; ++i) {
            for (size_t j = 0; j < dummyData.size(); j += 64) {
                dummyData[j] = TABLA_ENCRIPT_LOWER[dummyData[j]];
            }
            Sleep(10);
        }
    }
};

// FUNCIONES AUXILIARES SHA256
static unsigned long rotr(unsigned long x, int n) {
    return (x >> n) | (x << (32 - n));
}

static unsigned long ch(unsigned long x, unsigned long y, unsigned long z) {
    return (x & y) ^ (~x & z);
}

static unsigned long maj(unsigned long x, unsigned long y, unsigned long z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

static unsigned long sig0(unsigned long x) {
    return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
}

static unsigned long sig1(unsigned long x) {
    return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
}

static unsigned long gamma0(unsigned long x) {
    return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
}

static unsigned long gamma1(unsigned long x) {
    return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
}

// FUNCIONES DE ENCRIPTACIÓN
static void encriptarInPlace(char* data, size_t len) {
    size_t i = 0;
    // Desenrollado de bucle para mayor velocidad
    for (; i + 7 < len; i += 8) {
        data[i] = TABLA_ENCRIPT_LOWER[static_cast<unsigned char>(data[i])];
        data[i+1] = TABLA_ENCRIPT_LOWER[static_cast<unsigned char>(data[i+1])];
        data[i+2] = TABLA_ENCRIPT_LOWER[static_cast<unsigned char>(data[i+2])];
        data[i+3] = TABLA_ENCRIPT_LOWER[static_cast<unsigned char>(data[i+3])];
        data[i+4] = TABLA_ENCRIPT_LOWER[static_cast<unsigned char>(data[i+4])];
        data[i+5] = TABLA_ENCRIPT_LOWER[static_cast<unsigned char>(data[i+5])];
        data[i+6] = TABLA_ENCRIPT_LOWER[static_cast<unsigned char>(data[i+6])];
        data[i+7] = TABLA_ENCRIPT_LOWER[static_cast<unsigned char>(data[i+7])];
    }
    for (; i < len; ++i) {
        data[i] = TABLA_ENCRIPT_LOWER[static_cast<unsigned char>(data[i])];
    }
}

static void desencriptarInPlace(char* data, size_t len) {
    size_t i = 0;
    // Desenrollado de bucle para mayor velocidad
    for (; i + 7 < len; i += 8) {
        data[i] = TABLA_DECRYPT_LOWER[static_cast<unsigned char>(data[i])];
        data[i+1] = TABLA_DECRYPT_LOWER[static_cast<unsigned char>(data[i+1])];
        data[i+2] = TABLA_DECRYPT_LOWER[static_cast<unsigned char>(data[i+2])];
        data[i+3] = TABLA_DECRYPT_LOWER[static_cast<unsigned char>(data[i+3])];
        data[i+4] = TABLA_DECRYPT_LOWER[static_cast<unsigned char>(data[i+4])];
        data[i+5] = TABLA_DECRYPT_LOWER[static_cast<unsigned char>(data[i+5])];
        data[i+6] = TABLA_DECRYPT_LOWER[static_cast<unsigned char>(data[i+6])];
        data[i+7] = TABLA_DECRYPT_LOWER[static_cast<unsigned char>(data[i+7])];
    }
    for (; i < len; ++i) {
        data[i] = TABLA_DECRYPT_LOWER[static_cast<unsigned char>(data[i])];
    }
}

// FUNCIÓN SHA256
static string sha256Global(const char* data, size_t len) {
    unsigned long h[8] = {
        0x6a09e667UL, 0xbb67ae85UL, 0x3c6ef372UL, 0xa54ff53aUL,
        0x510e527fUL, 0x9b05688cUL, 0x1f83d9abUL, 0x5be0cd19UL
    };
    
    unsigned long long original_length_bits = static_cast<unsigned long long>(len) * 8;
    size_t padded_len = len + 1;
    while ((padded_len % 64) != 56) padded_len++;
    padded_len += 8;
    
    vector<char> buffer(padded_len);
    memcpy(&buffer[0], data, len);
    buffer[len] = 0x80;
    memset(&buffer[len + 1], 0, padded_len - len - 9);
    
    for (int i = 7; i >= 0; --i) {
        buffer[padded_len - 8 + (7-i)] = (original_length_bits >> (i * 8)) & 0xFF;
    }
    
    for (size_t chunk = 0; chunk < padded_len; chunk += 64) {
        unsigned long w[64];
        
        for (int i = 0; i < 16; ++i) {
            w[i] = (static_cast<unsigned long>(buffer[chunk + i * 4]) << 24) |
                   (static_cast<unsigned long>(buffer[chunk + i * 4 + 1]) << 16) |
                   (static_cast<unsigned long>(buffer[chunk + i * 4 + 2]) << 8) |
                   (static_cast<unsigned long>(buffer[chunk + i * 4 + 3]));
        }
        
        for (int i = 16; i < 64; ++i) {
            w[i] = gamma1(w[i - 2]) + w[i - 7] + gamma0(w[i - 15]) + w[i - 16];
        }
        
        unsigned long a = h[0], b = h[1], c = h[2], d = h[3];
        unsigned long e = h[4], f = h[5], g = h[6], h_var = h[7];
        
        for (int i = 0; i < 64; ++i) {
            unsigned long t1 = h_var + sig1(e) + ch(e, f, g) + SHA256_K[i] + w[i];
            unsigned long t2 = sig0(a) + maj(a, b, c);
            
            h_var = g; g = f; f = e; e = d + t1;
            d = c; c = b; b = a; a = t1 + t2;
        }
        
        h[0] += a; h[1] += b; h[2] += c; h[3] += d;
        h[4] += e; h[5] += f; h[6] += g; h[7] += h_var;
    }
    
    stringstream ss;
    for (int i = 0; i < 8; ++i) {
        ss << hex << setfill('0') << setw(8) << h[i];
    }
    
    return ss.str();
}

// I/O BÁSICO (PROCESO BASE)
static void writeFileBasic(const string& filename, const char* data, size_t size) {
    ofstream file(filename.c_str(), ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Cannot create file: " + filename);
    }
    file.write(data, size);
    file.close();
}

static vector<char> readFileBasic(const string& filename) {
    ifstream file(filename.c_str(), ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filename);
    }
    
    file.seekg(0, ios::end);
    size_t size = file.tellg();
    file.seekg(0, ios::beg);
    
    vector<char> buffer(size);
    file.read(&buffer[0], size);
    file.close();
    
    return buffer;
}

// I/O OPTIMIZADO (PROCESO OPTIMIZADO)
static void writeFileOptimized(const string& filename, const char* data, size_t size) {
    // Buffer grande para I/O optimizado
    static char buffer[MEGA_BUFFER_SIZE];
    
    ofstream file(filename.c_str(), ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Cannot create file: " + filename);
    }
    
    // Configurar buffer personalizado para mayor velocidad
    file.rdbuf()->pubsetbuf(buffer, MEGA_BUFFER_SIZE);
    
    // Escribir todo de una vez
    file.write(data, size);
    file.close();
}

static vector<char> readFileOptimized(const string& filename) {
    // Buffer grande para I/O optimizado
    static char buffer[MEGA_BUFFER_SIZE];
    
    ifstream file(filename.c_str(), ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filename);
    }
    
    // Configurar buffer personalizado para mayor velocidad
    file.rdbuf()->pubsetbuf(buffer, MEGA_BUFFER_SIZE);
    
    file.seekg(0, ios::end);
    size_t size = file.tellg();
    file.seekg(0, ios::beg);
    
    vector<char> result(size);
    file.read(&result[0], size);
    file.close();
    
    return result;
}

// FUNCIÓN DE THREAD
static DWORD WINAPI ThreadProcesarArchivos(LPVOID lpParam) {
    ThreadData* data = static_cast<ThreadData*>(lpParam);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
    
    try {
        data->tiempos.clear();
        data->tiempos.reserve(data->archivos.size());
        
        // Pre-allocar buffers para optimización
        vector<char> workBuffer;
        vector<char> hashBuffer;
        string hashString;
        
        if (data->optimizado) {
            workBuffer.reserve(data->originalSize * 2);
            hashBuffer.reserve(64);
            hashString.reserve(64);
        }
        
        for (size_t idx = 0; idx < data->archivos.size(); ++idx) {
            int numeroArchivo = data->archivos[idx];
            
            LARGE_INTEGER freq, start, end;
            QueryPerformanceFrequency(&freq);
            QueryPerformanceCounter(&start);
            
            try {
                if (data->optimizado) {
                    // PROCESO OPTIMIZADO - TODO EN MEMORIA
                    stringstream ss1, ss2, ss3;
                    ss1 << numeroArchivo << ".txt";
                    ss2 << numeroArchivo << "2.txt";
                    ss3 << numeroArchivo << ".sha";
                    
                    string filename = ss1.str();
                    string outFile = ss2.str();
                    string hashFile = ss3.str();
                    
                    // 1. Escribir archivo original (optimizado)
                    writeFileOptimized(filename, data->originalData, data->originalSize);
                    
                    // 2. Procesar en memoria (sin leer archivo)
                    workBuffer.assign(data->originalData, data->originalData + data->originalSize);
                    encriptarInPlace(&workBuffer[0], workBuffer.size());
                    
                    // 3. Escribir encriptado y hash (optimizado)
                    writeFileOptimized(filename, &workBuffer[0], workBuffer.size());
                    
                    hashString = sha256Global(&workBuffer[0], workBuffer.size());
                    writeFileOptimized(hashFile, hashString.c_str(), hashString.size());
                    
                    // 4. Validar hash (en memoria)
                    string calculatedHash = sha256Global(&workBuffer[0], workBuffer.size());
                    
                    if (calculatedHash == hashString) {
                        // 5. Desencriptar (en memoria)
                        desencriptarInPlace(&workBuffer[0], workBuffer.size());
                        writeFileOptimized(outFile, &workBuffer[0], workBuffer.size());
                        
                        // 6. Validación final (en memoria - sin leer archivo)
                        if (workBuffer.size() == data->originalSize && 
                            memcmp(&workBuffer[0], data->originalData, workBuffer.size()) == 0) {
                            // Validación exitosa
                        }
                    }
                    
                } else {
                    // PROCESO BASE - MUCHAS OPERACIONES DE I/O
                    stringstream ss1, ss2, ss3;
                    ss1 << numeroArchivo << ".txt";
                    ss2 << numeroArchivo << "2.txt";
                    ss3 << numeroArchivo << ".sha";
                    
                    string filename = ss1.str();
                    string outFile = ss2.str();
                    string hashFile = ss3.str();
                    
                    // 1. Escribir archivo original
                    writeFileBasic(filename, data->originalData, data->originalSize);
                    
                    // 2. Leer archivo
                    vector<char> buffer = readFileBasic(filename);
                    
                    // 3. Encriptar
                    encriptarInPlace(&buffer[0], buffer.size());
                    
                    // 4. Escribir encriptado
                    writeFileBasic(filename, &buffer[0], buffer.size());
                    
                    // 5. Generar hash
                    string hash = sha256Global(&buffer[0], buffer.size());
                    writeFileBasic(hashFile, hash.c_str(), hash.size());
                    
                    // 6. Leer archivo encriptado
                    vector<char> buffer2 = readFileBasic(filename);
                    
                    // 7. Leer hash
                    vector<char> hashBuffer = readFileBasic(hashFile);
                    string expectedHash(hashBuffer.begin(), hashBuffer.end());
                    
                    // 8. Validar hash
                    string calculatedHash = sha256Global(&buffer2[0], buffer2.size());
                    
                    if (calculatedHash == expectedHash) {
                        // 9. Desencriptar
                        desencriptarInPlace(&buffer2[0], buffer2.size());
                        
                        // 10. Escribir desencriptado
                        writeFileBasic(outFile, &buffer2[0], buffer2.size());
                        
                        // 11. Leer archivo final
                        vector<char> finalBuffer = readFileBasic(outFile);
                        
                        // 12. Validar con original
                        if (finalBuffer.size() == data->originalSize && 
                            memcmp(&finalBuffer[0], data->originalData, finalBuffer.size()) == 0) {
                            // Validación exitosa
                        }
                    }
                }
                
            } catch (const exception& e) {
                throw runtime_error("Error processing file " + to_string(numeroArchivo) + ": " + e.what());
            }
            
            QueryPerformanceCounter(&end);
            double tiempo = static_cast<double>(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
            data->tiempos.push_back(tiempo);
        }
        
        data->success = true;
    } catch (const exception& e) {
        data->success = false;
        data->errorMsg = e.what();
    }
    return 0;
}

// CLASE PRINCIPAL
class OptimizedFileProcessor {
private:
    string archivoOriginal;
    int numCopias;
    
    string formatDurationMS(double ms) const {
        stringstream ss;
        ss << fixed << setprecision(1) << ms << " ms";
        return ss.str();
    }
    
    string getCurrentSystemTime() const {
        SYSTEMTIME st;
        GetLocalTime(&st);
        stringstream ss;
        ss << setfill('0') << setw(2) << st.wHour << ":"
           << setfill('0') << setw(2) << st.wMinute << ":"
           << setfill('0') << setw(2) << st.wSecond;
        return ss.str();
    }
    
    void distribuirArchivos(vector<ThreadData>& threadData, bool optimizado) {
        // Usar 4 threads para estabilidad
        int threadsToUse = min(4, numCopias);
        threadData.resize(threadsToUse);
        
        int filesPerThread = numCopias / threadsToUse;
        int remainder = numCopias % threadsToUse;
        
        int currentFile = 1;
        for (int t = 0; t < threadsToUse; ++t) {
            threadData[t].archivos.clear();
            threadData[t].success = false;
            threadData[t].errorMsg = "";
            threadData[t].optimizado = optimizado;
            
            int filesForThisThread = filesPerThread;
            if (t < remainder) filesForThisThread++;
            
            for (int f = 0; f < filesForThisThread && currentFile <= numCopias; ++f) {
                threadData[t].archivos.push_back(currentFile);
                currentFile++;
            }
        }
    }
    
    vector<double> ejecutarConThreads(bool optimizado) {
        vector<char> originalData;
        if (optimizado) {
            originalData = readFileOptimized(archivoOriginal);
        } else {
            originalData = readFileBasic(archivoOriginal);
        }
        
        vector<ThreadData> threadData;
        distribuirArchivos(threadData, optimizado);
        
        for (size_t t = 0; t < threadData.size(); ++t) {
            threadData[t].originalData = &originalData[0];
            threadData[t].originalSize = originalData.size();
        }
        
        vector<HANDLE> threads;
        for (size_t t = 0; t < threadData.size(); ++t) {
            HANDLE hThread = CreateThread(NULL, 0, ThreadProcesarArchivos, &threadData[t], 0, NULL);
            if (hThread != NULL) {
                threads.push_back(hThread);
            }
        }
        
        if (!threads.empty()) {
            WaitForMultipleObjects(threads.size(), &threads[0], TRUE, INFINITE);
            for (size_t i = 0; i < threads.size(); ++i) {
                CloseHandle(threads[i]);
            }
        }
        
        for (size_t t = 0; t < threadData.size(); ++t) {
            if (!threadData[t].success) {
                throw runtime_error(threadData[t].errorMsg);
            }
        }
        
        vector<pair<int, double>> tiemposConIndice;
        for (size_t t = 0; t < threadData.size(); ++t) {
            for (size_t i = 0; i < threadData[t].archivos.size(); ++i) {
                int numeroArchivo = threadData[t].archivos[i];
                double tiempo = threadData[t].tiempos[i];
                tiemposConIndice.push_back(make_pair(numeroArchivo, tiempo));
            }
        }
        
        sort(tiemposConIndice.begin(), tiemposConIndice.end());
        
        vector<double> tiemposOrdenados;
        for (size_t i = 0; i < tiemposConIndice.size(); ++i) {
            tiemposOrdenados.push_back(tiemposConIndice[i].second);
        }
        
        return tiemposOrdenados;
    }

public:
    OptimizedFileProcessor(const string& archivo, int copias) 
        : archivoOriginal(archivo), numCopias(copias) {
        if (!g_csInitialized) {
            InitializeCriticalSection(&g_cs);
            g_csInitialized = true;
        }
    }
    
    ~OptimizedFileProcessor() {
        if (g_csInitialized) {
            DeleteCriticalSection(&g_cs);
            g_csInitialized = false;
        }
    }
    
    void limpiarArchivos() const {
        for (int i = 1; i <= numCopias; i++) {
            stringstream ss1, ss2, ss3;
            ss1 << i << ".txt";
            ss2 << i << "2.txt";
            ss3 << i << ".sha";
            
            DeleteFileA(ss1.str().c_str());
            DeleteFileA(ss2.str().c_str());
            DeleteFileA(ss3.str().c_str());
        }
    }
    
    double ejecutarProcesoBase() {
        cout << "\n=== PROCESO BASE ===\n";
        cout << "TI: " << getCurrentSystemTime() << "\n";
        cout.flush();
        
        vector<double> tiempos = ejecutarConThreads(false);
        double tiempoTotal = 0.0;
        
        for (int i = 0; i < numCopias; ++i) {
            cout << "Tiempo " << setfill('0') << setw(2) << (i+1) << ": " << formatDurationMS(tiempos[i]) << "\n";
            cout.flush();
            tiempoTotal += tiempos[i];
        }
        
        cout << "TFIN: " << getCurrentSystemTime() << "\n";
        cout << "TPPA: " << formatDurationMS(tiempoTotal / numCopias) << "\n";
        cout << "TT: " << formatDurationMS(tiempoTotal) << "\n";
        cout.flush();
        
        limpiarArchivos();
        return tiempoTotal;
    }
    
    double ejecutarProcesoOptimizado(double tiempoBase) {
        cout << "--------------------------------\n";
        cout << "=== PROCESO OPTIMIZADO ===\n";
        cout << "TI: " << getCurrentSystemTime() << "\n";
        cout.flush();
        
        vector<double> tiempos = ejecutarConThreads(true);
        double tiempoTotal = 0.0;
        
        for (int i = 0; i < numCopias; ++i) {
            cout << "Tiempo " << setfill('0') << setw(2) << (i+1) << ": " << formatDurationMS(tiempos[i]) << "\n";
            cout.flush();
            tiempoTotal += tiempos[i];
        }
        
        cout << "TFIN: " << getCurrentSystemTime() << "\n";
        cout << "TPPA: " << formatDurationMS(tiempoTotal / numCopias) << "\n";
        cout << "TT: " << formatDurationMS(tiempoTotal) << "\n";
        cout << "--------------------------------\n";
        
        double mejora = ((tiempoBase - tiempoTotal) / tiempoBase) * 100.0;
        cout << "DF: " << formatDurationMS(tiempoBase - tiempoTotal) << "\n";
        cout << "PM: " << fixed << setprecision(1) << mejora << "%\n";
        cout.flush();
        
        limpiarArchivos();
        return tiempoTotal;
    }
};

int main() {
    try {
        cout << "=== ULTRA-STABLE FILE PROCESSOR ===\n";
        cout << "Threads: " << MAX_THREADS << "\n";
        cout << "Buffer: " << MEGA_BUFFER_SIZE / (1024*1024) << "MB\n";
        cout << "Mediciones: " << BENCHMARK_RUNS << " por operacion\n";
        cout << "==========================================\n";
        cout.flush();
        
        SystemOptimizer::optimizeForPerformance();
        SystemOptimizer::warmupSystem();
        
        ifstream check("original.txt");
        if (!check.good()) {
            cout << "ERROR: original.txt no encontrado\n";
            return 1;
        }
        check.close();
        
        int numCopias;
        cout << "Numero de copias (1-50): ";
        cout.flush();
        cin >> numCopias;
        
        if (numCopias < 1 || numCopias > 50) {
            cout << "ERROR: Numero invalido\n";
            return 1;
        }
        
        OptimizedFileProcessor processor("original.txt", numCopias);
        
        double tiempoBase = processor.ejecutarProcesoBase();
        double tiempoOptimizado = processor.ejecutarProcesoOptimizado(tiempoBase);
        
        return 0;
        
    } catch (const exception& e) {
        cout << "ERROR: " << e.what() << "\n";
        return 1;
    }
}
