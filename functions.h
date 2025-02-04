#include <ctime>
#include <locale>
#include <random>
#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <stdlib.h>
#include <iostream>

#include <algorithm>
#include <unordered_set>

using namespace std;

vector<int> hideNum();
vector<int> attempt(int num);
string vectorToString(const vector<int>& vec);
struct data checkUp(const vector<int>& num, const vector<int>& joinedNum);
bool fullNum(string num);
bool checkNum(string num);

struct data {
    int val1;
    int val2;
};

//Funcion que comprobará el numero ingresado con el numero generado aleatoriamente.
struct data checkUp(const vector<int>& num, const vector<int>& joinedNum) {
    int fixeds = 0, spades = 0;
    unordered_set<int> numSet(num.begin(), num.end());
    struct data results;

    for (size_t i = 0; i < num.size(); i++){
        if (num[i] == joinedNum[i]){
            fixeds++;
        }
    }

    for (size_t i = 0; i < num.size(); ++i){
        if (num[i] != joinedNum[i]){
            if (numSet.find(joinedNum[i]) != numSet.end()){
                spades++;
            }
        }
    }
    
    results.val1 = fixeds;
    results.val2 = spades;
    
    return results;
}

//Funcion que permite el ingreso de un numero y lo convierte a un vector.
vector<int> attempt(int num) {
    vector<int> joinedNum;

    while (num > 0){
        joinedNum.push_back(num % 10);
        num /= 10;
    }

    reverse(joinedNum.begin(), joinedNum.end());

    return joinedNum;
}

//Función que crea un número aleatorio de 5 cifras sin repetir dígitos
vector<int> hideNum() {
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

// Función para convertir un vector a string
string vectorToString(const vector<int>& vec) {
    string num;
    num = to_string(vec[0]) + to_string(vec[1]) + to_string(vec[2]) + to_string(vec[3]) + to_string(vec[4]);

    return num;
}

//Función que comprueba si el valor ingresado contiene letras (true si no contiene, false si si)
bool fullNum(string num) {
    bool compro;

    for (int i = 0; i < num.size(); i++){
        if (isdigit(num[i])){
            compro = true;
        }
        else{
            return false;
        }
    }

    return compro;
}

//Función que comprueba si hay un número repetido en el valor ingresado (true si no hay valores repetidos, false si hay)
bool checkNum(string num) {
    bool check = true;

    bool seen[10] = {false};

    for (int i = 0; i < num.size(); i++){
        if (isdigit(num[i])){
            int digit = num[i] - '0';
            if (seen[digit]){
                check = false;
                break;
            }
            seen[digit] = true;
        }
    }
    return check;
}