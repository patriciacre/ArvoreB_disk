/*-----------------------------------------------------
                     Trabalho Final
    Disciplina: Técnicas de Programação
    Data: 22/01/2023
    Aluna1: Patrícia Crescêncio Martins | RA: 0040732
    Aluna2: Letícia Moreira Leonel | RA: 0040287
-----------------------------------------------------*/
#ifndef HEADER_H
#define HEADER_H
//cabeçalho
#include "serializable.h"

class header : public serializable {
   public:
      header(); // construtor default
      header(const string t, unsigned int v); // construtor parametrizado
      header(const header &h); // construtor de cópia
      virtual ~header(); // destrutor
      header operator=(const header &h); // sobrecarga do operador de atribuição
      bool operator==(const header &h); // sobrecarga do operador de igualdade
      unsigned long long int getFirstDeleted() const;// retorna o primeiro deletado
      void setFirstDeleted(unsigned long long int r); // define o primeiro deletado
      unsigned long long int getFirstValid() const; // retorna o primeiro válido
      void setFirstValid(unsigned long long int r); // define o primeiro válido
      string getType() const; // retorna o tipo do cabeçalho
      void setType(string t); // define o tipo do cabeçalho
      unsigned int getVersion() const; // retorna a versão
      void setVersion(unsigned int v); // define a versão
      unsigned long long int getRaiz () const; //retorna a raiz
      void setRaiz(unsigned long long int r); //define a raiz
      virtual string toString(); // serialização
      virtual void fromString(string repr); // desserialização
      virtual string toXML() { return ""; };
      virtual void fromXML(string repr) {};
      virtual string toCSV() { return ""; };
      virtual void fromCSV(string repr){};
      virtual string toJSON() { return ""; };
      virtual void fromJSON(string repr) {};
      virtual unsigned long long int size() const; // retorna o tamanho do cabeçalho
   protected:
      string type; // o tipo - 3bytes
      unsigned int version; // a versão - 4 bytes
      unsigned long long int firstValid; // o primeiro registro válido - este possui 8 bytes
      unsigned long long int firstDeleted; // o primeiro registro deletado - 8 bytes
      //possível guardar registro até 2⁶⁴ -1.
      unsigned long long int raiz; // 8 bytes
};

#endif // HEADER_H
