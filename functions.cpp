#include <iostream>
#include <locale>
#include <random>
#include <string>
#include <vector>
#include <ctime>
#include <unordered_set>
#include <utility>
#include <algorithm>
#include <stdlib.h>

using namespace std;

vector<int> hideNum();
vector<int> attempt(int num);
pair<int, int> checkUp(const vector<int>& num, const vector<int>& joinedNum);

int main(){
    setlocale(LC_ALL, "spanish");
    
    wcout << L"Número oculto: " << endl;

    vector<int>num = hideNum();
    for (auto i : num) {
        wcout << i << L" ";  // Imprimir cada dígito
    }

    vector<int> tryVec = attempt(45678);

    pair<int, int> result = checkUp(num, tryVec);


    wcout << "\nFijas: " << result.first << endl;
    wcout <<  "Picas: " << result.second << endl;

    system("pause");

    return 0;

}

//Funcion que comprobará el numero ingresado con el numero generado aleatoriamente.
pair<int, int> checkUp(const vector<int>& num, const vector<int>& joinedNum){
    int fijas = 0, picas = 0;
    unordered_set<int> numSet(num.begin(), num.end());

    for (int i = 0; i < num.size(); i++){
        if (num[i] == joinedNum[i]){
            fijas++;
        }
    }

    for (size_t i = 0; i < num.size(); ++i){
        if (num[i] != joinedNum[i]){
            if (numSet.find(joinedNum[i]) != numSet.end()){
                picas++;
            }
        }
    }
    
    return make_pair(fijas, picas);

}

//Funcion que permite el ingreso de un numero y lo convierte a un vector.
vector<int> attempt(int num){
    vector<int> joinedNum;

    while (num > 0){
        joinedNum.push_back(num % 10);
        num /= 10;
    }

    reverse(joinedNum.begin(), joinedNum.end());

    return joinedNum;
}

//Función que crea un número aleatorio de 5 cifras sin repetir dígitos
vector<int> hideNum(){
    vector<int> num;
    unordered_set<int> numGen;

    unsigned seed = static_cast<unsigned>(time(0));
    mt19937 generator(seed);

    uniform_int_distribution<int>distribution{0, 9};

    while (num.size() < 5){
        int digit = distribution(generator);
        
        if (numGen.find(digit) == numGen.end()){
            num.push_back(digit);
            numGen.insert(digit);
        }
    }

    return num;
}