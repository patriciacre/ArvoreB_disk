/*-----------------------------------------------------
                     Trabalho Final
    Disciplina: Técnicas de Programação
    Data: 22/01/2023
    Aluna1: Patrícia Crescêncio Martins | RA: 0040732
    Aluna2: Letícia Moreira Leonel | RA: 0040287
-----------------------------------------------------*/
#ifndef DBTREE_H
#define DBTREE_H
#include "typedFile.h"
#include "node.h"
#include <string>
#include <vector>
using namespace std;
// árvore

template <class T, const unsigned int MIN_DEGREE>
class dbtree : private typedFile<node<T, MIN_DEGREE>> {
    static_assert(is_base_of<serializable, T>::value, "T must be serializable"); //obriga T a ser filha de serializable
    public:
        dbtree(); // construtor padrão
        dbtree(const string name, const string type, const unsigned int version); // construtor parametrizado
        virtual ~dbtree() { this->close(); } // destrutor
        void setRoot(node<T, MIN_DEGREE> root) { this->root = root; } // set do atributo root
        node<T, MIN_DEGREE> getRoot() const { return this->root; } // get do atributo root
        bool close() { return typedFile<node<T, MIN_DEGREE>>::close(); } // fecha o arquivo da árvore
        int search(node<T, MIN_DEGREE> &x, T k); // faz a busca de um nó na árvore
        void insert(T k); // insere um valor na árvore
        void print(); // mostra a árvore no console
        bool isOPEN(); // se o arquivo da árvore está aberto
        void remove(T k); // remove um valor da árvore
        void updateNode(node<T, MIN_DEGREE> &n, unsigned long long int i); // atualiza o nó
    protected:
        node<T, MIN_DEGREE> root; // atributo que representa a raiz
        int searchAux(node<T, MIN_DEGREE> &x, T k); // auxiliar de busca
        void splitChildren(node<T, MIN_DEGREE> &x, unsigned int i, node<T, MIN_DEGREE> &y, unsigned long long int iAux); //divide o filho
        void insertNonFull(node<T, MIN_DEGREE> &x, T k, unsigned long long int i); //insere no nó adequado
        void printAux(node<T, MIN_DEGREE> n, vector<string> &v, unsigned int lvl); // auxiliar do print
        void removeAux(node<T, MIN_DEGREE> &x, T k, unsigned long long int i); // auxiliar da remoção
        T findPredecessor(node<T, MIN_DEGREE> &x, unsigned long long int i); // encontra o predecessor
        T findSucessor(node<T, MIN_DEGREE> &x, unsigned long long int i);// encontra o sucessor
        void defineRoot(node<T, MIN_DEGREE> &r, unsigned long long int iR, unsigned long long int iD); //define a nova raiz quando ela ficar vazia após fusão
};

// herda do construtor padrão de typedFile
template <class T, const unsigned int MIN_DEGREE>
dbtree<T, MIN_DEGREE>::dbtree():typedFile<node<T, MIN_DEGREE>>::typedFile()
{

}

// herda do construtor parametrizado de typedFile
template <class T, const unsigned int MIN_DEGREE>
dbtree<T, MIN_DEGREE>::dbtree(const string name, const string type, const unsigned int version):typedFile<node<T, MIN_DEGREE>>(name, type, version){
    header h; //para armazenar os dados do cabeçalho
    typedFile<node<T, MIN_DEGREE>>::readHeader(h); // leio o cabeçalho do disco
    if (h.getRaiz() == 0){ // não há uma raiz para a árvore no arquivo então vou criá-la
        node<T, MIN_DEGREE> n;
        this->root = n;
        record<node<T, MIN_DEGREE>> r(this->root);
        typedFile<node<T, MIN_DEGREE>>::insertRecord(r); // insere o registro da raiz no disco
        typedFile<node<T, MIN_DEGREE>>::readHeader(h); // lê o cabeçalho do disco
        h.setRaiz(h.getFirstValid());
        typedFile<node<T, MIN_DEGREE>>::writeHeader(h); // atualizo o cabeçalho do disco com a informação da raiz
    } else { // já existe uma raiz no arquivo então vou ler e trazer para memória
        unsigned long long int i = h.getRaiz();
        record<node<T, MIN_DEGREE>> rr; //cria um registro para armazenar o nó da raiz
        typedFile<node<T, MIN_DEGREE>>::readRecord(rr, i); // insere o registro do nó no disco
        this->root = rr.getData(); // pego a raiz que já existe no disco
    }
}

