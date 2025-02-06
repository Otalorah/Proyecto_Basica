# Juego de Adivinar el Número

Este es un juego en C++ en el que el jugador debe adivinar un número de 5 cifras diferentes en el menor número de intentos posible. Cada intento se evalúa proporcionando al jugador las pistas en forma de **PICAS** y **FIJAS**.

## Descripción

En este juego, el objetivo es adivinar un número de 5 cifras que contiene dígitos únicos. El jugador tiene un número limitado de intentos para adivinar el número generado aleatoriamente por la computadora. Después de cada intento, el programa muestra cuántas cifras están en la posición correcta (**FIJAS**) y cuántas cifras están en el número pero no en la posición correcta (**PICAS**).

## Reglas del juego

- El número generado por la computadora es de 5 cifras, todas diferentes entre sí.
- El jugador tiene un número máximo de intentos para adivinar el número.
- El jugador debe ingresar un número de 5 cifras en cada intento, donde todas las cifras son diferentes.
- El juego proporciona retroalimentación en términos de **PICAS** y **FIJAS**:
  - **FIJAS**: Cifras correctas en la posición correcta.
  - **PICAS**: Cifras correctas pero en una posición incorrecta.
- El juego termina cuando el jugador adivina el número o cuando agota sus intentos.
- Al final del juego, el número generado por la computadora se muestra y se le da la opción al jugador de iniciar una nueva partida.

## Características

- El número que debe adivinar el jugador tiene 5 cifras diferentes.
- El jugador puede ingresar un número de 5 cifras en cada intento.
- El programa proporciona pistas después de cada intento en forma de **PICAS** y **FIJAS**.
- Si el jugador adivina el número, gana el juego; si no, pierde después de agotar sus intentos.
- El número de intentos es limitado, y el jugador es advertido si el número ingresado no cumple con los requisitos.

## Ejemplo de flujo del juego

1. El computador genera un número aleatorio de 5 cifras diferentes, por ejemplo, `12345`.
2. El jugador tiene un número máximo de intentos, por ejemplo, 10 intentos.
3. El jugador ingresa un número como `54321` y el programa responde con:
   - **FIJAS: 1** (solo el `5` está en la posición correcta).
   - **PICAS: 3** (otros 3 dígitos están presentes pero en posiciones incorrectas).
4. El jugador continúa ingresando números y recibiendo pistas hasta que adivine el número o agote sus intentos.

## Instrucciones de ejecución

### Requisitos

- **Sistema operativo**: Windows, Linux, macOS.
- **Compilador de C++**: MinGW, g++, o MSVC.

### Compilar y ejecutar:

1. Abre una terminal o símbolo del sistema.
2. Navega al directorio donde se encuentra el archivo fuente main.cpp.
3. Compila el código usando el siguiente comando (asegúrate de tener MinGW o g++ instalado): g++ -mwindows -municode -o main main.cpp.
4. Ejecuta el archivo main.exe