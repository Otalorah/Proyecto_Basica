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
bool fullNum(string num);
bool checkNum(string num);
vector<int> attempt(int num);
wstring stringToWString(const string& str);
string vectorToString(const vector<int>& vec);
struct data checkUp(const vector<int>& num, const vector<int>& joinedNum);

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

// Función para convertir un string a wstring
wstring stringToWString(const string& str) {
    // Calcula el tamaño necesario para la cadena de caracteres anchos.
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    // Crea un wstring con el tamaño necesario.
    wstring wstrTo(size_needed, 0);
    // Convierte la cadena de entrada UTF-8 a una cadena de caracteres anchos.
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

// Función para convertir un string a UTF-8
string convertToUTF8(const char* str) {
    // Calcula el tamaño necesario para la cadena de caracteres anchos.
    int size_needed = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    if (size_needed <= 0) {
        return string();
    }
    // Crea un vector para contener la cadena de caracteres anchos.
    vector<wchar_t> wstrTo(size_needed);
    // Convierte la cadena de entrada ANSI a una cadena de caracteres anchos.
    MultiByteToWideChar(CP_ACP, 0, str, -1, &wstrTo[0], size_needed);

    // Calcula el tamaño necesario para la cadena UTF-8.
    size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstrTo[0], -1, NULL, 0, NULL, NULL);
    if (size_needed <= 0) {
        return string();
    }
    // Crea una cadena para contener la cadena codificada en UTF-8.
    string strTo(size_needed - 1, 0); // -1 para eliminar el carácter NUL
    // Convierte la cadena de caracteres anchos a una cadena codificada en UTF-8.
    WideCharToMultiByte(CP_UTF8, 0, &wstrTo[0], -1, &strTo[0], size_needed, NULL, NULL);
    return strTo;
}