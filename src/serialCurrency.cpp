/*-----------------------------------------------------
                     Trabalho Final
    Disciplina: Técnicas de Programação
    Data: 22/01/2023
    Aluna1: Patrícia Crescêncio Martins | RA: 0040732
    Aluna2: Letícia Moreira Leonel | RA: 0040287
-----------------------------------------------------*/
#include "serialCurrency.h"
#include <string>
#include <iostream>
#include <cmath>
//construtor default
serialCurrency::serialCurrency() : serializable() {
    this->x = 0;
    this->y = 0;
}
//construtor parametrizado para os valores de ponto fixo
serialCurrency::serialCurrency(float f) : serializable() {
    this->x = int(f); // pega apenas a parte inteira de f e coloca em x, considerando o sinal
    float aux;
    if (f < 0 && f > -1){ // caso f possua 0 negativo o negativo irá se perder ao passar para x, por isso vou manter o sinal de y
       aux = (f-this->x)*100;
    } else {
        aux = abs(f-this->x)*100; // cálculo para pegar os dois dígitos da parte decimal, ignorando o sinal
    }
    this->y = round(aux); // coloca em y a parte decimal arredondando
    if (this->y >= 100){ // caso y arredonde para 100 devo incrementar uma unidade em x
        this->y = 0;
        if (this->x >= 0){ // para x positivo
            this->x += 1;
        } else {
            this->x -= 1; // para x negativo
        }
    }
    if (this->y <= -100){ // testa o mesmo caso anterior porém para um número y negativo
        this->y = 0;
        this->x -= 1;
    }
}
//construtor de cópia
serialCurrency::serialCurrency(const serialCurrency &other) {
    this->setX(other.getX());
    this->setY(other.getY());
}
//destrutor
serialCurrency::~serialCurrency() {

}
//sobrecarga de operadores
serialCurrency serialCurrency::operator=(const serialCurrency &other) {
    serialCurrency aux(other);
    this->setX(other.getX());
    this->setY(other.getY());
    return aux;
}

bool serialCurrency::operator==(const serialCurrency &other) const {
    return (this->x == other.getX() && this->y == other.getY());
}

bool serialCurrency::operator<(const serialCurrency &other) const {
    if (this->x < other.getX()){
        return true;
    } else {
        if (this->x == other.getX()){
            return (this->y < other.getY());
        } else {
            return false;
        }
    }
}

bool serialCurrency::operator<=(const serialCurrency &other) const {
    if (this->x < other.getX()){
        return true;
    } else {
        if (this->x == other.getX()){
            return (this->y <= other.getY());
        } else {
            return false;
        }
    }
}

bool serialCurrency::operator>(const serialCurrency &other) const {
    if (this->x > other.getX()){
        return true;
    } else {
        if (this->x == other.getX()){
            return (this->y > other.getY());
        } else {
            return false;
        }
    }
}

bool serialCurrency::operator>=(const serialCurrency &other) const {
    if (this->x > other.getX()){
        return true;
    } else {
        if (this->x == other.getX()){
            return (this->y >= other.getY());
        } else {
            return false;
        }
    }
}

bool serialCurrency::operator!=(const serialCurrency &other) const {
    if (this->x != other.getX()){
        return true;
    } else {
        return (this->y != other.getY());
    }
}
//auxiliar para conversão
float serialCurrency::auxCalc(serialCurrency other, float &f1) const {
    string s1 = converter2();
    string s2 = other.converter2();
    f1 = stof(s1);
    float f2 = stof(s2);
    return f2;
}

serialCurrency serialCurrency::operator+(const serialCurrency& other) const {
    float f1;
    float f2 = auxCalc(other, f1);
    serialCurrency r(f1+f2);
    return r;
}

serialCurrency serialCurrency::operator-(const serialCurrency& other) const {
    float f1;
    float f2 = auxCalc(other, f1);
    serialCurrency r(f1-f2);
    return r;
}

serialCurrency serialCurrency::operator*(const serialCurrency& other) const {
    float f1;
    float f2 = auxCalc(other, f1);
    serialCurrency r(f1*f2);
    return r;
}

serialCurrency serialCurrency::operator/(const serialCurrency& other) const {
    float f1;
    float f2 = auxCalc(other, f1);
    serialCurrency r(f1/f2);
    return r;
}

ostream &operator << (ostream &output, const serialCurrency &sc){
    string aux = ".";
    serialCurrency s(sc);
    if (sc.getY() < 10 && sc.getY() > -10){
        aux = ".0";
    }
    if (sc.getY() < 0){
        s.setY(-sc.y) ; // transformo y em positivo
        output << "-" << s.x << aux << s.getY();
    } else {
        output << sc.getX() << aux << sc.getY();
    }
    return output;
}

istream &operator >> (istream &input, serialCurrency &sc){
    float f;
    input >> f;
    serialCurrency scAux(f);
    sc = scAux;
    return input;
}

void serialCurrency::setX(int x) {
    this->x = x;
}

int serialCurrency::getX() const {
    return this->x;
}

void serialCurrency::setY(int y) {
    this->y = y;
}

int serialCurrency::getY() const {
    return this->y;
}

string serialCurrency::converter2() const { // para conseguirmos realizar os cálculos (sum, sub, multi e div)
    string repr = "";                           // para ser possível converter para float
    int auxY = this->y;
    if (this->y < 0){
        int auxX = -(this->x);
        auxY = -(this->y);
        if (this->x == 0){
            repr += "-";
        }
        repr += to_string(auxX);
    } else {
        repr = to_string(this->x);
    }
    string auxP = ","; // aqui preciso de vírgula ao invés de ponto como é no print
    if (getY() < 10 && getY() > -10){
        auxP = ",0";
    }
    repr += auxP;
    repr += to_string(auxY);
    return repr;
}

string serialCurrency::converter() const { // para conseguirmos exibir no print
    string repr = "";
    int auxY = this->y;
    if (this->y < 0){
        int auxX = -(this->x);
        auxY = -(this->y);
        if (this->x == 0){
            repr += "-";
        }
        repr += to_string(auxX);
    } else {
        repr = to_string(this->x);
    }
    string auxP = ".";
    if (getY() < 10 && getY() > -10){
        auxP = ".0";
    }
    repr += auxP;
    repr += to_string(auxY);
    return repr;
}
// serialização
string serialCurrency::toString() {
    string repr = ""; //zera para evitar lixo
    repr += string(reinterpret_cast<char*>(&this->x), sizeof(int));
    repr += string(reinterpret_cast<char*>(&this->y), sizeof(int));
    return repr;
}
// desserialização
void serialCurrency::fromString(string repr) {
    int pos = 0;
    repr.copy(reinterpret_cast<char*>(&this->x), sizeof(int), pos);
    pos += sizeof(x); // avanço a posição
    repr.copy(reinterpret_cast<char*>(&this->y), sizeof(int), pos);
}

string serialCurrency::toXML() {
    return "";
}

void serialCurrency::fromXML(string repr) {

}

string serialCurrency::toCSV() {
    return "";
}

void serialCurrency::fromCSV(string repr) {

}

string serialCurrency::toJSON() {
    return "";
}

void serialCurrency::fromJSON(string repr) {

}

unsigned long long int serialCurrency::size() const {
    return sizeof(x)+sizeof(y);
}
