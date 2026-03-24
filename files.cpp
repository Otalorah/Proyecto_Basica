#include <fstream>
#include <iostream>
#include "files.h"

using namespace std;

bool saveScores(const string& playerName, int points, const vector<int>& hideNumResult) {
    ofstream file("scores.txt", ios::app);
    if (file.is_open()) {
        file << "Jugador: " << playerName << "\n";
        file << "Número oculto adivinado: ";
        for (const auto& digit : hideNumResult) {
            file << digit;
        }
        file << "\nPuntos Totales: " << points << "\n";
        file << "--------------------------\n";
        file.close();
        return true;
    }
    return false;
}

string getScore() {
    ifstream file("scores.txt");
    string scores, line;
    if (file.is_open()) {
        while (getline(file, line)) {
            scores += line + "\n";
        }
        file.close();
    } else {
        scores = "ERROR";
    }
    return scores;
}