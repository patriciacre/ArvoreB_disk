/*-----------------------------------------------------
                     Trabalho Final
    Disciplina: Técnicas de Programação
    Data: 22/01/2023
    Aluna1: Patrícia Crescêncio Martins | RA: 0040732
    Aluna2: Letícia Moreira Leonel | RA: 0040287
-----------------------------------------------------*/
#ifndef TYPEDFILE_H
#define TYPEDFILE_H
//arquivo
#include <fstream>
#include <type_traits>
#include "record.h"
#include "header.h"

using namespace std;
    //fazer checagem de tipo
const ios::openmode mode = ios::in | ios::out | ios::binary;

template <class T> //é template pq não sei o que será guardado no arquivo
//herança privada pois todos os métodos de fstream só poderão ser acessados diretamente por meio dessa classe
class typedFile : private fstream { //é um tipo de arquivo, por isso herda de fstream
   static_assert(is_base_of<serializable, T>::value, "T must be serializable"); //obriga T a ser filha de serializable
   public:
      typedFile();                     //openmode recebe um valor default, não é obrigado que seja entregue algo para esse parâmetro
      typedFile(const string name, const string type, const unsigned int version, ios::openmode openmode = mode);
      virtual ~typedFile();
      bool open(const string name, const string type, const unsigned int version, ios::openmode openmode = mode);
      bool isOpen(); // basta chamar is_open na implementação
      bool close(); // basta chamar close de fstream na implementação
      void clear(); // basta chamar clear de fstream na apresentação
      bool readRecord(record<T> &r, unsigned long long int i); //para que retorne bool e o registro seja alterado, i é o n° do registro
      bool writeRecord(record<T> &r, unsigned long long int i); // mesma ideia, utilizado para reciclar
      bool insertRecord(record<T> &r); // cria um registro novo, insert chama a writeRecord para escrever no arquivo
      bool deleteRecord(unsigned long long int i); //exclui onde está o registro, move pra lista de excluídos e atualiza os apontadores
      unsigned long long int getFirstValid();
      unsigned long long int getFirstDeleted();
      unsigned long long int getRaiz();
      unsigned long long int search(T data); // percorre a lista de válidos e retorna a posição do dado
   protected:
      header head; // cabeçalho
      bool readHeader(header &h); // desserializar
      bool writeHeader(header &h); // serializar
      unsigned long long int index2pos(unsigned long long int i); //retorna o valor em bytes de onde será inserido, com base no n° do registro
      unsigned long long int pos2index(unsigned long long int p); //retorna qual será o n° do registro para writeRecord escrever
};

template <class T>
typedFile<T>::typedFile() : fstream()
{

}

template <class T>
typedFile<T>::typedFile(const string name, const string type, const unsigned int version, ios::openmode openmode) : fstream(name.c_str(), mode) {
    if (isOpen()){ // arquivo já existe
        this->readHeader(this->head); // próximas rodadas
        if (!(this->head.getType() == type && this->head.getVersion() == version)) {
            cout << "Arquivo existente não é correspondente." << endl;
            this->close();
        }
    } else { // vou criar o arquivo, pois o construtor da fstream não conseguiu abrir o arquivo e deu falha
        this->clear(); // devo limpar as flags
        if (this->open(name, type, version, ios::out)){ // primeira rodada
            if (this->close()){ // fecho
                this->open(name, type, version); // abro novamente agora no modo padrão
            }
        }
    }
}

//destrutor para fechar o arquivo
template <class T>
typedFile<T>::~typedFile() {
    this->close();
}


template <class T>
bool typedFile<T>::open(const string name, const string type, const unsigned int version, ios::openmode openmode) {
    fstream::open(name, openmode);
    if (openmode == mode && !fstream::fail()){
        header h(type, version);
        this->head = h;
        return this->writeHeader(this->head);
    } else {
        if(fstream::fail()){
            this->clear();
            return false;
        } else {
            return true;
        }
    }
}

//se o arquivo já existe
template <class T>
bool typedFile<T>::isOpen() {
    return (fstream::is_open());
}

template <class T>
bool typedFile<T>::close() { //só vou fechar se tiver aberto
    if (isOpen()){
        fstream::close();
        if(fstream::fail()){
            this->clear();
            return false;
        } else {
            return true;
        }
    } else {
        return false; // não consegui fechar pois não estava aberto
    }
}

template <class T>
void typedFile<T>::clear() { // verifico se está aberto pra poder limpar
    if (isOpen()){
        fstream::clear();
    }
}
// basicamente realiza a desserialização do registro
template <class T>                          // n° do registro
bool typedFile<T>::readRecord(record<T> &r, unsigned long long int i) {
    if (isOpen() && i != 0 && fstream::good()){ //verificar se está aberto e não há flag de erro ativa para poder ler
        this->seekp(0, ios::end);
        unsigned long long int aux = tellp();
        if (index2pos(i) > aux){ //verifico se o n° do registro corresponde a algum que já tenha sido gravado no arquivo
            return false;
        } else {
            char *var = new char[r.size()]; // para guardar o conteúdo desserializado
            this->seekg(index2pos(i), ios::beg); // posiciono a cabeça de leitura
            this->read(var, r.size()); // li do arquivo e guardei em var - desserializei
            string serializedRecord = string(var, r.size());
            r.fromString(serializedRecord);
            delete[] var;
            var = nullptr;
            if (fstream::good()){
                return true;
            } else {
                this->clear();
                return false;
            }
        }
    } else {
        this->clear();
        return false;
    }
}
// basicamente realiza a serialização do registro
template <class T>                          // índice onde irei escrever
bool typedFile<T>::writeRecord(record<T> &r, unsigned long long int i) {
    if (isOpen() && fstream::good()){ // verifica se está aberto
        string reg = r.toString(); // serializo o registro
        this->seekp(index2pos(i), ios::beg);
        this->write(reg.c_str(), r.size());
        if (fstream::fail()){
            this->clear();
            cout << "Não foi possível escrever o registro! " <<  endl;
            return false;
        } else {
            return true;
        }
    } else {
        this->clear();
        return false;
    }
}