//busca por um valor na árvore
template <class T, const unsigned int MIN_DEGREE>
int dbtree<T, MIN_DEGREE>::search(node<T, MIN_DEGREE> &x, T k){
    x = this->root; //define quem é a raiz
    return searchAux(x, k);
}

//auxiliar para a busca
template <class T, const unsigned int MIN_DEGREE>
int dbtree<T, MIN_DEGREE>::searchAux(node<T, MIN_DEGREE> &x, T k){
    unsigned int i = 0;
    while(i < x.getN() && k > x.getKey(i)){ //pesquisa a chave
        i++;
    }
    if (i < x.getN() && k == x.getKey(i)){ //se a chave estiver no nó
        return i;
    } else { //se não estiver no nó e for folha
        if (x.isLeaf()){
            return -1;
        } else {
            record<node<T, MIN_DEGREE>> r; //crio um registro para pegar o nó
            typedFile<node<T, MIN_DEGREE>>::readRecord(r, x.getChildren(i)); //leio do arquivo
            x = r.getData(); //retorna o nó pra x
            return searchAux(x, k); //retorno recursivo do filho de x e da chave
        }
    }
}

//para inserir um valor na árvore
template <class T, const unsigned int MIN_DEGREE>
void dbtree<T, MIN_DEGREE>::insert(T k){
    unsigned long long int ir; // para guardar o índice da raiz
    ir = typedFile<node<T, MIN_DEGREE>>::getRaiz(); // recebe o índice da raiz
    if (this->root.getN() == node<T, MIN_DEGREE>::MAX){ // verifico se a raiz está cheia
        node<T, MIN_DEGREE> s; // crio um novo nó s
        this->setRoot(s); // seto s como a nova raiz
        this->root.setLeaf(false);
        this->root.setChildren(ir, 0); // a antiga raiz é o filho da esquerda da nova raiz
        record<node<T, MIN_DEGREE>> reg;
        typedFile<node<T, MIN_DEGREE>>::readRecord(reg, ir); // lê o registro correspondente a antiga raiz
        node<T, MIN_DEGREE> r;
        r = reg.getData(); // r é o nó correspondente a antiga raiz
        splitChildren(this->root, 0, r, 0); // entrego a nova raiz e r pois serão modificadas e devo escrever no disco
        unsigned long long int i = typedFile<node<T, MIN_DEGREE>>::getRaiz();
        insertNonFull(this->root, k, i); //insere a partir da raiz que não está mais cheia
    } else {
        insertNonFull(this->root, k, ir); //insere a partir da raiz que não está cheia
    }
}

