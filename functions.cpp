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
#include <iomanip>

using namespace std;

vector<int> hideNum();
vector<int> attempt(int num);
struct data checkUp(const vector<int>& num, const vector<int>& joinedNum);
bool fullNum(string num);
bool checkNum(string num);

struct data{
    int val1;
    int val2;
};

int main(){
    system("chcp 65001");
    
    string num = "12345";

    cout << checkNum(num) << endl;

    system("pause");

    return 0;

}

//Funcion que comprobará el numero ingresado con el numero generado aleatoriamente.
struct data checkUp(const vector<int>& num, const vector<int>& joinedNum){
    int fijas = 0, picas = 0;
    unordered_set<int> numSet(num.begin(), num.end());
    struct data results;

    for (size_t i = 0; i < num.size(); i++){
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
    
    results.val1 = fijas;
    results.val2 = picas;
    
    return results;

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

//Función que comprueba si el valor ingresado contiene letras (true si no contiene, false si si)
bool fullNum(string num){
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
bool checkNum(string num){
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