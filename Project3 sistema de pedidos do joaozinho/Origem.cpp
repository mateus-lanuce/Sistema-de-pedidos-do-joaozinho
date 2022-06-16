#include <iostream>
#include <fstream>
#include "product.h"


int main() {

	char choose;
	std::ifstream fin;
	std::ofstream fout;
	vectorProducts Estoque{};

	//tentar abrir o arquivo binario de estoque para iniciar o Estoque
	fin.open("estoque.bin", std::ios_base::in | std::ios_base::binary);

	if (fin.good()) {
		vectorProducts temp{};

		fin.read((char*)& temp.totalSize, sizeof temp.totalSize);
		fin.read((char*)& temp.usedSize, sizeof temp.usedSize);

		Estoque.vetDinamicProducts = new Product[temp.totalSize];

		Estoque.totalSize = temp.totalSize;
		Estoque.usedSize = temp.usedSize;

		for (int i = 0; i < temp.usedSize; i++) {
			fin.read((char*) &Estoque.vetDinamicProducts[i], sizeof Estoque.vetDinamicProducts[i]);
		}



	}
	else {
		Estoque.vetDinamicProducts = new Product[0];
	}

	do
	{
		char header[] = "Sistema de Controle";
		std::cout << header << std::endl;

		for (int i = 0; i < strlen(header); i++) {
			std::cout << '=';
		}

		std::cout << std::endl;

		std::cout << "(P)edir\n(A)dicionar\n(E)xcluir\n(L)istar\n(S)air";
		std::cout << "\n===================\n";
		
		std::cout << "Opção: [ ]\b\b";
		std::cin >> choose;

		choose = tolower(choose);
		
		system("cls");

		switch (choose) {
			case 'p' :
				order(Estoque);
				break;

			case 'a':
				addProduct(Estoque);
				break;

			case 'e':
				deleteProduct(Estoque);
				break;

			case 'l':
				listProducts(Estoque);
				break;

			default:
				break;
		}

		system("cls");

	} while (choose != 's');

	fout.open("estoque.bin", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

	if (fout.good()){

		//gravar primeiro o tamanho total depois o tamanho usado e os valores do vetor;
		fout.write((char*) &Estoque.totalSize, sizeof Estoque.totalSize);
		fout.write((char*) &Estoque.usedSize, sizeof Estoque.usedSize);

		for (int i = 0; i < Estoque.usedSize; i++) {
			fout.write((char*)&Estoque.vetDinamicProducts[i], sizeof Estoque.vetDinamicProducts[i]);
		}

	}
	else {
		std::cout << "Erro ao criar o arquivo de Estoque";
	}


	delete[] Estoque.vetDinamicProducts;
    return 0;
}

