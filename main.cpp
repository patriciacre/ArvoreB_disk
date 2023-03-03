/*-----------------------------------------------------
                     Trabalho Final
    Disciplina: Técnicas de Programação
    Data: 22/01/2023
    Aluna1: Patrícia Crescêncio Martins | RA: 0040732
    Aluna2: Letícia Moreira Leonel | RA: 0040287
-----------------------------------------------------*/

#include <iostream>
#include <clocale>
#include "serialCurrency.h"
#include "dbtree.h"

using namespace std;

int main() {
   setlocale(LC_ALL, "Portuguese");
   int op;
   float i;
   serialCurrency n;
                                        // mod relativo à moeda na versão 1
   dbtree<serialCurrency, 3> btree("arq.dat", "MOD", 1); // rvore em arquivo de registros do tipo serialCurrency
   node<serialCurrency, 3> no; // utilizado para apresentar o nó em que uma chave buscada estiver
   vector<serialCurrency> inserir = {5.50, 2.01, -5.33, 10.05, 21.13, 1.88, -3.14, 2.86, 37.55, 2.99, 34.31,
                                    49.00, 38.26, 20.85, 23.02, 17.01, 14.92, 13.81, 39.89, -13.22, 7.20};
   vector<serialCurrency> remover ={5.50, -5.33, 21.13, 1.88, -3.14, 34.31, 38.26, 23.02, 14.92, 39.89, 7.20};

   if (btree.isOPEN()) //se o arquivo da arvore estiver aberto
   {
      cout << "Árvore está pronta!\n\n";

      do {
         cout << "\n--------------------------------" <<endl;
         cout << "\nSelecione uma opção:\n\n"
              << "1. Inserir um valor na árvore\n"
              << "2. Excluir um valor na árvore\n"
              << "3. Pesquisar um valor na árvore\n"
              << "4. Imprimir a árvore completa\n"
              << "5. Executar casos de teste\n"
              << "6. Sair da árvore\n\n"
              << "Sua opção: ";
         cin >> op;
         switch (op) {
            case 1: //caso 1 - inserção
            {
               cout << "Digite um número: ";
               cin >> n;
               btree.insert(n); //inserindo
               cout << "\n" << endl;
               btree.print(); //mostrando
               break;
            }
            case 2: //caso 2 - remoção
            {
               cout << "Digite o valor que deseja remover da árvore: ";
               cin >> n;
               btree.remove(n); //removendo
               btree.print(); //mostrando
               break;
            }
            case 3:  //caso 3 - busca
            {
               cout << "Digite o valor a ser pesquisado: ";
               cin >> n;
               i = btree.search(no, n); //busca na árvore entregando o valor digitado
               if (i != -1){
                  cout << "Valor " << n << " encontrado na árvore no seguinte nó: \n" << endl; //mostrando em qual nó o valor digitado está
                  cout << "[ ";
                  for (unsigned int j = 0; j <= no.getN()-1; j++){
                    cout << no.getKey(j) << " ";
                  }
                  cout << "]" << endl;
                }
               else
                  cout << "Valor " << n << " não encontrado.\n" << endl;

               break;
            }
            case 4: //caso 4 - mostrando na tela a árvore completa
            {
               cout << "Imprimindo a árvore completa: \n" << endl;
               btree.print();
               break;
            }
            case 5:  //caso 5 - casos de teste disponibilizados para o trabalho
            {
                cout << "Inserindo os testes da árvore:" << endl;
                for(serialCurrency in: inserir)
                {
                    cout << "\nInserindo chave " << in << ":" << endl;
                    btree.insert(in); //inserindo casos
                    btree.print(); //mostrando a árvore
                }
                cout << "\nRemovendo os casos de teste da árvore:" << endl;
                for(serialCurrency r: remover)
                {
                    cout << "\nRemovendo chave " << r << ":" << endl;
                    btree.remove(r);  //removendo os casos
                    btree.print(); //mostrando a árvore
                }
                break;
            }
            case 6: //caso 6 - encerramento do programa
            {
               cout << "Encerrando o programa... ";
               btree.close(); //fechando a árvore
               cout << "Concluído." << endl;
               return 0;
               break;
            default:
               cout << "Opção inválida! Tente novamente..." << endl; //caso o usuário vá inserir algo não pedido
               break;
            }
         }
      } while (true);
   } else { // se a árvore não for iniciada
      cout << "Não foi possível iniciar a árvore!" << endl;
      return 1;
   }
   return 0;
}
