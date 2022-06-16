#include "product.h"
#include <iostream>
#include <fstream>
#include <cmath>

void order(vectorProducts& Products) {

	char header[] = "Pedir";
	std::cout << header << std::endl;

	for (int i = 0; i < strlen(header); i++) {
		std::cout << '-';
	}

	std::cout << std::endl;

	char nameFile[30];

	std::cout << "Arquivo: ";
	std::cin >> nameFile;

	std::ifstream fin;

	fin.open(nameFile);

	if (fin.good()) {

		char name[10]; //nome da pizzaria
		char tempLine[40];

		//le o cabeçalho do arquivo
		fin >> name >> name;
		fin.get();
		fin.getline(tempLine, 40);

		// contar quantas linhas de pedidos tem no arquivo para criar o vetor de produtos pedidos.
		int linhas = 1;
		char ch;
		while (fin.get(ch)) {
			if (ch == '\n')
				linhas++;
		}

		fin.close();

		//vetor de produtos pedidos
		Product* orderProducts = new Product[linhas];

		//reabre o arquivo e retira as duas primeiras linhas para ler os pedidos;

		fin.open(nameFile);
		fin.getline(tempLine, 40);
		fin.getline(tempLine, 40);

		//le os valores e coloca no vetor de pedidos e testa se existe no vetor principal e se tem estoque;
		bool orderIsPossible = true;
		int indexOnVectorProducts = 0;
		int indexOnOrderVector = 0;

		int distinctOrders = 0;

		//while que faz a leitura e organiza os valores.
		Product tempProduct{};
		do
		{
			fin >> tempProduct.name;

			formatWord(tempProduct.name);

			fin >> tempProduct.stock;

			//variavel que testa se já inclui no vetor de pedidos para apenas aumentar a quantidade pedida
			indexOnOrderVector = includesOnVector(orderProducts, tempProduct.name, linhas);

			//se já existe adiciona a quantidade de estoque pedido na posicao e diminui a quantidade de distintos
			if (indexOnOrderVector != -1) {
				orderProducts[indexOnOrderVector].stock += tempProduct.stock;
			}
			else {
				orderProducts[distinctOrders] = tempProduct;
				distinctOrders++;
			}

		} while (!fin.eof());

		//verifica se o pedido é possivel
		for (int i = 0; i < distinctOrders; i++) {

			//variavel que testa se o valor existe no vetor pai
			indexOnVectorProducts = includesOnVector(Products, orderProducts[i].name);

			if (indexOnVectorProducts != -1) {
				if (!haveStock(Products, indexOnVectorProducts, orderProducts[i].stock)) {
					orderIsPossible = false;
				}
			}
			else {
				orderIsPossible = false;
			}

		}


		//faz a gravação do pedido se for possível e se não for mostra o erro.
		if (orderIsPossible) {

			std::ofstream fout;

			char nfcName[30];

			strcpy_s(nfcName, nameFile);

			//gera o arquivo .nfc
			for (int i = 0; i < 30; i++) {

				if (nfcName[i] == '.') {
					nfcName[(i + 1)] = 'n';
					nfcName[(i + 2)] = 'f';
					nfcName[(i + 3)] = 'c';

					break;
				}

			}

			fout.open(nfcName);

			if (fout.good()) {

				fout << "Pizzaria " << name;
				fout << "\n--------------------------------------------------\n";

				float orderValue = 0, purchase = 0, discont = 0, amount = 0;
				
				//gera cada linha de pedidos
				for (int i = 0; i < distinctOrders; i++) {
					indexOnVectorProducts = includesOnVector(Products, orderProducts[i].name);
					
					fout.width(10);
					fout << std::left << orderProducts[i].name;
					fout.width(5);
					fout << std::right << orderProducts[i].stock; 
					fout << std::left << "Kg";
					fout.width(5);
					fout << std::right << " a ";
					fout.width(7);
					fout.precision(2);
					fout << std::right << Products.vetDinamicProducts[indexOnVectorProducts].price;
					fout << std::left << "/Kg";
					fout.width(4);
					fout << std::right << " = ";
					fout.precision(2);

					//quantidade pedida vezes o preço
					orderValue = orderProducts[i].stock * Products.vetDinamicProducts[indexOnVectorProducts].price;

					fout.width(5);
					fout.precision(2);
					fout << std::right << "R$";
					fout << std::left << std::fixed << orderValue << std::endl;

					purchase += orderValue;
				}

				fout << "--------------------------------------------------\n";
				fout.width(31);
				fout << std::right << "Compra";
				fout.width(5);
				fout << " = ";
				fout.width(5);
				fout.precision(2);

				fout << "R$" << purchase << "\n";

				if (purchase >= (float) 1000 ) {
					discont = purchase * 0.10;
					amount = purchase - discont;
				}
				else {
					amount = purchase;
				}

				fout.width(31);
				fout << std::right << "Desconto";
				fout.width(5);
				fout << " = ";
				fout.precision(2);
				fout.width(5);
				fout << "R$" << std::fixed << discont << "\n";
				
				fout.width(31);
				fout << std::right << "Total";
				fout.width(5);
				fout << " = ";
				fout.width(5);
				fout.precision(2);
				fout << "R$" << std::fixed << amount << "\n" << std::endl;

				//arquivo criado

				//-------------diminuir quantidades do vetor de estoque----------

				for (int i = 0; i < distinctOrders; i++) {
					indexOnVectorProducts = includesOnVector(Products, orderProducts[i].name);
				
					Products.vetDinamicProducts[indexOnVectorProducts].stock -= orderProducts[i].stock;
				}

				fout.close();
				fin.close();
			}
			else {
				std::cout << "Erro na criação do arquivo de pedido\n";
				fout.clear();
				fin.close();
			}

		}
		else {

			std::cout << "\n\nPedido falhou!\n";

			// mostra os erros
			for (int i = 0; i < distinctOrders; i++) {

				//variavel que testa se o valor existe no vetor pai
				indexOnVectorProducts = includesOnVector(Products, orderProducts[i].name);

				if (indexOnVectorProducts != -1) {
					if (!haveStock(Products, indexOnVectorProducts, orderProducts[i].stock)) {
						std::cout << orderProducts[i].name << ": Solicitado = " << orderProducts[i].stock << "Kg";
						std::cout << " / Em Estoque = " << Products.vetDinamicProducts[indexOnVectorProducts].stock << "Kg" << std::endl;
					}
				}
				else {
					std::cout << orderProducts[i].name << " Não é um produto cadastrado\n";
				}
			}

			system("pause");
		}



		delete[] orderProducts;

	}
	else {
		fin.clear();
		std::cout << "Não foi possivel ler o arquivo\n";
		system("pause");
	}

}