//divide o filho
template <class T, const unsigned int MIN_DEGREE>
void dbtree<T, MIN_DEGREE>::splitChildren(node<T, MIN_DEGREE> &x, unsigned int i, node<T, MIN_DEGREE> &y, unsigned long long int iAux){
    node<T, MIN_DEGREE> z; //cria um nó
    unsigned int j;
    z.setLeaf(y.isLeaf());
    z.setN(z.MIN);
    for (j = 0; j < z.MIN; j++){ // insere as chaves em z
        z.setKey(y.getKey(j+MIN_DEGREE), j);
    }
    if (!y.isLeaf()){ // insere os filhos em z - caso necessário
        for (j = 0; j < MIN_DEGREE; j++){
            z.setChildren(y.getChildren(j+MIN_DEGREE), j);
        }
    }
    y.setN(y.MIN);
    for (j = (x.getN()); j >= i+1; j--){ // reorganiza os filhos de x - caso existam
        x.setChildren(x.getChildren(j),j+1); //aloca os filhos em um espaço novo
    }
    record<node<T, MIN_DEGREE>> rz(z);
    typedFile<node<T, MIN_DEGREE>>::insertRecord(rz); // grava o registro de z no disco
    x.setChildren(typedFile<node<T, MIN_DEGREE>>::getFirstValid(), i+1);
    for (j = x.getN(); j > i; j--){ // reorganiza as chaves de x
        x.setKey(x.getKey(j-1), j);
    }
    x.setKey(y.getKey(y.MIN), i);
    x.setN(x.getN()+1);
    record<node<T, MIN_DEGREE>> ry; //cria um registro para armazenar o nó y
    this->updateNode(y, x.getChildren(i)); //atualiza o nó y que é filho de x
    header h; //cria um cabeçalho auxiliar
    if (x.getN() > 1){ // x já existia no disco
        this->updateNode(x, iAux); //basta atualizar x
    } else { //devo inserir um registro para x no disco
        record<node<T, MIN_DEGREE>> rx(x); //cria um registro com nó
        typedFile<node<T, MIN_DEGREE>>::insertRecord(rx); //insere x que é a nova raiz
        typedFile<node<T, MIN_DEGREE>>::readHeader(h);  //lê no disco o cabeçalho
        h.setRaiz(h.getFirstValid()); //seta a raiz como o primeiro válido
        typedFile<node<T, MIN_DEGREE>>::writeHeader(h); //escreve no disco o cabeçalho atualizado
    }
}

//para inserir o nó todo
template <class T, const unsigned int MIN_DEGREE>
void dbtree<T, MIN_DEGREE>::insertNonFull(node<T, MIN_DEGREE> &x, T k, unsigned long long int i) {
    int j = x.getN()-1; // j armazena quantas chaves há na pág. que vou inserir - 1
    bool flag = false;
    if (x.isLeaf()) { //se for folha
        x.insertKey(k); //insere a chave no x
        this->updateNode(x, i); // atualizo o x
    } else { //se não for folha
        while (j >= 0 && k < x.getKey(j)) {
            j--;
        }
        j++;
        record<node<T, MIN_DEGREE>> rc; //crio um registro para o nó
        typedFile<node<T, MIN_DEGREE>>::readRecord(rc, x.getChildren(j)); //leio do disco o registro que tem o nó
        node<T, MIN_DEGREE> nc;
        nc = rc.getData(); //pego os dados do nó
        if (nc.getN() == node<T, MIN_DEGREE>::MAX) {
            splitChildren(x, j, nc, i); //divide o filho
            if (k > x.getKey(j)) {
                flag = true;
                j++;
            }
        }
        if (flag == true){
            // devo ler novamente o registro do disco pois j foi modificado no if anterior a este
            record<node<T, MIN_DEGREE>> rAtual;
            typedFile<node<T, MIN_DEGREE>>::readRecord(rAtual, x.getChildren(j));
            node<T, MIN_DEGREE> nAtual = rAtual.getData();
            insertNonFull(nAtual, k, x.getChildren(j));  //insere o a chave no nó
        } else {
            insertNonFull(nc, k, x.getChildren(j)); //insere a chave no nó
        }
    }
}

//para mostrar a árvore
template <class T, const unsigned int MIN_DEGREE>
void dbtree<T, MIN_DEGREE>::print()
{
    int lvl = 0; // nível da árvore
    vector<string> levels(1); // vetor que irá armazenar as strings referentes aos nós da árvore
    printAux(this->root, levels, lvl); // chamo printAux para preencher o vetor de strings
    for (string i : levels) { // percorro e mostro o vetor de string
       cout << i << endl;
    }
}

