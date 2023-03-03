/*-----------------------------------------------------
                     Trabalho Final
    Disciplina: Técnicas de Programação
    Data: 22/01/2023
    Aluna1: Patrícia Crescêncio Martins | RA: 0040732
    Aluna2: Letícia Moreira Leonel | RA: 0040287
-----------------------------------------------------*/
#ifndef NODE_H
#define NODE_H // nós da árvore

#include "serializable.h"
#include <vector> //para armazenar as chaves e os filhos

using namespace std;
        // tipo que será guardado e grau mínimo
template <class T, const unsigned int MIN_DEGREE>
class node : public serializable {
    static_assert(is_base_of<serializable, T>::value, "T must be serializable");
    public:
        node(); //construtor padrão
        node(const node &no); //construtor de cópia
        virtual ~node() {} //destrutor
        void setN(unsigned int n){ this->n = n; } //define a quantidade de chaves do nó
        unsigned int getN() const { return this->n; } // retorna a quantidade de chaves do nó
        void setKey(T k, unsigned int i) { this->keys[i] = k; } // define a chave na posição determinada
        T getKey(unsigned int i) const { return this->keys[i]; } //retorna a chave na posição entregue
        void setLeaf(bool l){ this->leaf = l; } //define a folha
        bool isLeaf() const { return this->leaf; } //retorna se é folha - getLeaf()
        void setChildren(unsigned long long int pos, unsigned int i) { this->childrens[i] = pos; } // pos: índice do filho no arquivo; i: índice do vetor -define o filho
        unsigned long long int getChildren(unsigned int i) const { return this->childrens[i]; } //retorna o filho naquela posição
        void resize(unsigned int s); // altera o tamanho do nó
        void insertKey(T k); // insere a chave na posição correta
        int hasKey(T k); // encontra se o nó tem a chave
        void removeKey(unsigned long long int i, string type); //remoção da chave
        void insertChildren(unsigned long long int pos, string aux); //inserção do filho
        void rotateKeys(unsigned long long int i, node<T, MIN_DEGREE> &y, node<T, MIN_DEGREE> &z, string aux); //rotaciona as chaves nos nós
        void mergeChildren(unsigned long long int i, node<T, MIN_DEGREE> &y, node<T, MIN_DEGREE> &z); //fazendo a fusão dos filhos
        bool operator==(const node<T, MIN_DEGREE> &other) const;
        static const unsigned int MAX = 2*MIN_DEGREE-1;
        static const unsigned int MIN = MIN_DEGREE-1;
        static const unsigned int NOT_FOUND = -1;
        virtual string toString(); //serialização
        virtual void fromString(string repr); //desserialização
        virtual string toCSV() { return ""; }
        virtual void fromCSV(string repr) {}
        virtual string toJSON() { return ""; }
        virtual void fromJSON(string repr) {}
        virtual string toXML() { return ""; }
        virtual void fromXML(string repr) {}
        virtual unsigned long long int size() const; //retorna o tamanho do nó para serializar
    protected:
        unsigned long long int n; // 8 bytes de tamanho
        bool leaf;
        vector<T> keys;
        vector<unsigned long long int> childrens;
};
//construtor padrão
template <class T, const unsigned int MIN_DEGREE>
node<T, MIN_DEGREE>::node(){
    unsigned int i; // i para usar no for
    this->setLeaf(true); // inicialmente crio um nó folha
    this->setN(0); // inicialmente não possui nenhuma chave armazenada
    keys.resize(MAX); // defino o tamanho do vetor de chaves
    childrens.resize(MAX+1); // defino o tamanho do vetor de filhos
    for (i = 0; i < childrens.size(); i++){
        this->setChildren(NOT_FOUND, i); // a folha não tem filhos, por isso entregamos NOT_FOUND
    }
}
//construtor de cópia
template <class T, const unsigned int MIN_DEGREE>
node<T, MIN_DEGREE>::node(const node<T, MIN_DEGREE> &no){
    unsigned int i;
    keys.resize(MAX); //altera o tamanho do vetor de chaves
    for (i = 0; i < no.keys.size(); i++){
        this->keys[i] = no.getKey(i); // entrega as chaves
    }
    childrens.resize(MAX+1); //altera o tamanho do vetor de filhos
    for (i = 0; i < no.childrens.size(); i++){
        this->childrens[i] = no.getChildren(i); //entrega a posição dos filhos no arquivo
    }
    this->leaf = no.isLeaf(); //se for folha
    this->n = no.getN(); //retorna a quantidade de chaves do nó
}
// altera o tamanho do nó
template <class T, const unsigned int MIN_DEGREE>
 void node<T, MIN_DEGREE>::resize(unsigned int s){
    unsigned int i;
    for (i = s+1; i <= this->getN(); i++){ //percorre o vetor de filhos
        this->setChildren(NOT_FOUND, i); //define o filho como NOT_FOUND na posição do i
    }
    if(s == 0){
        this->setChildren(NOT_FOUND, 0);
    }
    this->setN(s); //definindo a quantidade
}
// insere a chave na posição correta
template <class T, const unsigned int MIN_DEGREE>
void node<T, MIN_DEGREE>::insertKey(T k){
    int j = getN()-1;
    while (j >= 0 && k < getKey(j)) {
        setKey(getKey(j), j + 1);
        j--;
    }
    setKey(k, j + 1); //define a posição da chave como j+1
    resize(getN()+1); //altera o tamanho
}