template <class T>
bool typedFile<T>::insertRecord(record<T> &r) { // verificar se está aberto antes de inserir
    if (isOpen() && fstream::good()){
        unsigned long long int aux = this->getFirstDeleted();
        if (aux != 0){ // então devo reciclar
            // arrumando o conteúdo do primeiro excluído do cabeçalho:
            record<T> varR;
            if (this->readRecord(varR, aux)){
                this->head.setFirstDeleted(varR.getNext());
            }
            r.undel();
            r.setNext(head.getFirstValid()); // arrumando e escrevendo o novo registro
            if (this->writeRecord(r, aux)){ // vou escrever o registro r na pos de aux, antigo firstDeleted
                this->head.setFirstValid(aux); // arrumando o primeiro válido
            }
        } else { // basta inserir no final do arquivo
            r.setNext(this->getFirstValid());
            this->seekp(0, ios::end);
            unsigned long long int pos = this->tellp(); // verifico onde está minha cabeça de escrita
            if (this->writeRecord(r, pos2index(pos))){
                head.setFirstValid(pos2index(pos));
            }
        }
        if (this->writeHeader(this->head) && fstream::good()){
            return true;
        } else {
            this->clear();
            return false;
        }
    } else {
        this->clear();
        return false;
    }
}

template <class T>              // n° do registro
bool typedFile<T>::deleteRecord(unsigned long long int i) { // verifico se está aberto o arquivo
    if (isOpen() && fstream::good()){ // verifico também se está tudo ok com o arquivo pela flag good
        record<T> r;
        if (this->readRecord(r, i)){ // tento ler o registro a ser excluído - desserializa
            r.del();
            if (this->getFirstValid() == i){
                head.setFirstValid(r.getNext());// altero o primeiro válido
            } else {
                record<T> aux;  // para guardar o registro anterior ao que desejo excluir
                unsigned long long int apont = this->getFirstValid();
                while ((aux.getNext() != i) && (readRecord(aux, apont))) {
                    if (aux.getNext() != i) {
                        apont = aux.getNext();
                    }
                }
                aux.setNext(r.getNext());// altero o prox valido
                // escrever o registro aux no arquivo
                this->writeRecord(aux, apont);
            }
            r.setNext(head.getFirstDeleted());
            head.setFirstDeleted(i); // n° do registro
            // alterando o cabeçalho e o registro no arquivo agora
            if ((this->writeRecord(r, i)) && (this->writeHeader(this->head))){
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        this->clear();
        return false;
    }
}

template <class T>
unsigned long long int typedFile<T>::getFirstValid() {
    // lendo o cabeçalho a partir do arquivo:
    if (this->readHeader(this->head)){
        return head.getFirstValid();
    } else {
        return 0;
    }
}

template <class T>
unsigned long long int typedFile<T>::getFirstDeleted() {
    // lendo o cabeçalho a partir do arquivo:
    if (this->readHeader(this->head)){
        return head.getFirstDeleted();
    } else {
        return 0;
    }
}

template <class T>
unsigned long long int typedFile<T>::getRaiz() {
    // lendo o cabeçalho a partir do arquivo:
    if (this->readHeader(this->head)){
        return head.getRaiz();
    } else {
        return 0;
    }
}

template <class T>             // receberá um tipo T e retornará o número do registro
unsigned long long int typedFile<T>::search(T data) { // verifico se está aberto o arquivo antes
    if (isOpen() &&  fstream::good()){
        unsigned long long int aux, achei = 0;
        record<T> r;
        r.setNext(this->getFirstValid());
        aux = r.getNext();
        while (this->readRecord(r, r.getNext())){
            if (r.getData() == data){
                achei = aux; // busca e guarda a pos do registro em achei
                break;
            } else {
                if (r.getNext() == 0){
                    break;
                }
            }
            aux = r.getNext();
        }
        return achei;
    } else {
        this->clear();
        return 0;
    }
}

//lê o cabeçalho
template <class T>
bool typedFile<T>::readHeader(header &h) { // desserializa
    if (isOpen() && fstream::good()){
        char *var = new char [h.size()];
        this->seekg(0, ios::beg);
        this->read(var, h.size());
        string repr = string(var, h.size());
        h.fromString(repr);
        delete[] var;
        var = nullptr;
        if (fstream::good()){
            return true;
        } else {
            this->clear();
            return false;
        }
    } else {
        this->clear();
        return false;
    }
}

//escreve no cabeçalho
template <class T>
bool typedFile<T>::writeHeader(header &h) { // serializa
    if (isOpen() && fstream::good()){
        string serializedHeader = h.toString();
        this->seekp(0, ios::beg);
        this->write(serializedHeader.c_str(), h.size());
        if (fstream::good()){
            return true;
        } else {
            this->clear();
            return false;
        }
    } else {
        this->clear();
        return false;
    }
}
// com base no n° do registro é calculada a pos em bytes
template <class T>
unsigned long long int typedFile<T>::index2pos(unsigned long long int i) {
    record<T> r;
    return (head.size()+(i-1)*r.size());
}

template <class T> // com base nos bytes retorno o n° do registro
unsigned long long int typedFile<T>::pos2index(unsigned long long int p) {
    record<T> r;
    return (p - head.size())/r.size()+1;
}
#endif // TYPEDFILE_H
