# Trivia TUI App - Elite Edition 🏆

## Descripción

Juego de Trivia en consola desarrollado en C++ (Estándar C++98) que ofrece una experiencia moderna de TUI (Text User Interface) con navegación interactiva, efectos de sonido y mecánicas estratégicas.

Compatible con Borland C++ (BCC 10.2) y compiladores modernos (GCC/MinGW). El proyecto implementa estructuras de datos manuales (`LinkedList`, `MyString`) sin utilizar la STL ni librerías externas.

## Características de Élite

- **Navegación Interactiva**: Selección de opciones mediante las flechas del teclado (`↑` `↓`).
- **Sistema de Comodines**:
  - **50/50**: Elimina dos opciones incorrectas.
  - **Doble o Nada**: Arriesga tus puntos para duplicar la ganancia o perderlo todo.
- **Temporizador de Presión**: Límite de 15 segundos por pregunta.
- **Perfiles de Jugador**: Guardado persistente de estadísticas (juegos, puntos, aciertos).
- **Audio & Visuales**: Sonidos reactivos (`Beep`) y colores dinámicos según el estado del juego.

## Estructura del Proyecto

- `main.cpp`: Punto de entrada (Configura UTF-8).
- `game.h`: Lógica de juego avanzada (Comodines, Timers, Turnos).
- `files.h`: Manejo de archivos (Preguntas y Perfiles binarios).
- `models.h`: Estructuras de datos del juego.
- `structures.h`: Implementación manual de C-Strings y Listas Enlazadas.
- `utils.h`: Motor de TUI (Navegación, Audio, Colores).

## Instrucciones de Compilación

### Opción 1: GCC / MinGW (Recomendado)

```bash
g++ main.cpp -o trivia_elite.exe
```

### Opción 2: Borland C++ (BCC 10.2)

```cmd
bcc32 main.cpp
```

## Ejecución

```cmd
./trivia_elite.exe
```

## Reglas del Juego

1. **Selección**: Navega por las categorías con las flechas.
2. **Comodines**: Antes de cada pregunta, puedes elegir usar un comodín (1 uso por partida).
3. **Tiempo**: Responde antes de que el contador llegue a 0.
4. **Progreso**: Tus puntos se multiplican según la ronda actual.
5. **Persistencia**: Tus estadísticas se actualizan automáticamente al final de la partida.
