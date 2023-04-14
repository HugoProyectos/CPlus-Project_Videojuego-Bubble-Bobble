#pragma once
#include "mapa.cpp"
#include "clasesConSprite/Pompa.hpp"
#include "clasesConSprite/Enemigo.cpp"
#include "clasesConSprite/Agua.hpp"
#include <vector>
#include "GeneradorPompas.hpp"
#include <iostream> //Para debuggear



class AdministradorPompas{
public:
	//Gesti�n de cambio de nivel
	bool cambiaNivel = false;
	uint32_t contadorFrames = 0;
	const uint32_t CUENTA_MAXIMA = 60 * 15;
	uint8_t enemigosPorMatar = 0;
	
	/*Rectangle posicionJugador = {-1,-1,-1,-1};
	bool jugadorCayendo = false;
	int sentidoJugador = 2; //3->mira a la derecha 2->mira a la izquierda: variable orientacionActual de Bub
	bool muriendo = false;
	int j1DebeRebotar = 0; //0->No
	int j1VelLateral = 0;*/
	DatosJugador j1, j2;
	Agua agua = Agua();
	GeneradorPompas generador = GeneradorPompas();

	std::vector<sh_Pompa> pompas;
	std::vector<sh_Enemigo> enemigos;
	
	AdministradorPompas() = default;

	//Funciones a llamar cuando se cambia de mapa
	void iniciaMapa(int numEnemigos) {
		cambiaNivel = false;
		contadorFrames = 0;
		enemigosPorMatar = numEnemigos;
		pompas.clear();
	}

	void CambioDeMapa(uint8_t idMapa) {
		generador.cambiarModo(idMapa);
	}
	////////////////////////////////////////
	

	//Secci�n pompas
	std::vector<sh_Pompa> elimina(int i) {
		std::vector<sh_Pompa> auxiliar;
		for (int j = 0; j < pompas.size(); j++) {
			if (j != i) {
				auxiliar.push_back(pompas.at(j));
			}
		}
		return auxiliar;
	}

