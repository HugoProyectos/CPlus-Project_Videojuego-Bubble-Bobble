#pragma once
#include "clasesConSprite/Pompa.cpp"
#include <vector>
#include <iostream> //Para debuggear

class AdministradorPompas{
public:
	std::vector<Pompa> pompas;
	Rectangle posicionJugador;

	AdministradorPompas() = default;

	std::vector<Pompa> elimina(int i) {
		std::vector<Pompa> auxiliar;
		for (int j = 0; j < pompas.size(); j++) {
			if (j != i) {
				auxiliar.push_back(pompas.at(j));
			}
		}
		return auxiliar;
	}

	void actualizaPompas() {
		for (int i = 0; i < pompas.size(); i++) {
			if (pompas.at(i).matame) {
				//auto aBorrar = pompas.begin() + i;
				//pompas.erase(aBorrar); //-->Necesita comparador entre pompas
				pompas = elimina(i);
				i--;
			} else {
				pompas.at(i).Actualizar();
			}
		}
		if (pompas.size() == 0) {
			//std::cout << "Sin pompas que actualizar" << std::endl;
		}
	};

	void dibujaPompas() {
		for (int i = 0; i < pompas.size(); i++) {
			pompas[i].Dibujar();
		}
	};
};