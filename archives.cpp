#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

using namespace std;

struct Puntaje {
    string nombre;    // Nombre del jugador
    int num_oculto;   // Número adivinado
    int intentos;     // Número de intentos

    // Métodos para obtener los valores de las variables miembro
    int obtenerNumOculto() const {
        return num_oculto;
    }

    string obtenerNombre() const {
        return nombre;
    }

    int obtenerIntentos() const {
        return intentos;
    }
};

void guardarPuntajes(const Puntaje& p) {

    setlocale(LC_ALL, "spanish");

    // Abrir el archivo en modo anexado
    ofstream archivo("puntajes.txt", ios::app);
    if (archivo.is_open()) {
        // Escribir los datos en el archivo
        archivo << p.obtenerNombre() << " - NÚMERO ADIVINADO: " << p.obtenerNumOculto() 
                << " - NUMERO DE INTENTOS: " << p.obtenerIntentos() << endl;
        archivo.close();
        cout << "DATOS GUARDADOS CORRECTAMENTE" << endl;
    } else {
        cout << "ERROR:--Los datos no se guardaron correctamente--" << endl;
    }
}

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

using namespace std;

struct Puntaje {
    string nombre;    // Nombre del jugador
    int num_oculto;   // Número adivinado
    int intentos;     // Número de intentos

    // Métodos para obtener los valores de las variables miembro
    int obtenerNumOculto() const {
        return num_oculto;
    }

    string obtenerNombre() const {
        return nombre;
    }

    int obtenerIntentos() const {
        return intentos;
    }
};

void guardarPuntajes(const Puntaje& p) {

    setlocale(LC_ALL, "spanish");

    // Abrir el archivo en modo anexado
    ofstream archivo("puntajes.txt", ios::app);
    if (archivo.is_open()) {
        // Escribir los datos en el archivo
        archivo << p.obtenerNombre() << " - NÚMERO ADIVINADO: " << p.obtenerNumOculto() 
                << " - NUMERO DE INTENTOS UTILIZADOS: " << p.obtenerIntentos() << endl;
        archivo.close();
        cout << "DATOS GUARDADOS CORRECTAMENTE" << endl;
    } else {
        cout << "ERROR:--Los datos no se guardaron correctamente--" << endl;
    }