void addProduct(vectorProducts& Products) {

	char header[] = "Adicionar";
	std::cout << header << std::endl;

	for (int i = 0; i < strlen(header); i++) {
		std::cout << '-';
	}

	std::cout << std::endl;

	Product tempRegister{};

	//leitura dos valores
	std::cout << "Produto: ";
	std::cin >> tempRegister.name;
	std::cout << "Preço: ";
	std::cin >> tempRegister.price;
	std::cout << "Quantidade: ";
	std::cin >> tempRegister.stock;

	
	formatWord(tempRegister.name);

	//testar se já existe no vetor
	bool haveProduct = false;
	int indexIfExist = 0;
	for (int i = 0; i < Products.usedSize; i++) {
		
		if (!strcmp(Products.vetDinamicProducts[i].name, tempRegister.name)) {
			haveProduct = true;
			indexIfExist = i;
		}
	
	}

	if (haveProduct) {
		//ja que existe atualiza o preço e aumenta o estoque
		Products.vetDinamicProducts[indexIfExist].price = tempRegister.price;
		Products.vetDinamicProducts[indexIfExist].stock += tempRegister.stock;
	}
	else {
		//se não existe verificar se cabe e adicionar
		increaseVectorSize(Products);

		//usedSize ultimo index livre
		int lastIndex = Products.usedSize;
		Products.vetDinamicProducts[lastIndex] = tempRegister;

		Products.usedSize++;
	}

}

