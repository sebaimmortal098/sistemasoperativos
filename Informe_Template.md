# Informe del Proyecto - Sistemas Operativos
## Mejorando el Performance de Manejo de Archivos (MTPA)

---

### Datos del Equipo

**Integrantes del Equipo:**
- [Nombre Apellido 1 - Código/ID]
- [Nombre Apellido 2 - Código/ID] 
- [Nombre Apellido 3 - Código/ID]
- [Nombre Apellido 4 - Código/ID]

**Curso:** Sistemas Operativos  
**Profesor:** [Nombre del Profesor]  
**Fecha de Entrega:** [Fecha]  
**Universidad:** [Nombre de la Universidad]

---

### Resumen Ejecutivo

[Escribir un párrafo resumiendo el proyecto, objetivos y resultados principales]

---

### 1. Introducción

#### 1.1 Objetivo del Proyecto
El objetivo principal de este proyecto es optimizar el rendimiento en el manejo de archivos mediante técnicas de programación avanzada en C++, implementando operaciones de:
- Generación masiva de copias de archivos
- Encriptación y desencriptación de contenido
- Generación y validación de hashes
- Comparación de integridad de archivos

#### 1.2 Alcance
[Describir el alcance del proyecto y limitaciones si las hay]

---

### 2. Estrategia de Optimización Implementada

#### 2.1 Técnicas de Optimización Utilizadas

**1. Procesamiento Paralelo**
- Implementación de `std::async` y `std::future`
- Uso de threads para operaciones concurrentes
- [Explicar cómo mejora el rendimiento]

**2. Gestión Eficiente de Memoria**
- [Describir técnicas de gestión de memoria utilizadas]
- [Explicar beneficios obtenidos]

**3. Operaciones Asíncronas**
- [Describir implementación de operaciones asíncronas]
- [Beneficios en términos de tiempo]

**4. Algoritmos Optimizados**
- [Describir optimizaciones en algoritmos de encriptación]
- [Describir optimizaciones en generación de hash]

#### 2.2 Librerías Utilizadas

| Librería | Propósito | Justificación |
|----------|-----------|---------------|
| `<chrono>` | Medición de tiempo de alta precisión | [Justificar uso] |
| `<thread>` | Manejo de threads | [Justificar uso] |
| `<future>` | Operaciones asíncronas | [Justificar uso] |
| `<mutex>` | Sincronización thread-safe | [Justificar uso] |
| [Otras] | [Propósito] | [Justificación] |

---

### 3. Implementación Técnica

#### 3.1 Arquitectura del Sistema

```
[Incluir diagrama de la arquitectura del sistema]
```

#### 3.2 Algoritmo de Encriptación

**Especificación del Algoritmo:**
- **Letras:** Desplazamiento de 3 posiciones ASCII hacia la derecha
- **Números:** Intercambio simétrico (0↔9, 1↔8, 2↔7, etc.)
- **Otros caracteres:** Sin modificación

**Ejemplo:**
```
Texto original: "Hola123"
Texto encriptado: "Krod876"
```

#### 3.3 Proceso de Validación

[Describir el proceso de validación implementado]

---

### 4. Resultados y Análisis de Performance

#### 4.1 Mediciones con N = 10

**Configuración de Prueba:**
- **Sistema Operativo:** [Especificar]
- **Procesador:** [Especificar]
- **RAM:** [Especificar]
- **Número de copias:** 10

**Resultados Obtenidos:**

| Fase | Tiempo (ms) | Descripción |
|------|-------------|-------------|
| TI | [Tiempo] | Hora de inicio |
| Tiempo 01 | [Tiempo] | Generación de copias |
| Tiempo 02 | [Tiempo] | Encriptación y hash |
| Tiempo 03 | [Tiempo] | Validación y desencriptación |
| Tiempo 04 | [Tiempo] | Comparación con original |
| TFIN | [Tiempo] | Hora de finalización |
| **TT** | **[Tiempo Total]** | **Tiempo Total del Proceso** |
| TPPA | [Tiempo] | Tiempo promedio por archivo |

#### 4.2 Comparación de Performance

**Antes de la Optimización (Proceso Secuencial):**
- Tiempo Total Estimado: [Tiempo] ms

**Después de la Optimización (Proceso Paralelo):**
- Tiempo Total Medido: [Tiempo] ms

**Mejora Obtenida:**
- **DF (Diferencia):** [Tiempo diferencia] ms
- **PM (Porcentaje de Mejora):** [X]%

#### 4.3 Análisis de Escalabilidad

[Incluir tabla con diferentes valores de N y sus tiempos correspondientes]

| N (Copias) | Tiempo Total (ms) | Tiempo por Archivo (ms) |
|------------|-------------------|-------------------------|
| 5 | [Tiempo] | [Tiempo] |
| 10 | [Tiempo] | [Tiempo] |
| 20 | [Tiempo] | [Tiempo] |
| 50 | [Tiempo] | [Tiempo] |

---

### 5. Desafíos y Soluciones

#### 5.1 Desafíos Técnicos Encontrados

**1. [Desafío 1]**
- **Problema:** [Describir problema]
- **Solución:** [Describir solución implementada]
- **Resultado:** [Describir resultado obtenido]

**2. [Desafío 2]**
- **Problema:** [Describir problema]
- **Solución:** [Describir solución implementada]
- **Resultado:** [Describir resultado obtenido]

#### 5.2 Limitaciones Identificadas

[Describir limitaciones del sistema y posibles mejoras futuras]

---

### 6. Conclusiones

#### 6.1 Objetivos Cumplidos

- ✅ [Objetivo 1]
- ✅ [Objetivo 2]
- ✅ [Objetivo 3]
- ✅ [Objetivo 4]

#### 6.2 Lecciones Aprendidas

[Describir qué aprendieron durante el desarrollo del proyecto]

#### 6.3 Trabajo Futuro

[Describir posibles mejoras y extensiones del proyecto]

---

### 7. Referencias

1. [Referencia 1]
2. [Referencia 2]
3. [Referencia 3]

---

### Anexos

#### Anexo A: Código Fuente Completo

[Incluir código fuente o referencia a archivos adjuntos]

#### Anexo B: Casos de Prueba

[Incluir casos de prueba realizados]

#### Anexo C: Logs de Ejecución

[Incluir ejemplos de salida del programa]

---

**Nota:** Este informe debe acompañar al archivo comprimido con el código fuente y ser presentado en formato DOC/DOCX para cumplir con los requisitos del proyecto. 