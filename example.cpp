#include <string>
#include <fstream>
#include <iostream>
#include <windows.h>
#include "functions.h"

using namespace std;

void saveScores(const string& playerName, int points, const vector<int>& hideNumResult);
string getScore();

int main() {
   // Configuración en la consola
   SetConsoleCP(CP_UTF8);
   SetConsoleOutputCP(CP_UTF8);
   locale::global(locale(""));

   // Convertir el nombre de usuario de char a string
   string playerName = "PARGALAY";

   // Se obtiene el número oculto desde 'functions.h'
   vector<int> hideNumResult = hideNum(); 
   
   int points = 7777;

   // Guardar el puntaje en el archivo
   // saveScores(playerName, points, hideNumResult);

   // Mostrar los puntajes guardados
   string allScores = getScore();
   cout << "Puntajes guardados:\n" << allScores;

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
      scores = "ERROR: No se pudo abrir el archivo para lectura.";
   }

   return scores;  
}