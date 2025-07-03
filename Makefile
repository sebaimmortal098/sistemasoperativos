# Makefile para Proyecto Sistemas Operativos - MTPA
# Mejorando el Performance de Manejo de Archivos

CXX = g++
CXXFLAGS = -std=c++11 -pthread -O2 -Wall -Wextra
TARGET = proyecto_so
TARGET_WIN = proyecto_so.exe
SOURCE = main_simple.cpp
SOURCE_OPENSSL = main.cpp

# Detectar sistema operativo
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
    TARGET_EXEC = $(TARGET_WIN)
    CLEAN_CMD = del /Q *.exe *.o *.log 2>nul
else
    DETECTED_OS := $(shell uname -s)
    TARGET_EXEC = $(TARGET)
    CLEAN_CMD = rm -f $(TARGET) *.o *.log
endif

.PHONY: all clean run test help

# Objetivo principal
all: $(TARGET_EXEC)

# Compilar version simple (recomendada)
$(TARGET_EXEC): $(SOURCE)
	@echo "================================================"
	@echo "  COMPILANDO PROYECTO SISTEMAS OPERATIVOS"
	@echo "  Versión: Compatible con Dev C++"
	@echo "  Sistema: $(DETECTED_OS)"
	@echo "================================================"
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET_EXEC)
	@echo "✓ Compilación exitosa!"
	@echo "  Ejecutable creado: $(TARGET_EXEC)"

# Compilar version con OpenSSL
openssl: $(SOURCE_OPENSSL)
	@echo "================================================"
	@echo "  COMPILANDO VERSIÓN CON OPENSSL"
	@echo "================================================"
	$(CXX) $(CXXFLAGS) $(SOURCE_OPENSSL) -o $(TARGET_EXEC) -lssl -lcrypto
	@echo "✓ Compilación con OpenSSL exitosa!"

# Ejecutar el programa
run: $(TARGET_EXEC)
	@echo "================================================"
	@echo "  EJECUTANDO PROGRAMA"
	@echo "================================================"
	./$(TARGET_EXEC)

# Ejecutar prueba con 10 copias
test: $(TARGET_EXEC)
	@echo "================================================"
	@echo "  EJECUTANDO PRUEBA AUTOMÁTICA (N=10)"
	@echo "================================================"
	@echo "10" | ./$(TARGET_EXEC)

# Limpiar archivos generados
clean:
	@echo "Limpiando archivos temporales..."
	$(CLEAN_CMD)
	@echo "✓ Limpieza completada"

# Debug version
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET_EXEC)
	@echo "✓ Versión debug compilada"

# Mostrar ayuda
help:
	@echo "================================================"
	@echo "  MAKEFILE - PROYECTO SISTEMAS OPERATIVOS"
	@echo "================================================"
	@echo "Objetivos disponibles:"
	@echo "  all      - Compilar el programa (predeterminado)"
	@echo "  openssl  - Compilar versión con OpenSSL"
	@echo "  run      - Compilar y ejecutar el programa"
	@echo "  test     - Ejecutar prueba automática con N=10"
	@echo "  debug    - Compilar versión debug"
	@echo "  clean    - Limpiar archivos temporales"
	@echo "  help     - Mostrar esta ayuda"
	@echo ""
	@echo "Ejemplos de uso:"
	@echo "  make           - Compilar"
	@echo "  make run       - Compilar y ejecutar"
	@echo "  make test      - Ejecutar prueba"
	@echo "  make clean     - Limpiar"

# Verificar dependencias
check:
	@echo "Verificando dependencias..."
	@echo -n "C++ Compiler: "
	@which $(CXX) > /dev/null && echo "✓ Encontrado" || echo "✗ No encontrado"
	@echo -n "Archivo fuente: "
	@test -f $(SOURCE) && echo "✓ Encontrado" || echo "✗ No encontrado"
	@echo -n "Archivo original: "
	@test -f original.txt && echo "✓ Encontrado" || echo "✗ No encontrado" 