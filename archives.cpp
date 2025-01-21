#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Función para guardar los puntajes en un archivo
void saveScores(vector<string>& playerNames, vector<vector<int>>& hideNums, vector<int>& attempts) {
    ofstream file("scores.txt", ios::app);  // Abrir el archivo en modo anexar
    if (file.is_open()) {
        for (size_t i = 0; i < playerNames.size(); ++i) {
            file << "Jugador: " << playerNames[i] << "\n";
            file << "Número oculto adivinado: ";
            for (const auto& digit : hideNums[i]) {
                file << digit;  // Escribir cada dígito 
            }
            file << "\nIntentos: " << attempts[i] << "\n";
            file << "--------------------------\n";
        }
        file.close();
        cout << "PUNTAJE GUARDADO CORRECTAMENTE." << endl;
    } else {
        cout << "ERROR: Los datos no se guardaron correctamente." << endl;
    }
}

int main() {
    vector<string> playerNames = {"Alice", "Bob"};
    vector<vector<int>> hideNums = {{1, 2, 3}, {4, 5, 6}};
    vector<int> attempts = {3, 5};

    saveScores(playerNames, hideNums, attempts);

    return 0;
}



