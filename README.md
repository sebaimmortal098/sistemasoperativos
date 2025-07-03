# Proyecto Sistemas Operativos - MTPA
## Mejorando el Performance de Manejo de Archivos

### Descripción del Proyecto

Este proyecto implementa un sistema optimizado de manejo de archivos en C++ que incluye:
- Generación de N copias de un archivo original
- Encriptación usando algoritmo de desplazamiento ASCII
- Generación de hash para validación de integridad
- Validación y desencriptación de archivos
- Comparación con archivo original
- Medición precisa de tiempos de ejecución

### Algoritmo de Encriptación

El programa utiliza un algoritmo de encriptación simple como se especifica en el enunciado:
- **Letras**: Se desplazan 3 posiciones hacia la derecha en el alfabeto (A→D, B→E, etc.)
- **Números**: Se intercambian por su simétrico (0→9, 1→8, 2→7, etc.)
- **Otros caracteres**: Se mantienen sin cambios

### Optimizaciones Implementadas

1. **Procesamiento Paralelo**: Uso de `std::async` y `std::future` para ejecutar tareas en paralelo
2. **Threads Seguros**: Implementación de mutex para operaciones thread-safe
3. **Medición de Alta Precisión**: Uso de `std::chrono::high_resolution_clock`
4. **Gestión Eficiente de Memoria**: Manejo optimizado de strings y archivos
5. **Operaciones Asíncronas**: Todas las operaciones de archivos se ejecutan de forma asíncrona

### Archivos Incluidos

- `main.cpp`: Versión con OpenSSL para hash SHA-256 real
- `main_simple.cpp`: Versión compatible con Dev C++ (recomendada)
- `original.txt`: Archivo de texto base para procesamiento
- `README.md`: Este archivo de instrucciones

### Compilación

#### Opción 1: Dev C++ (Recomendado)
1. Abrir Dev C++
2. Crear un nuevo proyecto de consola C++
3. Reemplazar el código con el contenido de `main_simple.cpp`
4. Compilar con C++11 o superior

**Configuración para Dev C++:**
- Ir a Tools → Compiler Options
- En "Compiler" agregar: `-std=c++11 -pthread`
- En "Linker" agregar: `-pthread`

#### Opción 2: Línea de Comandos (Windows)
```bash
g++ -std=c++11 -pthread -O2 main_simple.cpp -o proyecto_so.exe
```

#### Opción 3: Visual Studio Code
```bash
g++ -std=c++11 -pthread -O2 main_simple.cpp -o proyecto_so
```

### Ejecución

1. Asegúrese de que el archivo `original.txt` esté en el mismo directorio
2. Ejecute el programa compilado
3. Ingrese el número de copias a generar (máximo 50)
4. Observe los tiempos de ejecución reportados

### Formato de Salida

El programa muestra la información en el formato requerido:

```
=== PROCESO BASE ===
TI: [hora de inicio en ms]
Tiempo 01: [tiempo generación copias] ms
Tiempo 02: [tiempo encriptación y hash] ms  
Tiempo 03: [tiempo validación y desencriptación] ms
Tiempo 04: [tiempo comparación] ms
TFIN: [hora finalización en ms]
TPPA: [tiempo promedio por archivo] ms
TT: [tiempo total] ms

=== PROCESO OPTIMIZADO ===
DF: [diferencia de performance]
PM: [porcentaje de mejora]
```

### Archivos Temporales

Durante la ejecución se generan archivos temporales:
- `1.txt, 2.txt, ..., N.txt`: Copias del archivo original
- `1.sha, 2.sha, ..., N.sha`: Hashes de los archivos encriptados

**Nota**: Estos archivos se eliminan automáticamente al finalizar el proceso.

### Requisitos del Sistema

- **SO**: Windows 10/11
- **Compilador**: MinGW, Dev C++, Visual Studio
- **Estándar**: C++11 o superior
- **RAM**: Mínimo 4GB (recomendado 8GB)
- **Espacio en disco**: Suficiente para N copias del archivo original

### Medición de Performance

El programa mide automáticamente:
- Tiempo de cada fase del proceso
- Tiempo total de ejecución (TTT)
- Tiempo promedio por archivo (TPPA)
- Diferencia de performance (DF)
- Porcentaje de mejora (PM)

### Troubleshooting

**Error: "No se encontró el archivo original.txt"**
- Verifique que `original.txt` esté en el mismo directorio que el ejecutable

**Error de compilación con threads**
- Asegúrese de agregar `-pthread` en las opciones del compilador

**Rendimiento lento**
- Reduzca el número de copias para pruebas
- Verifique que tiene suficiente RAM disponible

### Estrategia de Optimización

1. **Paralelización**: Todas las operaciones se ejecutan en paralelo usando threads
2. **Carga Asíncrona**: Los archivos se procesan de forma asíncrona
3. **Gestión de Memoria**: Uso eficiente de strings y buffers
4. **Algoritmos Optimizados**: Encriptación y hash implementados de forma eficiente
5. **Sincronización Mínima**: Uso de mutex solo cuando es necesario

### Resultados Esperados

Con N=10 copias, el programa debe completar todo el proceso en menos de 1000ms en hardware moderno, demostrando una mejora significativa respecto a implementaciones secuenciales.

### Notas Técnicas

- El programa limpia automáticamente los archivos temporales
- Maneja errores de E/O de forma robusta  
- Implementa logging thread-safe para debugging
- Compatible con caracteres especiales y acentos del español

### Equipo de Desarrollo

- **Integrantes**: [Agregar nombres de los integrantes del equipo]
- **Curso**: Sistemas Operativos
- **Proyecto**: MTPA - Mejorando el Performance de Manejo de Archivos # sistemasoperativos
