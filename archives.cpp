#include <iostream>
#include <fstream>
#include <string>
#include "functions.h"
#include "main.h"

using namespace std;

void saveScores(const string& playerName, int points, const vector<int>& hideNumResult);

int main() {
    // Convertir el nombre de usuario de char a string
    string playerName(player.username);

    // Se obtiene el número oculto desde 'functions.h'
    vector<int> hideNumResult = hideNum(); 
    
    // Se obtiene el puntaje total desde 'main.h'
    int points = player.points;

    // Guardar el puntaje en el archivo
    saveScores(playerName, points, hideNumResult);

    return 0;
}

// Función para guardar los puntajes en un archivo
void saveScores(const string& playerName, int points, const vector<int>& hideNumResult) {
    ofstream file("scores.txt", ios::app);  // Abrir el archivo en modo anexar
    if (file.is_open()) {
        file << "Jugador: " << playerName << "\n";
        file << "Número oculto adivinado: ";
        for (const auto& digit : hideNumResult) {  
            file << digit;  
        }
        file << "\nPuntos Totales: " << points << "\n";
        file << "--------------------------\n";
        file.close();
        cout << "PUNTAJE GUARDADO CORRECTAMENTE." << endl;
    } else {
        cout << "ERROR: No se pudo abrir el archivo para escritura." << endl;
    }
}
