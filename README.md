# ArvoreB_disk

Trabalho desenvolvido junto com Letícia Moreira Leonel: https://github.com/moreiraleticial

O trabalho consiste na implementação de uma Árvore B em disco, desenvolvido para a disciplina de Técnicas de Programação do curso Bacharelado em Engenharia de Computação do IFMG campus Bambuí. 
Segundo o enunciado disponibilizado pelo professor, Samuel Pereira Dias, a árvore B em disco é um arquivo tipado em que cada página é armazenada como um registro do arquivo. O programa armazena um conjunto de objetos do TAD (tipo abstrato de dados) serialCurrency na árvore B, com o armazenamento de cada página da árvore em um registro de arquivo tipado, conforme visto na disciplina. A árvore tem grau mínimo t = 3. O tipo serialCurrency é um tipo de dados serializável. Seu conteúdo é um valor de ponto fixo, com duas casas decimais, no formato x.y, sendo x a parte inteira, com qualquer número de dígitos, e y a parte fracionária, com exatamente 2 dígitos. Podendo assumir valores positivos ou negativos.

O programa apresenta um menu que permite ao usuário:

   - inserir um valor na árvore;
   - remover um valor da árvore;
   - imprimir a árvore completa;
   - buscar um valor em uma árvore;
   - executar um conjunto de testes padronizados;
   
Após a correção realizada pelo professor são sugeridas algumas melhorias que podem ser implementadas no código:
	Não é preciso converter para string para imprimir da forma solicitada. Uso de strings como parâmetros para mudar comportamento de métodos é não é algo eficiente, seria melhor usar constantes ou enums. Não é aconselhável que métodos de classes façam entrada/saída, especialmente para erros, deve retornar códigos de erro que sejam válidos. A busca do predecessor/sucessor poderia ser mais otimizada.
	
Algumas classes foram desenvolvidas com base em código previamente disponibilizado pelo próprio professor da disciplina. A árvore B desenvolvida seguiu os pseudocódigos de Cormen (2009).

Referência: CORMEN, Thomas H.; LEISERSON, Charles E.; RIVEST, Ronald L.; STEIN, Clifford. Introduction to Algorithms. 3rd ed. Cambridge, Massachusetts: The MIT Press, 2009. 1292 p. ISBN 978-0-262-53305-8.
