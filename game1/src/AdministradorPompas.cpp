#pragma once
#include "clasesConSprite/Pompa.hpp"
#include "clasesConSprite/Enemigo.cpp"
#include <vector>
#include <iostream> //Para debuggear

typedef std::shared_ptr<Enemigo> sh_Enemigo;

class AdministradorPompas{
public:
	Rectangle posicionJugador = { -1,-1,-1,-1 };
	bool jugadorCayendo = false;
	int sentidoJugador = 2; //3->mira a la derecha 2->mira a la izquierda: variable orientacionActual de Bub

	std::vector<Pompa> pompas;
	std::vector<sh_Enemigo> enemigos;
	
	AdministradorPompas() = default;

	//Sección pompas
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
				pompas.at(i).Actualizar(posicionJugador, jugadorCayendo, sentidoJugador);
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

	//Sección enemigos
	std::vector<sh_Enemigo> eliminaEnemigo(int i) {
		std::vector<sh_Enemigo> auxiliar;
		for (int j = 0; j < enemigos.size(); j++) {
			if (j != i) {
				auxiliar.push_back(enemigos.at(j));
			}
		}
		return auxiliar;
	}

	void actualizaEnemigos() {
		for (int i = 0; i < enemigos.size(); i++) {
			if (enemigos.at(i)->borrame) {
				//auto aBorrar = pompas.begin() + i;
				//pompas.erase(aBorrar); //-->Necesita comparador entre pompas
				enemigos = eliminaEnemigo(i);
				i--;
			}
			else {
				enemigos.at(i)->Actualizar(posicionJugador);
			}
		}
		if (enemigos.size() == 0) {
			//std::cout << "Sin pompas que actualizar" << std::endl;
		}
	};

	void dibujaEnemigos() {
		for (int i = 0; i < enemigos.size(); i++) {
			enemigos[i]->Dibujar();
		}
	};
};