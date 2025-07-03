@echo off
echo ================================================
echo    SISTEMAS OPERATIVOS - PROYECTO MTPA
echo    Script de Compilacion para Windows
echo ================================================

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

echo Compilando el programa...
echo.

REM Compilar con diferentes opciones segun el compilador disponible
g++ -std=c++11 -pthread -O2 main_simple.cpp -o proyecto_so.exe 2>error.log

if %ERRORLEVEL% == 0 (
    echo ✓ Compilacion exitosa!
    echo.
    echo El ejecutable 'proyecto_so.exe' ha sido creado.
    echo.
    echo Para ejecutar el programa, escriba: proyecto_so.exe
    echo.
    
    REM Limpiar archivo de errores si no hay errores
    if exist "error.log" del "error.log"
    
    echo Presione cualquier tecla para ejecutar el programa...
    pause >nul
    
    echo.
    echo Ejecutando el programa...
    echo ================================================
    proyecto_so.exe
    
) else (
    echo.
    echo ✗ Error en la compilacion!
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
    echo.
)

pause 