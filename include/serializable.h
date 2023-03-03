/*-----------------------------------------------------
                     Trabalho Final
    Disciplina: Técnicas de Programação
    Data: 22/01/2023
    Aluna1: Patrícia Crescêncio Martins | RA: 0040732
    Aluna2: Letícia Moreira Leonel | RA: 0040287
-----------------------------------------------------*/
#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H
#include <string>
using namespace std;

class serializable {
   public:
      serializable() {} //construtor default
      virtual ~serializable() {} //destrutor
      virtual string toString() = 0; //serialização
      virtual void fromString(string repr) = 0; //desserialização
      virtual string toXML() = 0;
      virtual void fromXML(string repr) = 0;
      virtual string toCSV() = 0;
      virtual void fromCSV(string repr) = 0;
      virtual string toJSON() = 0;
      virtual void fromJSON(string repr) = 0;
      virtual unsigned long long int size() const = 0;
};

#endif // SERIALIZABLE_H