//auxiliar para mostrar a árvore
template <class T, const unsigned int MIN_DEGREE>
void dbtree<T, MIN_DEGREE>::printAux(node<T, MIN_DEGREE> n, vector<string> &v, unsigned int lvl)
{
    record<node<T, MIN_DEGREE>> r; //cria um nó
    string no = "[";
    unsigned int i = 0;
    if (v.size() < lvl + 1) { // verifico se mudei de nivel
        v.resize(lvl + 1);
    }
    if (!n.isLeaf()) {
        for (i = 0; i <= n.getN(); i++) { // percorro e leio do disco os filhos do nó não folha
            if (n.getChildren(i) != n.NOT_FOUND){
                typedFile<node<T, MIN_DEGREE>>::readRecord(r, n.getChildren(i)); //leio no disco
                node<T, MIN_DEGREE> f = r.getData();
                printAux(f, v, lvl+1); // chamo printAux para o nó filho
            }
        }
    }
    for (i = 0; i < n.getN(); i++) {
        no += n.getKey(i).converter()+", "; // concateno na string as chaves do nó
    }
    if (no.length() > 1) { // organiza a string de acordo com seu tamanho
        no += "\b\b] ";
    } else {
        no += "] ";
    }
    v[lvl] += no; // incrementa no vetor a string no nível adequado
}

//para retornar se o arquivo da árvore está aberto
template <class T, const unsigned int MIN_DEGREE>
bool dbtree<T, MIN_DEGREE>::isOPEN()
{
    return typedFile<node<T, MIN_DEGREE>>::isOpen();
}

//remoção
template <class T, const unsigned int MIN_DEGREE>
void dbtree<T, MIN_DEGREE>::remove(T k)
{
    unsigned long long int i = typedFile<node<T, MIN_DEGREE>>::getRaiz(); // recebe o índice da raiz
    removeAux(this->root, k, i); // remove entregando a raiz, o índice e a chave
}

