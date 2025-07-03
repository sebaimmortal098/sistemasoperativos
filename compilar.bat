@echo off
echo ===============================================
echo    COMPILADOR ULTRA-OPTIMIZADO - PROYECTO MTPA
echo    Version de Maxima Velocidad
echo ===============================================

REM Verificar si existe el archivo principal
if not exist "main_simple.cpp" (
    echo ERROR: No se encontro el archivo main_simple.cpp
    pause
    exit /b 1
)

REM Verificar si existe el archivo original
if not exist "original.txt" (
    echo ERROR: No se encontro el archivo original.txt
    pause
    exit /b 1
)

echo Compilando main_simple.cpp con optimizaciones maximas...
echo Flags de optimizacion: -O3 -march=native -DNDEBUG -ffast-math
echo.

REM Intentar compilacion con optimizaciones maximas
g++ -std=c++11 -pthread -O3 -march=native -DNDEBUG -ffast-math -mtune=native main_simple.cpp -o proyecto_so.exe 2>error.log

if %ERRORLEVEL% == 0 (
    echo ✓ COMPILACION ULTRA-OPTIMIZADA EXITOSA! 
    echo ✓ Ejecutable creado: proyecto_so.exe
    echo ✓ Optimizaciones aplicadas:
    echo   - Nivel O3 (optimizacion maxima)
    echo   - Instrucciones nativas del CPU
    echo   - Matematicas rapidas
    echo   - Codigo de debug removido
    echo   - Ajustes para CPU especifico
    echo.
    
    REM Limpiar archivo de errores
    if exist "error.log" del "error.log"
    
    echo Presiona cualquier tecla para ejecutar el programa...
    pause >nul
    echo.
    echo ===============================================
    echo      EJECUTANDO PROGRAMA ULTRA-OPTIMIZADO
    echo ===============================================
    echo.
    proyecto_so.exe
    
) else (
    echo.
    echo ✗ Error en la compilacion con optimizaciones maximas.
    echo ✗ Intentando compilacion basica...
    echo.
    
    REM Fallback a compilacion basica
    g++ -std=c++11 -pthread -O2 main_simple.cpp -o proyecto_so.exe 2>error.log
    
    if %ERRORLEVEL% == 0 (
        echo ✓ Compilacion basica exitosa!
        echo ✓ Ejecutable creado: proyecto_so.exe
        echo.
        
        if exist "error.log" del "error.log"
        
        echo Presiona cualquier tecla para ejecutar...
        pause >nul
        echo.
        echo ===============================================
        echo           EJECUTANDO PROGRAMA
        echo ===============================================
        echo.
        proyecto_so.exe
        
    ) else (
        echo.
        echo ✗ Error en compilacion basica.
        echo.
        if exist "error.log" (
            echo Detalles del error:
            type error.log
            echo.
        )
        echo.
        echo Posibles soluciones:
        echo 1. Instale MinGW o Dev C++
        echo 2. Verifique que g++ este en el PATH
        echo 3. Use Dev C++ para compilar manualmente
        echo 4. Comando manual: g++ -std=c++11 -pthread -O2 main_simple.cpp -o proyecto_so.exe
        echo.
    )
)

echo.
echo ===============================================
echo     COMPILACION ULTRA-OPTIMIZADA TERMINADA
echo ===============================================
pause 