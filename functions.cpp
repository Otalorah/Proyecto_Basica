#include <iostream>
#include <locale>
#include <stdlib.h>
#include <random>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <set>
#include <utility>

using namespace std;

vector<int> num_oculto();
vector<int> intento(int num);
void resultado();
pair<int, int> comprobacion(const vector<int>& num, const vector<int>& numIngresado);

int main(){
    setlocale(LC_ALL, "spanish");
    
    wcout << L"Número oculto: " << endl;

    vector<int>num = num_oculto();
    for (int i : num) {
        wcout << i << L" ";  // Imprimir cada dígito
    }

    vector<int> intentoVec = intento(45678);

    pair<int, int> resultado = comprobacion(num, intentoVec);


    wcout << "\nFijas: " << resultado.first << endl;
    wcout <<  "Picas: " << resultado.second << endl;
    system("pause");
    return 0;

}

//Funcion que arroja el resultado de la comparacion entre el numero ingresado y el numero generado aleatoriamente.
void resultado(){

}

//Funcion que comprobará el numero ingresado con el numero generado aleatoriamente.
pair<int, int> comprobacion(const vector<int>& num, const vector<int>& numIngresado){
    int fijas = 0, picas = 0;

    for (int i = 0; i < num.size(); i++){
        if (num[i] == numIngresado[i]){
            fijas++;
        }
    }

    for (int i = 0; i < num.size(); i++){
        for (int j = 0; j < numIngresado.size(); j++){
            if (num[i] == numIngresado[j]){
                picas++;
            }
        }
    }

    picas -= fijas;
    
    return make_pair(fijas, picas);

}

//Funcion que permite el ingreso de un numero y lo convierte a un vector.
vector<int> intento(int num){
    vector<int> numIngresado;

    while (num > 0){
        numIngresado.insert(numIngresado.begin(), num % 10);
        num /= 10;
    }

    return numIngresado;
}

//Función que crea un número aleatorio de 5 cifras sin repetir dígitos
vector<int> num_oculto(){
    vector<int> num;
    set<int> numGen;

    unsigned seed = static_cast<unsigned>(time(0));
    mt19937 generador(seed);

    uniform_int_distribution<int>distribucion{0, 9};

    while (num.size() < 5){
        int digito = distribucion(generador);
        
        if (numGen.find(digito) == numGen.end()){
            num.push_back(digito);
            numGen.insert(digito);
        }
    }

    return num;
}