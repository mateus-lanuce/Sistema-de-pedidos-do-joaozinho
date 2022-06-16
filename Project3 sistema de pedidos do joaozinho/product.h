#pragma once

struct Product {
    char name[30]{};
    unsigned int stock = 0;
    float price = 0;
};

struct vectorProducts {
    Product *vetDinamicProducts;
    int usedSize = 0;
    int totalSize = 0;
};

/*
* lê um arquivo de pedido
* grava um recibo
* atualiza as quantidades dos produtos em estoque
* falha se não tem estoque em algum produto e exibe o erro, e não faz alterações nem gera recibo.
*/
void order(vectorProducts &Products);

/*
* adiciona um novo registro ao vetor de estoque
* se o produto já existe só atualiza o preço e a quantidade.
* atualiza o tamanho usado.
*/
void addProduct(vectorProducts& Products);

/*
* mostra uma lista dos produtos e exclui o selecionado pelo usuário
* atualiza o tamanho usado
*/
bool deleteProduct(vectorProducts& Products);

void listProducts(const vectorProducts& Products);

/*
* cria um novo vetor com igual a capacidade	anterior + 2n
* n = quantidade de vezes que o vetor foi expandido durante a execução
* modifica a variavel de tamanho total com o novo tamanho total.
*/
bool increaseVectorSize(vectorProducts& Products);

//coloca o nome do produto no formato Yxxxx
void formatWord(char word[]);

/*
* verifica se o nome passado está incluso no vetor
* retorna o index com a posição do valor, ou -1 se não existir
*/
int includesOnVector(const vectorProducts& Products, const char name[]);

/*
* verifica se o nome passado está incluso no vetor
* retorna o index com a posição do valor, ou -1 se não existir
*/
int includesOnVector(const Product Products[], const char name[], const int tam);


inline bool haveStock(const vectorProducts& Products, int index, int quantity);