	void actualizaPompas() {

		//Actualiza el estado de las pompas
		for (int i = 0; i < pompas.size(); i++) {
			if (pompas.at(i)->matame) {
				//auto aBorrar = pompas.begin() + i;
				//pompas.erase(aBorrar); //-->Necesita comparador entre pompas			
				pompas = elimina(i);
				i--;
			} else if (pompas.at(i)->disparada > 0) { //comprueba contacto con los enemigos
				//std::cout << "CHECK DISPARADA" << std::endl;

				for (int j = 0; j < enemigos.size(); j++) {
					if (!enemigos.at(j)->borrame && !enemigos.at(j)->muerto
					&& ((pompas.at(i)->destRec.y + pompas.at(i)->destRec.height / 2) >= (enemigos.at(j)->destRec.y + enemigos.at(j)->destRec.height / 2)
					&& (pompas.at(i)->destRec.y - pompas.at(i)->destRec.height / 2) <= (enemigos.at(j)->destRec.y + enemigos.at(j)->destRec.height / 2)
					|| (pompas.at(i)->destRec.y + pompas.at(i)->destRec.height / 2) >= (enemigos.at(j)->destRec.y - enemigos.at(j)->destRec.height / 2)
					&& (pompas.at(i)->destRec.y - pompas.at(i)->destRec.height / 2) <= (enemigos.at(j)->destRec.y - enemigos.at(j)->destRec.height / 2))
						&& ((pompas.at(i)->destRec.x + pompas.at(i)->destRec.width/2) >= (enemigos.at(j)->destRec.x - enemigos.at(j)->destRec.width/2)
					&& (pompas.at(i)->destRec.x - pompas.at(i)->destRec.width / 2) <= (enemigos.at(j)->destRec.x - enemigos.at(j)->destRec.width / 2)
					|| (pompas.at(i)->destRec.x + pompas.at(i)->destRec.width / 2) >= (enemigos.at(j)->destRec.x + enemigos.at(j)->destRec.width / 2)
					&& (pompas.at(i)->destRec.x - pompas.at(i)->destRec.width / 2) <= (enemigos.at(j)->destRec.x + enemigos.at(j)->destRec.width / 2))) { //Si choca con el enemigo, lo marca para que se borre y se cambia el estado de la pompa
						//std::cout << "Entro" << std::endl;
						enemigos.at(j)->borrame = true;
						//std::cout << "Salgo" << std::endl;
						pompas.at(i)->modulo = enemigos.at(j)->tipo;
						pompas.at(i)->indiceAnimacion = pompas.at(i)->modulo * Pompa::NUM_FOTOGRAMAS;
						pompas.at(i)->enemigoContenido = enemigos.at(j);
						pompas.at(i)->disparada = 0;
						break;
					}
				}
				uint8_t useless;
				sh_Enemigo enemigo = pompas.at(i)->Actualizar(j1, j2, useless);
				if (enemigo != NULL) {
					enemigos.push_back(enemigo);
					if (enemigo->muerto) {
						enemigosPorMatar--;
					}
				}
				if (pompas.at(i)->cadena) {
					for (int j = 0; j < pompas.size(); j++) {
						if (!pompas.at(j)->cadena && pompas.at(j)->animacionActiva != Pompa::EXPLOTA) {
							Rectangle ini = pompas.at(i)->destRec;
							Rectangle candidata = pompas.at(j)->destRec;
							//Choca por izquierda
							//			derecha
							//			abajo
							//			arriba
							if ((ini.x - ini.width / 2 + 2 < candidata.x + candidata.width / 2 - 2) && (ini.x + ini.width / 2 - 2 >= candidata.x + candidata.width / 2 - 2) && !((ini.y - ini.height / 2 + 2 > candidata.y + candidata.height / 2 - 2) || (ini.y + ini.height / 2 - 2 < candidata.y - candidata.height / 2 + 2))
								|| (ini.x - ini.width / 2 + 2 < candidata.x - candidata.width / 2 + 2) && (ini.x + ini.width / 2 - 2 >= candidata.x - candidata.width / 2 + 2) && !((ini.y - ini.height / 2 + 2 > candidata.y + candidata.height / 2 - 2) || (ini.y + ini.height / 2 - 2 < candidata.y - candidata.height / 2 + 2))
								|| (ini.y - ini.height / 2 + 2 < candidata.y + candidata.height / 2 - 2) && (ini.y + ini.height / 2 - 2 >= candidata.y + candidata.height / 2 - 2) && !((ini.x - ini.width / 2 + 2 > candidata.x + candidata.width / 2 - 2) || (ini.x + ini.width / 2 - 2 < candidata.x - candidata.width / 2 + 2))
								|| (ini.y - ini.height / 2 + 2 < candidata.y - candidata.height / 2 + 2) && (ini.y + ini.height / 2 - 2 >= candidata.y - candidata.height / 2 + 2) && !((ini.x - ini.width / 2 + 2 > candidata.x + candidata.width / 2 - 2) || (ini.x + ini.width / 2 - 2 < candidata.x - candidata.width / 2 + 2))) {
								pompas.at(j)->cadena = true;
							}
						}
					}
				}
			} else {
				uint8_t creaAgua = false;
				sh_Enemigo enemigo = pompas.at(i)->Actualizar(j1, j2, creaAgua);
				if (pompas.at(i)->animacionActiva != Pompa::EXPLOTA && i < pompas.size() - 1) {

					for (int j = i + 1; j < pompas.size(); j++) {
						Rectangle pompa1 = pompas.at(i)->destRec;
						Rectangle pompa2 = pompas.at(j)->destRec;
						if (pompas.at(j)->disparada == 0 && pompas.at(j)->animacionActiva != Pompa::EXPLOTA) {
							//Empuja la pompa2, que se mete por debajo (disminuye y) (se ha creado m�s tarde, por lo que est� en una etapa m�s tard�a de la ruta).
							//Choca por la derecha de la que empuja -> Empuja hacia la derecha
							if ((pompa1.x - pompa1.width / 3 < pompa2.x + pompa2.width / 3 ) && (pompa1.x + pompa1.width / 3  >= pompa2.x + pompa2.width / 3 ) && !((pompa1.y - pompa1.height / 3  > pompa2.y + pompa2.height / 3 ) || (pompa1.y + pompa1.height / 3  < pompa2.y - pompa2.height / 3 ))) {
								pompas.at(i)->destRec.x += pompa1.width / 8;
								pompas.at(i)->destRec.y -= pompa1.width / 8;
								pompas.at(j)->destRec.y += pompa1.width / 8;
								if(pompas.at(i)->tVida != Pompa::INFINITA) pompas.at(i)->tVida -= 5;
								if (pompas.at(j)->tVida != Pompa::INFINITA) pompas.at(j)->tVida -= 5;
							}
							//Choca por la izquierda de la que empuja -> Empuja hacia la izquierda
							else if ((pompa1.x - pompa1.width / 3 < pompa2.x - pompa2.width / 3) && (pompa1.x + pompa1.width / 3 >= pompa2.x - pompa2.width / 3) && !((pompa1.y - pompa1.height / 3 > pompa2.y + pompa2.height / 3) || (pompa1.y + pompa1.height / 3 < pompa2.y - pompa2.height / 3))) {
								pompas.at(i)->destRec.x -= pompa1.width / 8;
								pompas.at(i)->destRec.y -= pompa1.width / 8;
								pompas.at(j)->destRec.y += pompa1.width / 8;
								if (pompas.at(i)->tVida != Pompa::INFINITA) pompas.at(i)->tVida -= 5;
								if (pompas.at(j)->tVida != Pompa::INFINITA) pompas.at(j)->tVida -= 5;
							}
							if (pompas.at(i)->tVida < 0 && pompas.at(i)->tVida != Pompa::INFINITA) {
								pompas.at(i)->tVida = 0;
							}
							if (pompas.at(j)->tVida < 0 && pompas.at(j)->tVida != Pompa::INFINITA) {
								pompas.at(j)->tVida = 0;
							}
						}
					}
				}
				if (enemigo != NULL) {
					enemigos.push_back(enemigo); 
					if (enemigo->muerto) {
						enemigosPorMatar--;
					}
				}
				else if (creaAgua == 2 && !agua.existe) {
					agua.stream[0].destRec.x = pompas.at(i)->destRec.x;
					agua.stream[0].destRec.y = pompas.at(i)->destRec.y;
					agua.stream[0].direccionDerecha = false;
					agua.existe = true;
				}
				else if (creaAgua == 3 && !agua.existe) {
					agua.stream[0].destRec.x = pompas.at(i)->destRec.x;
					agua.stream[0].destRec.y = pompas.at(i)->destRec.y;
					agua.stream[0].direccionDerecha = true;
					agua.existe = true;
				}
				if (pompas.at(i)->cadena) {
					for (int j = 0; j < pompas.size(); j++) {
						//Si es candidata a continuar la cadena y est� en contacto con la pompa de la cadena
						if (!pompas.at(j)->cadena && pompas.at(j)->animacionActiva != Pompa::EXPLOTA ) {
							Rectangle ini = pompas.at(i)->destRec;
							Rectangle candidata = pompas.at(j)->destRec;
							//Choca por izquierda
							//			derecha
							//			abajo
							//			arriba
							if ((ini.x - ini.width / 2 + 2 < candidata.x + candidata.width / 2 - 2) && (ini.x + ini.width / 2 - 2 >= candidata.x + candidata.width / 2 - 2) && !((ini.y - ini.height / 2 + 2 > candidata.y + candidata.height / 2 - 2) || (ini.y + ini.height / 2 - 2 < candidata.y - candidata.height / 2 + 2))
								|| (ini.x - ini.width / 2 + 2 < candidata.x - candidata.width / 2 + 2) && (ini.x + ini.width / 2 - 2 >= candidata.x - candidata.width / 2 + 2) && !((ini.y - ini.height / 2 + 2 > candidata.y + candidata.height / 2 - 2) || (ini.y + ini.height / 2 - 2 < candidata.y - candidata.height / 2 + 2))
								|| (ini.y - ini.height / 2 + 2 < candidata.y + candidata.height / 2 - 2) && (ini.y + ini.height / 2 - 2 >= candidata.y + candidata.height / 2 - 2) && !((ini.x - ini.width / 2 + 2 > candidata.x + candidata.width / 2 - 2) || (ini.x + ini.width / 2 - 2 < candidata.x - candidata.width / 2 + 2))
								|| (ini.y - ini.height / 2 + 2 < candidata.y - candidata.height / 2 + 2) && (ini.y + ini.height / 2 - 2 >= candidata.y - candidata.height / 2 + 2) && !((ini.x - ini.width / 2 + 2 > candidata.x + candidata.width / 2 - 2) || (ini.x + ini.width / 2 - 2 < candidata.x - candidata.width / 2 + 2))) {
								pompas.at(j)->cadena = true;
							}
							
						}
					}
				}
			}
			
		}
		if (pompas.size() == 0) {
			//std::cout << "Sin pompas que actualizar" << std::endl;
		}


		//incrementamos el n�mero de iteraciones si numEnemigos = 0;
		if (enemigosPorMatar == 0 && !cambiaNivel) {
			if (contadorFrames == 0) {
				for (int i = 0; i < pompas.size(); i++) {
					pompas.at(i)->explota();
				}
			}
			contadorFrames++;
			if (contadorFrames >= CUENTA_MAXIMA) {
				std::cout << "FIN DE MAPA" << std::endl;
				cambiaNivel = true;
			}
		}
		if (enemigosPorMatar > 0) {
			//Genera pompas del mapa
			std::vector<sh_Pompa> pompasGeneradas = generador.generarPompa();
			for (int i = 0; i < pompasGeneradas.size(); i++) {
				//std::cout << "Creo pompa" << std::endl;
				pompas.push_back(pompasGeneradas.at(i));
			}
		}
	};

	void dibujaPompas() {
		for (int i = 0; i < pompas.size(); i++) {
			pompas[i]->Dibujar();
		}
	};

	//Secci�n enemigos
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
				enemigos.at(i)->Actualizar(j1.posicionJugador);
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