//auxiliar para a remoção
template <class T, const unsigned int MIN_DEGREE>
void dbtree<T, MIN_DEGREE>::removeAux(node<T, MIN_DEGREE> &x, T k, unsigned long long int i)
{
    unsigned int pos = x.hasKey(k); //encontra a pos da chave no nó se existir
    //criação de nós
    record<node<T, MIN_DEGREE>> rX;
    record<node<T, MIN_DEGREE>> rY;
    record<node<T, MIN_DEGREE>> rZ;
    node<T, MIN_DEGREE> y;
    node<T, MIN_DEGREE> z;
    unsigned long long int auxP; // para guardar o índice do filho de x quando x for deletado
    T aux;
    string tipo = "";
    if(x.isLeaf()) //No Caso 1 - se for folha
    {
        if(pos != node<T, MIN_DEGREE>::NOT_FOUND)  //se a chave está na página, remove
        {
            x.removeKey(pos, "F"); // F significa que é remoção de página folha
            this->updateNode(x, i);
            cout << "Chave removida com sucesso!\n" << endl;
        }else
        {
            cout << "Chave não está na árvore!\n" << endl;
        }
    } else { //se não for folha e a chave estiver em uma página interna chamamos o caso 2
        if(pos != node<T, MIN_DEGREE>::NOT_FOUND && !x.isLeaf())
        {
            dbtree<T, MIN_DEGREE>::readRecord(rY, x.getChildren(pos)); //lê o arquivo
            unsigned long long int endY = x.getChildren(pos);
            y = rY.getData();
            if(y.getN() > node<T, MIN_DEGREE>::MIN) // 2a
            {
                aux = this->findPredecessor(y, endY); //vamos encontrar o predecessor e remover
                x.setKey(aux, pos);
                this->updateNode(x, i); //atualizo o nó
            }else
            {
                dbtree<T, MIN_DEGREE>::readRecord(rZ, x.getChildren(pos+1));
                z = rZ.getData();
                if (z.getN() > node<T, MIN_DEGREE>::MIN){ // 2b
                    aux = this->findSucessor(z, x.getChildren(pos+1)); //encontre o sucessor e remova
                    x.setKey(aux, pos);
                    this->updateNode(x, i); //atualizo o nó
                } else { // ninguém pode emprestar - devemos fazer a fusão - 2c
                    typedFile<node<T, MIN_DEGREE>>::deleteRecord(x.getChildren(pos+1));
                    x.mergeChildren(pos, y, z);
                    if (x == this->root && x.getN() == 0)  // testa se a raiz ficou vazia
                    {
                        defineRoot(y, endY, i);
                        removeAux(this->root, k, endY); //removendo
                    }
                    else
                    {
                        this->updateNode(x, i); //atualizo o nó
                        removeAux(y, k, endY); //removo k de y
                    }
                    this->updateNode(y, endY); // atualizo o y
                }
            }
        } else { // caso 3 - se a chave não está na página interna
            pos = 0;
            while(pos < x.getN()){
                int j = pos;
                if (x.getKey(j) > k || pos == x.getN() - 1) {
                    if (!(x.getKey(j) > k)){ // verifica se k é maior que a chave assim, está na última pos do vetor de chaves
                        j = x.getN();
                    }
                    record<node<T, MIN_DEGREE>> rF;
                    dbtree<T, MIN_DEGREE>::readRecord(rF, x.getChildren(j)); // para não perder a info de next, caso exista
                    node<T, MIN_DEGREE> f;
                    f = rF.getData();
                    bool flag = false;
                    // caso 3a
                    if(f.getN() == node<T, MIN_DEGREE>::MIN)
                    {
                        if (!(j-1 < 0)){ // verifica se existe predecessor
                            dbtree<T, MIN_DEGREE>::readRecord(rY, x.getChildren(j - 1));
                            y = rY.getData();
                            if(y.getN() > node<T, MIN_DEGREE>::MIN) // predecessor pode doar chave
                            {
                                x.rotateKeys(j - 1, y, f, "P"); //rotaciona as chaves de modo que o predecessor doa
                                //atualiza os nós
                                this->updateNode(x, i);
                                this->updateNode(y, x.getChildren(j - 1));
                                this->updateNode(f, x.getChildren(j));
                                flag = true;
                            }
                        }
                        if (pos+1<=x.getN() && flag == false){ //verifica se há sucessor e predecessor não doou
                            dbtree<T, MIN_DEGREE>::readRecord(rZ, x.getChildren(j + 1));
                            z = rZ.getData();
                            if (z.getN() > node<T, MIN_DEGREE>::MIN){ // sucessor pode doar chave
                                x.rotateKeys(j, f, z, "S"); //rotaciona as chaves de modo que o sucessor doa
                                //atualiza os nós
                                this->updateNode(x, i);
                                this->updateNode(z, x.getChildren(j + 1));
                                this->updateNode(f, x.getChildren(j));
                                flag = true;
                            }
                        }  // caso 3b - faz fusão
                        if (j-1 >= 0 && flag == false) { // testa se existe irmão à esquerda para fazer fusão
                            unsigned long long int aux = x.getChildren(j); // índice do registro f
                            auxP = x.getChildren(j-1); // índice do registro y (predecessor)
                            x.mergeChildren(j-1, y, f); // faz a fusão de f em y
                            dbtree<T, MIN_DEGREE>::deleteRecord(aux); // deletamos f
                            if (x == this->root && x.getN() == 0){ // verifico se a raiz ficou vazia
                                defineRoot(y, auxP, i);
                            } else {
                                this->updateNode(x, i); //atualiza o nó
                            }
                            this->updateNode(y, auxP);
                            tipo = "IrmaoDaEsquerda";
                        } else { // fusão com irmão da direita
                            if (flag == false){
                                unsigned long long int auxS = x.getChildren(j + 1); // índice do registro z
                                auxP = x.getChildren(j); // índice do registro f
                                x.mergeChildren(j, f, z); // faz fusão de z em f
                                dbtree<T, MIN_DEGREE>::deleteRecord(auxS); // deletamos Z
                                if (x == this->root && x.getN() == 0){ // verifico se a raiz ficou vazia
                                    defineRoot(f, auxP, i);
                                } else {
                                    this->updateNode(x, i);
                                }
                                this->updateNode(f, auxP);
                            }
                        }
                    }
                    if (tipo != "IrmaoDaEsquerda"){
                        if (f == this->root){
                            removeAux(this->root, k, auxP);
                        } else {
                            removeAux(f, k, x.getChildren(j)); //remove
                        }
                    } else {
                        if (y == this->root){
                            removeAux(this->root, k, auxP);
                        } else {
                            removeAux(y, k, x.getChildren(j-1));
                        }
                    }
                    break;
                }
                pos++;
            }
        }
    }
}
//define a nova raiz quando ela ficar vazia após fusão
template <class T, const unsigned int MIN_DEGREE>
void dbtree<T, MIN_DEGREE>::defineRoot(node<T, MIN_DEGREE> &r, unsigned long long int iR, unsigned long long int iD)
{
    this->setRoot(r);
    header h; //crio um cabeçalho auxiliar
    typedFile<node<T, MIN_DEGREE>>::readHeader(h); //leio o cabeçalho
    h.setRaiz(iR);
    typedFile<node<T, MIN_DEGREE>>::writeHeader(h); //atualizo o cabeçalho
    dbtree<T, MIN_DEGREE>::deleteRecord(iD); // deleto x, pois ficou vazio
}
//encontra o predecessor
template <class T, const unsigned int MIN_DEGREE>
T dbtree<T, MIN_DEGREE>::findPredecessor(node<T, MIN_DEGREE> &x, unsigned long long int i)
{
    T k;
    if (x.isLeaf()){ //se for folha
        k = x.getKey(x.getN()-1); //define a chave k que é a que está na última posição das chaves do nó
        removeAux(x, k, i); //remove k
        return k;
    } else {
        record<node<T, MIN_DEGREE>> r; //cria um registro para o nó
        node<T, MIN_DEGREE> n;
        dbtree<T, MIN_DEGREE>::readRecord(r, x.getChildren(x.getN())); //lê para o registro o filho adequado
        n = r.getData(); //retona n
        if (n.isLeaf() && n.getN() == n.MIN){ // evita que remova chave de uma folha que já esteja com ocupação mínima
            k = n.getKey(n.getN()-1);
            // removeAux será chamado para realizar a remoção da chave que devo subir
            // depois para o nó não folha
            // de modo que irá cair no caso 3 direto, pois já entrego o nó adequado
            // não sendo necessário percorrer a árvore deste o início
            removeAux(x, n.getKey(n.getN()-1), i);
            dbtree<T, MIN_DEGREE>::readRecord(r, x.getChildren(n.getN()-1));
            n = r.getData();
            return k;
        }
        return findPredecessor(n, x.getChildren(x.getN()));
    }
}

