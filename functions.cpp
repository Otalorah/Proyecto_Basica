#include <iostream>
#include <locale>
#include <stdlib.h>
#include <random>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

vector<int> num_oculto();
void intento();
void comprobacion();
vector<int> numAVector (int num);

int main(){
    setlocale(LC_ALL, "spanish");
    
    wcout << L"Número oculto: " << endl;

    vector<int>num = num_oculto();
    for (int i : num) {
        wcout << i << L" ";  // Imprimir cada dígito
    }

    system("pause");
    return 0;
}

//Funcion que comprobará el numero ingresado con el numero generado aleatoriamente.
void comprobacion(){
}

//Funcion que permite el ingreso de un numero.
void intento(){
}

//Funcion que llena con ceros el numero ingresado en caso de no estar completo
void ceros(){

}

//Funcion que convierte los numeros a vectores
vector<int> numAVector (int num){
    vector<int> digitos;
    while (num > 0){
        digitos.push_back(num % 10);
        num /= 10;
    } 
    reverse(digitos.begin(), digitos.end());
    return digitos;
}

vector<int> num_oculto(){
    vector<int> num;

    unsigned seed = static_cast<unsigned>(time(0));
    mt19937 generador(seed);

    uniform_int_distribution<int>distribucion{0, 9};

    for (int i = 0; i < 5; ++i){
        int digito = distribucion(generador);
        num.push_back(digito);
    }
    return num;
}