/*-----------------------------------------------------
                     Trabalho Final
    Disciplina: Técnicas de Programação
    Data: 22/01/2023
    Aluna1: Patrícia Crescêncio Martins | RA: 0040732
    Aluna2: Letícia Moreira Leonel | RA: 0040287
-----------------------------------------------------*/
#include "header.h"
// construtor default
header::header() : serializable() {
   type = "***";
   firstDeleted = 0;
   firstValid = 0;
   version = 0;
   raiz = 0;
}
// construtor parametrizado
header::header(const string t, unsigned int v) : serializable() {
   // ctor
   type = t.substr(0, 3); //trunco para que pegue apenas os 3 primeiros caracteres
   firstDeleted = 0;
   firstValid = 0;
   version = v;
   raiz = 0;
}
// construtor de cópia
header::header(const header &h) {
   type = h.getType();
   firstDeleted = h.getFirstDeleted();
   firstValid = h.getFirstValid();
   version = h.getVersion();
   raiz = h.getRaiz();
}
// destrutor
header::~header() {

}
// sobrecarga do operadores
header header::operator=(const header &h) {
   header aux(h);
   if (this == &h)
      return *this;

   type = h.getType();
   firstDeleted = h.getFirstDeleted();
   firstValid = h.getFirstValid();
   version = h.getVersion();
   raiz = h.getRaiz();

   return aux;
}

bool header::operator==(const header &h) {
   return type == h.getType() && version == h.getVersion() &&
          firstDeleted == h.getFirstDeleted() &&
          firstValid == h.getFirstValid() && raiz == h.getRaiz();
}
// retorna o primeiro deletado
unsigned long long int header::getFirstDeleted() const {
   return firstDeleted;
}
// define o primeiro deletado
void header::setFirstDeleted(unsigned long long int r) {
   firstDeleted = r;
}
// retorna o primeiro válido
unsigned long long int header::getFirstValid() const {
   return firstValid;
}
// define o primeiro válido
void header::setFirstValid(unsigned long long int r) {
   firstValid = r;
}
//retorna a raiz
unsigned long long int header::getRaiz() const {
   return raiz;
}
//define a raiz
void header::setRaiz(unsigned long long int r) {
   raiz = r;
}
// retorna o tipo do cabeçalho
string header::getType() const {
   return type;
}
// define o tipo do cabeçalho
void header::setType(string t) {
   type = t.substr(0, 3);
}
// retorna a versão
unsigned int header::getVersion() const {
   return version;
}
// define a versão
void header::setVersion(unsigned int v) {
   version = v;
}
//serializa o cabeçalho
string header::toString() {
   string aux = ""; //zero o conteúdo da string para evitar lixo
   aux += type; // concateno o conteúdo de aux com o type
   aux += string(reinterpret_cast<char*>(&version), sizeof(version));
   aux += string(reinterpret_cast<char*>(&firstValid), sizeof(firstValid));
   aux += string(reinterpret_cast<char*>(&firstDeleted), sizeof(firstDeleted));
   aux += string(reinterpret_cast<char*>(&raiz), sizeof(raiz)); // crio um objeto anônimo do tipo string
   return aux;
}
//desserializa o cabeçalho
void header::fromString(string repr) {
   int pos = 0;
   type = repr.substr(pos, 3); //vou pegar a partir de 0, 3 bytes
   pos += type.length(); //avanço a posição
   repr.copy(reinterpret_cast<char*>(&version), sizeof(version), pos); //copia da string para o atributo version
   pos += sizeof(version); //avanço a posição
   repr.copy(reinterpret_cast<char*>(&firstValid), sizeof(firstValid), pos); //copia da string repr para o atributo firstValid
   pos += sizeof(firstValid); //avanço a posição
   repr.copy(reinterpret_cast<char*>(&firstDeleted), sizeof(firstDeleted), pos); //copia da string repr para o atributo firstDeleted
   pos += sizeof(raiz); //avanço a posição
   repr.copy(reinterpret_cast<char*>(&raiz), sizeof(raiz), pos); //copia da string repr para o atributo raiz
}
// retorna o tamanho do cabeçalho
unsigned long long int header::size() const {
   return type.length() + sizeof(version) + sizeof(firstValid) + sizeof(firstDeleted) + sizeof(raiz); //retorna 23 bytes
}
