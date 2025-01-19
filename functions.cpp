#include <iostream>
#include <locale>
#include <stdlib.h>
#include <random>

using namespace std;

int num_oculto();
void intento();
void comprobacion();

int main(){
    setlocale(LC_ALL, "spanish");

    num_oculto();
    wcout << L"Número oculto: " << num_oculto() << endl;

    system("pause");
    return 0;
}

void comprobacion(){
    //Funcion que comprobará el numero ingresado con el numero generado aleatoriamente.
}

void intento(){
    //Funcion que permite el ingreso de un numero.
}

int num_oculto(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 99999);

    int num = dis(gen);
    return num;
}