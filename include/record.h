/*-----------------------------------------------------
                     Trabalho Final
    Disciplina: Técnicas de Programação
    Data: 22/01/2023
    Aluna1: Patrícia Crescêncio Martins | RA: 0040732
    Aluna2: Letícia Moreira Leonel | RA: 0040287
-----------------------------------------------------*/
#ifndef RECORD_H
#define RECORD_H
//registro
#include <string>
#include "serializable.h"

using namespace std;

template <class T> //talvez tenha que acrescentar aqui MIN_DEGREE
class record : public serializable {
    // asserção é um recurso global: garante que o tipo T deve ser filha de serializable,
    // pois tudo que tiver no registro precisa ser serializavel
    // só posso compilar o programa se o programador garantir que a classe serializable é base de T
   static_assert(is_base_of<serializable, T>::value, "T must be serializable"); //programação por contrato,
   // caso não atenda a exigência aparecerá a mensagem de erro que está entre aspas
   public:
      record(); //construtor default
      record(T d); //construtor parametrizado
      record(const record<T> &other); //construtor de cópia
      virtual ~record(); //destrutor
      record<T> operator=(const record<T> &other); //sobrecarga de operadores
      bool operator==(const record<T> &other);
      T getData() const; // retorna o objeto, no caso serialCurrency
      void setData(T d); // define o objeto, no caso serialCurrency
      unsigned long long int getNext() const; // retorna o próximo
      void setNext(unsigned long long int n); // define o próximo
      bool isDeleted() const; //retorna se foi deletado
      void del(); // como se fosse o set, quando quero marcar que foi deletado
      void undel(); //para reciclar, marco que não está deletado
      virtual string toString(); //serialização
      virtual void fromString(string repr); //desserialização
      virtual string toCSV() { return ""; }
      virtual void fromCSV(string repr) {}
      virtual string toJSON() { return ""; }
      virtual void fromJSON(string repr) {}
      virtual string toXML() { return ""; }
      virtual void fromXML(string repr) {}
      virtual unsigned long long int size() const; //retorna o tamanho do registro para serializar
   protected:
      T data; //dado que será o nó
      bool deleted; //flag que indica se o registro foi deletado ou não
      unsigned long long int next; //guarda o número do próximo registro - NÃO É PONTEIRO
};
//construtor default
template <class T>
record<T>::record() : serializable() {
    T dadoAux;
    this->data = dadoAux;
    this->deleted = false;
    this->next = 0;
}
//construtor parametrizado
template <class T>
record<T>::record(T d) : serializable() {
    this->data = d;
    this->deleted = false;
    this->next = 0;
}
//construtor de cópia
template <class T>
record<T>::record(const record<T> &other) {
    this->data = other.getData();
    this->deleted = other.isDeleted();
    this->next = other.getNext();
}
//destrutor
template <class T>
record<T>::~record() {

}
//sobrecarga de operadores
template <class T>
record<T> record<T>::operator=(const record<T> &other) {
    record<T> aux(other);
    this->data = other.getData();
    this->deleted = other.isDeleted();
    this->next = other.getNext();
    return aux;
}

template <class T>
bool record<T>::operator==(const record<T> &other) {
    return ((this->data == other.getData()) && (this->deleted == other.isDeleted()) && (this->next == other.getNext()));
}
// retorna o objeto serialCurrency no caso
template <class T>
T record<T>::getData() const {
    return this->data;
}
// define o objeto serialCurrency no caso
template <class T>
void record<T>::setData(T d) {
    this->data = d;
}
// retorna o próximo
template <class T>
unsigned long long int record<T>::getNext() const {
    return this->next;
}
// define o próximo
template <class T>
void record<T>::setNext(unsigned long long int n) {
    this->next = n;
}
//retorna se foi deletado
template <class T>
bool record<T>::isDeleted() const {
    return this->deleted;
}
// como se fosse o set, quando quero marcar que foi deletado
template <class T>
void record<T>::del() {
    this->deleted = true;
}
//para reciclar, marco que não está deletado
template <class T>
void record<T>::undel() {
    this->deleted = false;
}
//serialização
template <class T>
string record<T>::toString() {
    string repr = "";
    repr += data.toString();
    repr += string(reinterpret_cast<char*>(&deleted), sizeof(deleted));
    repr += string(reinterpret_cast<char*>(&next), sizeof(next));
    return repr;
}
//desserialização
template <class T>
void record<T>::fromString(string repr) {
    int pos = 0;
    string aux;
    aux = repr.substr(pos, data.size());
    data.fromString(aux);
    pos += data.size();
    repr.copy(reinterpret_cast<char*>(&deleted), sizeof(bool), pos);
    pos += sizeof(deleted);
    repr.copy(reinterpret_cast<char*>(&next), sizeof(next), pos);
}
//retorna o tamanho do registro
template <class T>
unsigned long long int record<T>::size() const {
    return ((this->data.size())+(sizeof(this->deleted))+(sizeof(this->next)));
}

#endif // RECORD_H
