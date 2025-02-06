#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

string getScore();
bool saveScores(const string& playerName, int points, const vector<int>& hideNumResult);

// Función para guardar los puntajes en un archivo
bool saveScores(const string& playerName, int points, const vector<int>& hideNumResult) {
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
      return true;
   } else {
      return false;
   }
}


// Función para obtener los puntajes guardados
string getScore() {
   ifstream file("scores.txt");  // Abrir el archivo en modo lectura
   string scores;
   string line;
   
   if (file.is_open()) {
      while (getline(file, line)) {
         scores += line + "\n";  // Guardar cada línea en el string
      }
      file.close();
   } else {
      scores = "ERROR";
   }

   return scores;  
}