bool deleteProduct(vectorProducts& Products) {


	char header[] = "Excluir";
	std::cout << header << std::endl;

	for (int i = 0; i < strlen(header); i++) {
		std::cout << '-';
	}

	std::cout << std::endl;

	if (Products.usedSize == 0) {
		std::cout << "Não existem produtos cadastrados\n";
		system("pause");

		return false;
	}

	for (int i = 0; i < Products.usedSize; i++) {

		std::cout << (i + 1) << ") " << Products.vetDinamicProducts[i].name << std::endl;

	}

	uint32_t deleteIndex = 0;
	std::cout << "\n Número do produto: ";
	std::cin >> deleteIndex;

	deleteIndex -= 1;

	//verifica se o indice existe para ser apagado
	if (deleteIndex < Products.usedSize) {
	
		char confirm = 0;
		std::cout << "Deseja excluir " << Products.vetDinamicProducts[deleteIndex].name << " (S/N)? ";
		std::cin >> confirm;

		

		if (tolower(confirm) == 's') {

			//diminuir o tamanho usado e retirar o produto do vetor reorganizando o vetor.

			for (int i = deleteIndex; i < Products.usedSize;) {
				//anterior = proximo a partir do excluido para reorganizar o vetor.
				Products.vetDinamicProducts[i] = Products.vetDinamicProducts[++i];
				
			}


			Products.usedSize--;

		}

	}
	else {
		std::cout << "Você não pode apagar um produto que não está listado!\n";
		system("pause");
		return false;
	}


	system("pause");

	return true;
}

void listProducts(const vectorProducts& Products) {

	char header[] = "Listagem";
	std::cout << header << std::endl;

	for (int i = 0; i < strlen(header); i++) {
		std::cout << '-';
	}

	std::cout << std::endl;

	//percorre todo o tamanho usado do vetor exibindo
	for (int i = 0; i < Products.usedSize; i++) {

		std::cout.width(10); 
		std::cout << std::left << Products.vetDinamicProducts[i].name << " - R$";

		std::cout.width(5);
		std::cout.precision(2);
		std::cout << std::right << std::fixed << Products.vetDinamicProducts[i].price; 
		std::cout.width(4);
		std::cout << std::right << " - ";

		std::cout.width(3);
		std::cout.precision(0);
		std::cout << std::right << std::fixed << Products.vetDinamicProducts[i].stock << "Kg\n";

	}

	system("pause");

}

bool increaseVectorSize(vectorProducts& Products) {
	
	//cria uma variavel estatica que so pode ser inicializada 1 vez e aumenta ela de acordo com as execuções da função.
	static int n;

	Product* tempVet = nullptr;

	if (Products.usedSize < Products.totalSize)
		return false;

	Products.totalSize = Products.usedSize + (pow(2, n));
	n++;

	tempVet = new Product[Products.totalSize];

	for (int i = 0; i <= Products.usedSize; i++) {
		tempVet[i] = Products.vetDinamicProducts[i];
	}

	delete[] Products.vetDinamicProducts;

	Products.vetDinamicProducts = tempVet;

	return true;

}

inline bool haveStock(const vectorProducts& Products, int index, int quantity) {

	if (Products.vetDinamicProducts[index].stock >= quantity) {
		return true;
	}
	else {
		return false;
	}

}

int includesOnVector(const Product Products[], const char name[], const int tam) {
	
	for (int i = 0; i < tam; i++) {
		if (!strcmp(Products[i].name, name))
			return i;
	}

	return -1;
}

int includesOnVector(const vectorProducts& Products, const char ProductName[]) {

	for (int i = 0; i < Products.usedSize; i++) {
		if (!strcmp(Products.vetDinamicProducts[i].name, ProductName)) {
			return i;
		}
	}

	return -1;
}

void formatWord(char word[]) {
	for (int i = 0; word[i] != '\0'; i++) {
		if (i > 0)
			word[i] = tolower(word[i]);
		else
			word[i] = toupper(word[i]);
	}
}