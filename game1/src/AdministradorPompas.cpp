#pragma once
#include "mapa.cpp"
#include "clasesConSprite/Pompa.hpp"
#include "clasesConSprite/Enemigo.cpp"
#include <vector>
#include <iostream> //Para debuggear



class AdministradorPompas{
public:
	Rectangle posicionJugador = { -1,-1,-1,-1 };
	bool jugadorCayendo = false;
	int sentidoJugador = 2; //3->mira a la derecha 2->mira a la izquierda: variable orientacionActual de Bub
	bool muriendo = false;

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
			} else if (pompas.at(i).disparada > 0) { //comprueba contacto con los enemigos
				//std::cout << "CHECK DISPARADA" << std::endl;

				for (int j = 0; j < enemigos.size(); j++) {
					if (!enemigos.at(j)->borrame && !enemigos.at(j)->muerto
					&& ((pompas.at(i).destRec.y + pompas.at(i).destRec.height / 2) >= (enemigos.at(j)->destRec.y + enemigos.at(j)->destRec.height / 2)
					&& (pompas.at(i).destRec.y - pompas.at(i).destRec.height / 2) <= (enemigos.at(j)->destRec.y + enemigos.at(j)->destRec.height / 2)
					|| (pompas.at(i).destRec.y + pompas.at(i).destRec.height / 2) >= (enemigos.at(j)->destRec.y - enemigos.at(j)->destRec.height / 2)
					&& (pompas.at(i).destRec.y - pompas.at(i).destRec.height / 2) <= (enemigos.at(j)->destRec.y - enemigos.at(j)->destRec.height / 2))
						&& ((pompas.at(i).destRec.x + pompas.at(i).destRec.width/2) >= (enemigos.at(j)->destRec.x - enemigos.at(j)->destRec.width/2)
					&& (pompas.at(i).destRec.x - pompas.at(i).destRec.width / 2) <= (enemigos.at(j)->destRec.x - enemigos.at(j)->destRec.width / 2)
					|| (pompas.at(i).destRec.x + pompas.at(i).destRec.width / 2) >= (enemigos.at(j)->destRec.x + enemigos.at(j)->destRec.width / 2)
					&& (pompas.at(i).destRec.x - pompas.at(i).destRec.width / 2) <= (enemigos.at(j)->destRec.x + enemigos.at(j)->destRec.width / 2))) { //Si choca con el enemigo, lo marca para que se borre y se cambia el estado de la pompa
						//std::cout << "Entro" << std::endl;
						enemigos.at(j)->borrame = true;
						//std::cout << "Salgo" << std::endl;
						pompas.at(i).modulo = enemigos.at(j)->tipo;
						pompas.at(i).enemigoContenido = enemigos.at(j);
						pompas.at(i).disparada = 0;
						break;
					}
				}
				sh_Enemigo enemigo = pompas.at(i).Actualizar(posicionJugador, jugadorCayendo, sentidoJugador, muriendo);
				if (enemigo != NULL) {
					enemigos.push_back(enemigo);
				}
			} else {
				sh_Enemigo enemigo = pompas.at(i).Actualizar(posicionJugador, jugadorCayendo, sentidoJugador, muriendo);
				if (enemigo != NULL) {
					enemigos.push_back(enemigo);
				}
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

	void actualizaEnemigos(Plataformas& plataformas) {
		for (int i = 0; i < enemigos.size(); i++) {
			if (enemigos.at(i)->borrame) {
				//auto aBorrar = pompas.begin() + i;
				//pompas.erase(aBorrar); //-->Necesita comparador entre pompas
				enemigos = eliminaEnemigo(i);
				i--;
			}
			else {
				enemigos.at(i)->Actualizar(posicionJugador);
				for (int j = 0; j < plataformas.listaPlataforma.size(); j++) {
					enemigos.at(i)->compruebaSuelo();
					enemigos.at(i)->compruebaColision(plataformas.listaPlataforma[j], i);
				}
			}
		}
		if (enemigos.size() == 0) {
			//std::cout << "Sin enemigos que actualizar" << std::endl;
		}
	};

	void dibujaEnemigos() {
		for (int i = 0; i < enemigos.size(); i++) {
			enemigos[i]->Dibujar();
		}
	};
};