//encontra o sucessor
template <class T, const unsigned int MIN_DEGREE>
T dbtree<T, MIN_DEGREE>::findSucessor(node<T, MIN_DEGREE> &x, unsigned long long int i)
{
    T k;
    if (x.isLeaf()){ //se for folha
        k = x.getKey(0);  //define a chave k que é a que está na primeira posição das chaves do nó
        removeAux(x, k, i); //remove k
        return k;
    } else {
        record<node<T, MIN_DEGREE>> r; //cria um registro para o nó
        node<T, MIN_DEGREE> n;
        dbtree<T, MIN_DEGREE>::readRecord(r, x.getChildren(0)); //lê para o registro o filho adequado
        n = r.getData();
        if (n.isLeaf() && n.getN() == n.MIN){ // evita que remova chave de uma folha que já esteja com ocupação mínima
            k = n.getKey(0);
            // removeAux será chamado para realizar a remoção da chave que devo subir
            // depois para o nó não folha
            // de modo que irá cair no caso 3 direto, pois já entrego o nó adequado
            // não sendo necessário percorrer a árvore deste o início
            removeAux(x, n.getKey(0), i);
            dbtree<T, MIN_DEGREE>::readRecord(r, x.getChildren(0));
            n = r.getData();
            return k;
        }
        return findSucessor(n, x.getChildren(0));
    }
}

//atualiza o nó
template <class T, const unsigned int MIN_DEGREE>
void dbtree<T, MIN_DEGREE>::updateNode(node<T, MIN_DEGREE> &n, unsigned long long int i){
    record<node<T, MIN_DEGREE>> r;
    typedFile<node<T, MIN_DEGREE>>::readRecord(r, i); // leio para não perder o next
    r.setData(n); //define o nó
    typedFile<node<T, MIN_DEGREE>>::writeRecord(r, i); // atualiza o nó no disco
}

#endif // DBTREE_H
