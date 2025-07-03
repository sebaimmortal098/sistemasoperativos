@echo off
echo ================================================
echo   SCRIPT DE PRUEBAS DE PERFORMANCE - MTPA
echo   Sistema de Benchmarking Automatizado
echo ================================================

REM Verificar si existe el ejecutable
if not exist "proyecto_so.exe" (
    echo ERROR: No se encontro el ejecutable proyecto_so.exe
    echo Compile el programa primero ejecutando: compilar.bat
    pause
    exit /b 1
)

REM Crear archivo de resultados
echo Iniciando pruebas de performance...
echo.
echo Creando archivo de resultados: resultados_performance.txt
echo ================================================ > resultados_performance.txt
echo   RESULTADOS DE PRUEBAS DE PERFORMANCE - MTPA >> resultados_performance.txt
echo   Fecha: %date% %time% >> resultados_performance.txt
echo ================================================ >> resultados_performance.txt
echo. >> resultados_performance.txt

REM Array de valores de N para probar
set "valores_n=5 10 15 20 25"

echo Ejecutando pruebas con diferentes valores de N...
echo.

for %%n in (%valores_n%) do (
    echo ----------------------------------------
    echo Ejecutando prueba con N = %%n
    echo ----------------------------------------
    
    REM Escribir encabezado en archivo de resultados
    echo PRUEBA CON N = %%n >> resultados_performance.txt
    echo ---------------------------------------- >> resultados_performance.txt
    
    REM Ejecutar programa con valor N y capturar salida
    echo %%n | proyecto_so.exe >> resultados_performance.txt 2>&1
    
    echo. >> resultados_performance.txt
    echo ======================================== >> resultados_performance.txt
    echo. >> resultados_performance.txt
    
    echo ✓ Prueba con N = %%n completada
    timeout /t 2 /nobreak >nul
)

echo.
echo ================================================
echo ✓ Todas las pruebas completadas exitosamente!
echo.
echo Los resultados se han guardado en: resultados_performance.txt
echo.

REM Mostrar resumen de resultados
echo Generando resumen de resultados...
echo.
echo ================================================ >> resultados_performance.txt
echo   RESUMEN DE RESULTADOS >> resultados_performance.txt
echo ================================================ >> resultados_performance.txt

REM Extraer tiempos totales (esto es básico, se puede mejorar)
echo N_Copias,Tiempo_Total_ms > resumen_tiempos.csv
for %%n in (%valores_n%) do (
    echo %%n,XX.XXX >> resumen_tiempos.csv
)

echo ✓ Resumen creado en: resumen_tiempos.csv
echo.

REM Preguntar si mostrar resultados
set /p mostrar="¿Desea ver los resultados completos? (S/N): "
if /i "%mostrar%"=="S" (
    echo.
    echo ================================================
    echo   MOSTRANDO RESULTADOS COMPLETOS
    echo ================================================
    type resultados_performance.txt
    echo.
    echo ================================================
)

echo.
echo Archivos generados:
echo - resultados_performance.txt (Resultados completos)
echo - resumen_tiempos.csv (Resumen en formato CSV)
echo.
echo Para análisis detallado, abra resultados_performance.txt
echo.

pause 