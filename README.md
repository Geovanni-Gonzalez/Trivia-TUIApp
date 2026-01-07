# Trivia TUI App

## Descripción

Juego de Trivia en consola desarrollado en C++ (Estándar C++98) compatible con Borland C++ (BCC 10.2) y compiladores modernos (GCC/MinGW).
El proyecto implementa estructuras de datos lineales manuales (`LinkedList`, `MyString`) sin utilizar la STL ni librerías externas.

## Estructura del Proyecto

- `main.cpp`: Punto de entrada.
- `game.h`: Lógica principal del juego (Rondas, Turnos, Puntuación).
- `files.h`: Manejo de archivos (Carga de categorías).
- `models.h`: Definición de Entidades (Jugador, Pregunta, Categoría).
- `structures.h`: Estructuras de datos manuales (`MyString`, `LinkedList`).
- `utils.h`: Utilidades varias (Random, Limpiar pantalla).
- `*.txt`: Archivos de categorías de preguntas (ej. `General.txt`).

## Instrucciones de Compilación

### Opción 1: GCC / MinGW (Recomendado para pruebas rápidas)

Abrir una terminal en la carpeta `programa` y ejecutar:

```bash
g++ main.cpp -o trivia.exe
```

### Opción 2: Borland C++ (BCC 10.2)

Si utiliza el compilador clásico de Borland:

1. Abra la consola de comandos de BCC.
2. Navegue a la carpeta del proyecto.
3. Ejecute:

```cmd
bcc32 main.cpp
```

*Nota: El código no utiliza características modernas de C++11/14/17 para asegurar compatibilidad.*

## Ejecución

Una vez compilado, ejecute el programa:

```cmd
trivia.exe
```

O si está en PowerShell:

```powershell
./trivia.exe
```

## Reglas del Juego

1. Se cargarán automáticamente todos los archivos `.txt` en la carpeta como categorías.
2. Seleccione la cantidad de jugadores (1-6) y sus nombres.
3. Elija la cantidad de rondas.
4. En cada turno, elija una categoría y responda la pregunta.
5. Gana quien acumule más puntos al final. En caso de empate, habrá muerte súbita.
