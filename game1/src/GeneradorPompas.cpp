#include "generadorPompas.hpp"
#include <stdlib.h>
#include <time.h>

GeneradorPompas::GeneradorPompas(uint8_t idMapa) {
	this->idMapa = idMapa;
	srand(time(NULL));
}

void GeneradorPompas::cambiarModo(uint8_t idMapa) {
	this->idMapa = idMapa;
	Pompa::ID_MAPA = idMapa;
	contadorCiclos = 0;
}

std::vector<sh_Pompa> GeneradorPompas::generarPompa() {
	std::vector<sh_Pompa> resultado;
	//std::vector<Pompa> pompas;
	contadorCiclos++;
	if (contadorCiclos >= ciclosPompa[idMapa]) {
		//std::cout << "Chekeo de generación de pompas" << std::endl;
		contadorCiclos = 0;
		switch (idMapa) {
		case 4: //Mapa del agua
		{
			Pompa p;
			Rectangle r[2] = { { GetScreenWidth() / 2 - 150, GetScreenHeight() + 32, 32, 32}, { GetScreenWidth() / 2 + 150, GetScreenHeight() + 32, 32, 32 } };
			//resultado.push_back(std::make_shared<Pompa>(p1));
			//resultado.push_back(std::make_shared<Pompa>(p2));
			int numRand = rand() % 100;
			for (int i = 0; i < 2; i++) {
				//Selección del color de la pompa
				if (numRand < 50) { //verde
					p = Pompa(verde, r[i], 0, 0, false, 1000000);
				}
				else { //azul
					p = Pompa(azul, r[i], 0, 0, false, 1000000);
				}
				numRand = rand() % 100;
				if (numRand < 50) { //Que contenga agua
					p.modulo = Pompa::MODULO_AGUA;
				}
				resultado.push_back(std::make_shared<Pompa>(p));
			}
			break;
		}
		default:
			break;
		}
	}
	return resultado;
}