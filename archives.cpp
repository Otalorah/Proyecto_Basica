#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <random>


using namespace std;


string name();
vector<int> hideNum();
vector<int>attempts();
void saveScores(const string& playerName, const vector<int>& hideNum, int totalAttempts);

int main() {
    // Nombre dle jugador
    string playerName = name();
    
    // Numero ocutlo
    vector<int> hideNumResult = hideNum();

    // Numero total de  intentos usados por el ususario
    int totalAttempts = 3;

    // Guardar el puntaje en el archivo
    saveScores(playerName, hideNumResult, totalAttempts);

    return 0;
}

// Función que genera un número oculto aleatorio de 5 dígitos únicos
vector<int> hideNum() {
    vector<int> num;
    unordered_set<int> numGen;

    unsigned seed = static_cast<unsigned>(time(0));
    mt19937 generator(seed);

    uniform_int_distribution<int> distribution{0, 9};

    while (num.size() < 5) {
        int digit = distribution(generator);

        if (numGen.find(digit) == numGen.end()) {
            num.push_back(digit);
            numGen.insert(digit);
        }
    }

    return num;
}

// Función para guardar los puntajes en un archivo
void saveScores(const string& playerName, const vector<int>& hideNum, int  totalAttempts) {
    ofstream file("scores.txt", ios::app);  // Abrir el archivo en modo anexar
    if (file.is_open()) {
        file << "Jugador: " << playerName << "\n";
        file << "Número oculto adivinado: ";
        for (const auto& digit : hideNum) {  
            file << digit;// Escribir cada dígito
        }
        file << "\nIntentos: " << totalAttempts << "\n";
        file << "--------------------------\n";
        file.close();
        cout << "PUNTAJE GUARDADO CORRECTAMENTE." << endl;
    } else {
        cout << "ERROR: Los datos no se guardaron correctamente." << endl;
    }
}

string name(){

    string playerName;
    cout<< "Ingrese nombre de judador: "<< endl;
    cin>>playerName;
    return playerName;
    
}