// encontra se o nó tem a chave
template<class T, const unsigned int MIN_DEGREE>
int node<T, MIN_DEGREE>::hasKey(T k)
{
    unsigned int i = 0;
    int pos = NOT_FOUND;
    while (i < getN() && pos == -1)
    {
        if (getKey(i) == k)
        {
            pos = i; // posição da chave no nó
        }
        i++;
    }
    return pos; //retorna a posição encontrada ou não
}

// remoção da chave
template<class T, const unsigned int MIN_DEGREE>
void node<T, MIN_DEGREE>::removeKey(unsigned long long int i, string type)
{
    unsigned int j = i;
    while (j < getN() - 1)  //enquanto j for menor que a quantidade de chaves do nó -1
    {
        setKey(getKey(j + 1), j);
        j++;
    }
    if(!isLeaf()) // se for folha
    {
        if (type == "M"){ // caso em que vou reorganizar os filhos após fusão - merge
            j = i+1;
            while(j < getN())
            {
                setChildren(getChildren(j+1), j);
                j++;
            }
        } else { // caso em que vou reorganizar os filhos após rotação
            j = i;
            while(j < getN())
            {
                setChildren(getChildren(j+1), j);
                j++;
            }
        }
    }
    resize(getN() - 1); //altera o tamanho do nó
}

//inserindo o filho
template<class T, const unsigned int MIN_DEGREE>
void node<T, MIN_DEGREE>::insertChildren(unsigned long long int i, string aux)
{
    if (aux == "P"){ // caso do predecessor
        int j = this->getN();
        while (j > 0)
        {
            this->setChildren(this->getChildren(j - 1), j);
            j--;
        }
        this->setChildren(i, 0);
    } else { // caso do sucessor
        this->setChildren(i, this->getN());
    }
}

//rotação de chaves
template<class T, const unsigned int MIN_DEGREE>
void node<T, MIN_DEGREE>::rotateKeys(unsigned long long int i, node<T, MIN_DEGREE> &y, node<T, MIN_DEGREE> &z, string aux)
{
    if(aux == "P") // rotação com o predecessor
    {
        z.insertKey(getKey(i));
        setKey(y.getKey(y.getN() - 1), i);

        if(!y.isLeaf())
        {
            //realoca os filhos
            z.insertChildren(y.getChildren(y.getN()), aux);
        }
        y.setN(y.getN() - 1);
    }else // rotação com o sucessor
    {
        y.insertKey(getKey(i));
        setKey(z.getKey(0), i);
        if(!z.isLeaf())
        {
            y.insertChildren(z.getChildren(0), aux);
        }
        z.removeKey(0, "R");
    }
}

//fazendo a fusão dos filhos
template<class T, const unsigned int MIN_DEGREE>
void node<T, MIN_DEGREE>::mergeChildren(unsigned long long int i, node<T, MIN_DEGREE> &y, node<T, MIN_DEGREE> &z)
{
    long long int j;
    y.insertKey(getKey(i));
    for(j = 0; j < MIN; j++)
    {
        y.insertKey(z.getKey(j));
    }
    if(!y.isLeaf()) // se não for folha
    {
        for(j = 0; j <= z.getN(); j++) {
            y.setChildren(z.getChildren(j), MIN_DEGREE+j);
        }
    }
    removeKey(i, "M"); //remove a chave após a fusão
    z.resize(0); //altera o tamanho
}

template<class T, const unsigned int MIN_DEGREE>
bool node<T, MIN_DEGREE>::operator==(const node<T, MIN_DEGREE> &other) const {
    return (this->n == other.getN() && this->leaf == other.isLeaf() &&
            this->keys == other.keys && this->childrens == other.childrens);
}


template <class T, const unsigned int MIN_DEGREE>
string node<T, MIN_DEGREE>::toString(){
    unsigned int i;
    string aux = ""; // zero o conteúdo da string para evitar lixo
    aux += string(reinterpret_cast<char*>(&n), sizeof(n)); // concateno o conteúdo de aux com n
    aux += string(reinterpret_cast<char*>(&leaf), sizeof(leaf)); // cria um objeto anônimo do tipo string
    for (i = 0; i < keys.size(); i++){
        aux += keys[i].toString(); // concateno em aux a chave convertida para string
    }
    for (i = 0; i < childrens.size(); i++){
        aux += string(reinterpret_cast<char*>(&childrens[i]), sizeof(childrens[i]));
    }   // concateno em aux a pos correspondentes aos filhos
   return aux;
}

template <class T, const unsigned int MIN_DEGREE>
void node<T, MIN_DEGREE>::fromString(string repr){
    unsigned int i;
    int pos = 0;
    repr.copy(reinterpret_cast<char*>(&n), sizeof(n), pos); //copio para n a partir da pos 0, o tamanho de n
    pos += sizeof(n); //avanço a posição
    repr.copy(reinterpret_cast<char*>(&leaf), sizeof(leaf), pos); //copia da string para o atributo leaf
    pos += sizeof(leaf); // avanço a posição
    string reprData;
    for (i = 0; i < keys.size(); i++){
        reprData = "";
        reprData = repr.substr(pos, keys[i].size());
        keys[i].fromString(reprData);
        pos += keys[i].size();
    }
    for (i = 0; i < childrens.size(); i++){
        repr.copy(reinterpret_cast<char*>(&childrens[i]), sizeof(childrens[i]), pos);
        pos += sizeof(childrens[i]);
    }
}
//define o tamanho do nó
template<class T, const unsigned int MIN_DEGREE>
unsigned long long int node<T, MIN_DEGREE>::size() const {
    return (sizeof(n)+sizeof(leaf)+(keys.size()*keys[0].size())+(childrens.size()*sizeof(unsigned long long int)));
}                               //quant de keys no vetor * tam de cada chave, por ex o da pos 0.

#endif // NODE_H
