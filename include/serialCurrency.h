/*-----------------------------------------------------
                     Trabalho Final
    Disciplina: Técnicas de Programação
    Data: 22/01/2023
    Aluna1: Patrícia Crescêncio Martins | RA: 0040732
    Aluna2: Letícia Moreira Leonel | RA: 0040287
-----------------------------------------------------*/
#ifndef SERIALCURRENCY_H
#define SERIALCURRENCY_H
#include "serializable.h"
#include <iostream>

class serialCurrency : serializable {
   friend ostream &operator << (ostream &output, const serialCurrency &sc); // classe amiga
   friend istream &operator >> (istream &input, serialCurrency &sc); // classe amiga
   public:
      serialCurrency(); //construtor default
      serialCurrency(float f); //construtor parametrizado para os valores de ponto fixo
      serialCurrency(const serialCurrency &other); //construtor de cópia
      virtual ~serialCurrency(); //destrutor
      serialCurrency operator=(const serialCurrency &other); //sobrecarga de operadores
      bool operator==(const serialCurrency &other) const;
      bool operator<(const serialCurrency &other) const;
      bool operator<=(const serialCurrency &other) const;
      bool operator>(const serialCurrency &other) const;
      bool operator>=(const serialCurrency &other) const;
      bool operator!=(const serialCurrency &other) const;
      serialCurrency operator+(const serialCurrency &other) const;
      serialCurrency operator-(const serialCurrency &other) const;
      serialCurrency operator*(const serialCurrency &other) const;
      serialCurrency operator/(const serialCurrency &other) const;
      int getX() const;
      int getY() const;
      float auxCalc(serialCurrency other, float &f1) const; //auxiliar para conversão
      string converter() const; //converter a string para mostrar no print
      string converter2() const; //para realizar cálculos e converter para float
      virtual string toString(); //serialização
      virtual void fromString(string repr); // desserialização
      virtual string toXML();
      virtual void fromXML(string repr);
      virtual string toCSV();
      virtual void fromCSV(string repr);
      virtual string toJSON();
      virtual void fromJSON(string repr);
      virtual unsigned long long int size() const;
   protected:
      void setX(int x);
      void setY(int y);
      float value;
      int x;
      int y;
};

#endif // SERIALCURRENCY_H
