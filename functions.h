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
pair<int, int> checkUp(const vector<int>& num, const vector<int>& joinedNum);

//Funcion que comprobará el numero ingresado con el numero generado aleatoriamente.
pair<int, int> checkUp(const vector<int>& num, const vector<int>& joinedNum){
    int spades = 0, fixed = 0;
    unordered_set<int> numSet(num.begin(), num.end());

    for (int i = 0; i < num.size(); i++){
        if (num[i] == joinedNum[i]){
            spades++;
        }
    }

    for (size_t i = 0; i < num.size(); ++i){
        if (num[i] != joinedNum[i]){
            if (numSet.find(joinedNum[i]) != numSet.end()){
                fixed++;
            }
        }
    }
    
    return make_pair(spades, fixed);

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

// Función para convertir un vector a string
string vectorToString(const vector<int>& vec) {
    string num;
    num = to_string(vec[0]) + to_string(vec[1]) + to_string(vec[2]) + to_string(vec[3]) + to_string(vec[4]);

    return num;
}