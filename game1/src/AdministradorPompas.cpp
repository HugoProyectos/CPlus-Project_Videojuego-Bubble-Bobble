#pragma once
#include "mapa.cpp"
#include "clasesConSprite/Pompa.hpp"
#include "clasesConSprite/Enemigo.cpp"
#include "clasesConSprite/Agua.hpp"
#include "clasesConSprite/Frutas.cpp"
#include <vector>
#include "GeneradorPompas.hpp"
#include <iostream> //Para debuggear
#include <clasesConSprite/Blanco.cpp>
#include <mapa.cpp>
#include "clasesConSprite/Rayo.hpp"





class AdministradorPompas {
public:
	Sound sonidoMuerteEnemigo = LoadSound("resources/music/sonido_muerte_enemigo.mp3");

	//Referencias a plataformas y columnas para la colision de pompas
	Columnas* col;
	Plataformas* plat;
	//Texturas para puntuacion de cadena
	Texture2D bubPoints = LoadTexture("resources/puntos/puntosBub.png");
	Rectangle pointSourceBub;
	Rectangle destRecBub;
	int pointsTimeBub = 0;
	Texture2D bobPoints = LoadTexture("resources/puntos/puntosBob.png");
	Rectangle pointSourceBob;
	Rectangle destRecBob;
	int pointsTimeBob = 0;
	int lastHeight;
	int lastWidth;
	Rectangle lastPopped;
	//Gesti�n de tiempo de mapa
	uint32_t contadorSkull = 0;
	uint32_t limiteContadorSkull = 60 * 90;
	bool hurryUp = false; //Se podr� usar para saber cuando mostrar el mensaje de hurry up
	Scores scores = Scores(0, 0, 20, SKYBLUE);
	//Gesti�n de cambio de nivel
	bool cambiaNivel = false;
	uint32_t contadorFrames = 0;
	const uint32_t CUENTA_MAXIMA_FIN_NIVEL = 60 * 10;
	int8_t enemigosPorMatar = 0;

	int killCountBub = 0;
	int lastKCBub = 0;
	int killCountBob = 0;
	int lastKCBob = 0;

	/*Rectangle posicionJugador = {-1,-1,-1,-1};
	bool jugadorCayendo = false;
	int sentidoJugador = 2; //3->mira a la derecha 2->mira a la izquierda: variable orientacionActual de Bub
	bool muriendo = false;
	int j1DebeRebotar = 0; //0->No
	int j1VelLateral = 0;*/
	Texture2D spriteRayo = LoadTexture("resources/rayo/rayo.png");
	DatosJugador j1, j2;
	Agua agua = Agua();
	GeneradorPompas generador = GeneradorPompas();

	std::vector<sh_Pompa> pompas;
	std::vector<sh_Enemigo> enemigos;
	std::vector<Rayo> rayos;
	
	// Frutas -------
	std::vector<sh_Frutas> frutas;
	//---------------

	AdministradorPompas() = default;

	//Funciones a llamar cuando se cambia de mapa
	void iniciaMapa(int numEnemigos, uint32_t cuentaAtras = 100) {
		cambiaNivel = false;
		contadorFrames = 0;
		enemigosPorMatar = numEnemigos;
		limiteContadorSkull = cuentaAtras;
		contadorSkull = 0;
		hurryUp = false;
		pompas.clear();
		enemigos.clear();
		frutas.clear();
	}

	void CambioDeMapa(uint8_t idMapa) {
		/*std::cout << "------------------------------------------CAMBIAMOS AL MAPA " << (int)idMapa << std::endl;
		std::cout << "------------------------------------------" << std::endl;
		std::cout << "------------------------------------------" << std::endl;
		std::cout << "------------------------------------------" << std::endl;
		std::cout << "------------------------------------------" << std::endl;
		std::cout << "------------------------------------------" << std::endl;
		std::cout << "------------------------------------------" << std::endl;
		std::cout << "------------------------------------------" << std::endl;
		std::cout << "------------------------------------------" << std::endl;
		std::cout << "------------------------------------------" << std::endl;*/
		generador.cambiarModo(idMapa);
	}
	////////////////////////////////////////

	//Secci�n rayos
	std::vector<Rayo> eliminaRayo(int i) {
		std::vector<Rayo> auxiliar;
		for (int j = 0; j < rayos.size(); j++) {
			if (j != i) {
				auxiliar.push_back(rayos.at(j));
			}
		}
		return auxiliar;
	}

	void actualizaRayos() {
		for (int i = 0; i < rayos.size(); i++) {
			//std::cout << "Dimensiones pompa; " << pompas.at(i)->destRec.x << "," << pompas.at(i)->destRec.y << "/" << pompas.at(i)->lastHeight << "," << pompas.at(i)->lastWidth << std::endl;

			if (rayos.at(i).borrame) {
				//auto aBorrar = pompas.begin() + i;
				//pompas.erase(aBorrar); //-->Necesita comparador entre pompas			
				rayos = eliminaRayo(i);
				i--;
			}
			else {
				rayos.at(i).Actualizar(j1,j2);
				for (int j = 0; j < enemigos.size(); j++) {
					if (!enemigos.at(j)->borrame && !enemigos.at(j)->muerto && (enemigos.at(j)->tipo != -2)
						&& ((rayos.at(i).destRec.y + rayos.at(i).destRec.height / 2) >= (enemigos.at(j)->destRec.y + enemigos.at(j)->destRec.height / 2)
							&& (rayos.at(i).destRec.y - rayos.at(i).destRec.height / 2) <= (enemigos.at(j)->destRec.y + enemigos.at(j)->destRec.height / 2)
							|| (rayos.at(i).destRec.y + rayos.at(i).destRec.height / 2) >= (enemigos.at(j)->destRec.y - enemigos.at(j)->destRec.height / 2)
							&& (rayos.at(i).destRec.y - rayos.at(i).destRec.height / 2) <= (enemigos.at(j)->destRec.y - enemigos.at(j)->destRec.height / 2))
						&& ((rayos.at(i).destRec.x + rayos.at(i).destRec.width / 2 - rayos.at(i).destRec.width * 0.125) >= (enemigos.at(j)->destRec.x - enemigos.at(j)->destRec.width / 2)
							&& (rayos.at(i).destRec.x - rayos.at(i).destRec.width / 2 + rayos.at(i).destRec.width * 0.125) <= (enemigos.at(j)->destRec.x - enemigos.at(j)->destRec.width / 2)
							|| (rayos.at(i).destRec.x + rayos.at(i).destRec.width / 2 - rayos.at(i).destRec.width * 0.125) >= (enemigos.at(j)->destRec.x + enemigos.at(j)->destRec.width / 2)
							&& (rayos.at(i).destRec.x - rayos.at(i).destRec.width / 2 + rayos.at(i).destRec.width * 0.125) <= (enemigos.at(j)->destRec.x + enemigos.at(j)->destRec.width / 2))) { //Si choca con el enemigo, lo marca para que se borre y se cambia el estado de la pompa
						enemigos.at(j)->muertePorRayo = true;
						enemigos.at(j)->muerto = true;
						rayos.at(i).animacionActiva = 1;
						rayos.at(i).indiceAnimacion = 0;
						enemigosPorMatar--;
						break;
					}
				}
			}
		}
	}

	void DibujaRayos() {
		for (int i = 0; i < rayos.size(); i++) {
			rayos[i].Dibujar();
		}
	}

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

	void dibujarPuntosCadena() {
		if (pointsTimeBub > 0) {
			pointsTimeBub--;
			if (lastWidth != GetScreenWidth()) {
				destRecBub.x = (destRecBub.x / lastWidth) * GetScreenWidth();
				destRecBub.width = (destRecBub.width / lastWidth) * GetScreenWidth();
			}
			if (lastWidth != GetScreenWidth()) {
				destRecBub.y = (destRecBub.y / lastHeight) * GetScreenHeight();
				destRecBub.height = (destRecBub.height / lastHeight) * GetScreenHeight();
			}
			destRecBub.y -= (float)GetScreenHeight() / 300;
			if (destRecBub.y < -50) {
				destRecBub.y = GetScreenHeight() + 50;
			}
			DrawTexturePro(bubPoints, pointSourceBub, destRecBub, { destRecBub.width / 2, destRecBub.height / 2 }, 0.0f, WHITE);
		}
		if (pointsTimeBob > 0) {
			pointsTimeBob--;
			if (lastWidth != GetScreenWidth()) {
				destRecBob.x = (destRecBob.x / lastWidth) * GetScreenWidth();
				destRecBob.width = (destRecBob.width / lastWidth) * GetScreenWidth();
			}
			if (lastWidth != GetScreenWidth()) {
				destRecBob.y = (destRecBob.y / lastHeight) * GetScreenHeight();
				destRecBob.height = (destRecBob.height / lastHeight) * GetScreenHeight();
			}
			destRecBob.y -= (float)GetScreenHeight() / 300;
			if (destRecBob.y < -50) {
				destRecBob.y = GetScreenHeight() + 50;
			}
			DrawTexturePro(bobPoints, pointSourceBob, destRecBob, { destRecBob.width / 2, destRecBob.height / 2 }, 0.0f, WHITE);
		}
		lastWidth = GetScreenWidth();
		lastHeight = GetScreenHeight();
	}

	void actualizaPompas() {
		//std::cout << "Pompas a actualizar: " << pompas.size() << std::endl;
		//Actualiza el estado de las pompas
		for (int i = 0; i < pompas.size(); i++) {
			//std::cout << "Dimensiones pompa; " << pompas.at(i)->destRec.x << "," << pompas.at(i)->destRec.y << "/" << pompas.at(i)->lastHeight << "," << pompas.at(i)->lastWidth << std::endl;

			if (pompas.at(i)->matame) {
				//auto aBorrar = pompas.begin() + i;
				//pompas.erase(aBorrar); //-->Necesita comparador entre pompas			
				if (pompas.at(i)->modulo = Pompa::MODULO_RAYO) {
					int val = pompas.at(i)->sentidoJugador;
					bool izq = val == 3;
					Rayo r = Rayo(spriteRayo, pompas.at(i)->destRec, izq);
					//std::cout << "Direccion del rayo: " << r.izquierda << "Deberia ser: " << izq << std::endl;
					r.izquierda = izq;
					rayos.push_back(r);
				}
				pompas = elimina(i);
				i--;
			}
			else if (pompas.at(i)->disparada > 0) { //comprueba contacto con los enemigos
				//std::cout << "CHECK DISPARADA" << std::endl;

				for (int j = 0; j < enemigos.size(); j++) {
					if (!enemigos.at(j)->borrame && !enemigos.at(j)->muerto && (enemigos.at(j)->tipo != -2)
						&& ((pompas.at(i)->destRec.y + pompas.at(i)->destRec.height / 2) >= (enemigos.at(j)->destRec.y + enemigos.at(j)->destRec.height / 2)
							&& (pompas.at(i)->destRec.y - pompas.at(i)->destRec.height / 2) <= (enemigos.at(j)->destRec.y + enemigos.at(j)->destRec.height / 2)
							|| (pompas.at(i)->destRec.y + pompas.at(i)->destRec.height / 2) >= (enemigos.at(j)->destRec.y - enemigos.at(j)->destRec.height / 2)
							&& (pompas.at(i)->destRec.y - pompas.at(i)->destRec.height / 2) <= (enemigos.at(j)->destRec.y - enemigos.at(j)->destRec.height / 2))
						&& ((pompas.at(i)->destRec.x + pompas.at(i)->destRec.width / 2) >= (enemigos.at(j)->destRec.x - enemigos.at(j)->destRec.width / 2)
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
				sh_Enemigo enemigo = pompas.at(i)->Actualizar(j1, j2, useless, col, plat, &scores);
				if (enemigo != NULL) {
					enemigos.push_back(enemigo);
					if (enemigo->muerto) {
						PlaySound(sonidoMuerteEnemigo);
						enemigosPorMatar--;
						std::cout << "Quedan " << enemigosPorMatar << " enemigos por matar." << std::endl;
					}
				}
				if (pompas.at(i)->cadena) {
					if (pompas.at(i)->cadena == 1) {
						if (pompas.at(i)->enemigoContenido != NULL) {
							killCountBub++;
							lastPopped = pompas.at(i)->destRec;
							pompas.at(i)->killCount = killCountBub;
							switch (killCountBub) {
							case 1:
								scores.puntuacion1 += 1000;
								break;
							case 2:
								scores.puntuacion1 += 2000;
								break;
							case 3:
								scores.puntuacion1 += 4000;
								break;
							case 4:
								scores.puntuacion1 += 8000;
								break;
							case 5:
								scores.puntuacion1 += 16000;
								break;
							case 6:
								scores.puntuacion1 += 32000;
								break;
							default:
								scores.puntuacion1 += 64000;
							}
						}
						else {
							scores.puntuacion1 += 10;
						}
					}
					else if (pompas.at(i)->cadena == 2) {
						if (pompas.at(i)->enemigoContenido != NULL) {
							killCountBob++;
							lastPopped = pompas.at(i)->destRec;
							pompas.at(i)->killCount = killCountBob;
							switch (killCountBob) {
							case 1:
								scores.puntuacion2 += 1000;
								break;
							case 2:
								scores.puntuacion2 += 2000;
								break;
							case 3:
								scores.puntuacion2 += 4000;
								break;
							case 4:
								scores.puntuacion2 += 8000;
								break;
							case 5:
								scores.puntuacion2 += 16000;
								break;
							case 6:
								scores.puntuacion2 += 32000;
								break;
							default:
								scores.puntuacion1 += 64000;
							};
						}
						else {
							scores.puntuacion2 += 10;
						}
					}
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
								pompas.at(j)->cadena = pompas.at(i)->cadena;
							}
						}
					}
				}
			}
			else {
				uint8_t creaAgua = false;
				sh_Enemigo enemigo = pompas.at(i)->Actualizar(j1, j2, creaAgua, col, plat, &scores);
				if (pompas.at(i)->animacionActiva != Pompa::EXPLOTA && i < pompas.size() - 1) {

					for (int j = i + 1; j < pompas.size(); j++) {
						Rectangle pompa1 = pompas.at(i)->destRec;
						Rectangle pompa2 = pompas.at(j)->destRec;
						if (pompas.at(j)->disparada == 0 && pompas.at(j)->animacionActiva != Pompa::EXPLOTA) {
							//Empuja la pompa2, que se mete por debajo (disminuye y) (se ha creado m�s tarde, por lo que est� en una etapa m�s tard�a de la ruta).
							//Choca por la derecha de la que empuja -> Empuja hacia la derecha
							if ((pompa1.x - pompa1.width / 3 < pompa2.x + pompa2.width / 3 ) && (pompa1.x + pompa1.width / 3  >= pompa2.x + pompa2.width / 3 ) && !((pompa1.y - pompa1.height / 3  > pompa2.y + pompa2.height / 3 ) || (pompa1.y + pompa1.height / 3  < pompa2.y - pompa2.height / 3 ))) {
								pompas.at(i)->destRec.x += pompa1.width / 16;
								pompas.at(i)->destRec.y -= pompa1.width / 16;
								pompas.at(j)->destRec.y += pompa1.width / 16;
								if(pompas.at(i)->tVida != Pompa::INFINITA) pompas.at(i)->tVida -= 5;
								if (pompas.at(j)->tVida != Pompa::INFINITA) pompas.at(j)->tVida -= 5;
							}
							//Choca por la izquierda de la que empuja -> Empuja hacia la izquierda
							else if ((pompa1.x - pompa1.width / 3 < pompa2.x - pompa2.width / 3) && (pompa1.x + pompa1.width / 3 >= pompa2.x - pompa2.width / 3) && !((pompa1.y - pompa1.height / 3 > pompa2.y + pompa2.height / 3) || (pompa1.y + pompa1.height / 3 < pompa2.y - pompa2.height / 3))) {
								pompas.at(i)->destRec.x -= pompa1.width / 16;
								pompas.at(i)->destRec.y -= pompa1.width / 16;
								pompas.at(j)->destRec.y += pompa1.width / 16;
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
						PlaySound(sonidoMuerteEnemigo);
						enemigosPorMatar--;
						std::cout << "Quedan " << (int)enemigosPorMatar << " enemigos por matar." << std::endl;
					}
				}
				else if (creaAgua == 2 && !agua.existe) {
					agua.stream[0].destRec.x = pompas.at(i)->destRec.x;
					agua.stream[0].destRec.y = pompas.at(i)->destRec.y;
					agua.stream[0].destRec.width = GetScreenWidth() / 61.5f;
					agua.stream[0].destRec.height = GetScreenHeight() / 34.6f;
					agua.lastHeight = GetScreenHeight();
					agua.lastWidth = GetScreenWidth();
					agua.stream[0].direccionDerecha = false;
					agua.existe = true;
				}
				else if (creaAgua == 3 && !agua.existe) {
					agua.stream[0].destRec.x = pompas.at(i)->destRec.x;
					agua.stream[0].destRec.y = pompas.at(i)->destRec.y;
					agua.stream[0].destRec.width = GetScreenWidth() / 61.5f;
					agua.stream[0].destRec.height = GetScreenHeight() / 34.6f;
					agua.lastHeight = GetScreenHeight();
					agua.lastWidth = GetScreenWidth();
					agua.stream[0].direccionDerecha = true;
					agua.existe = true;
				}
				if (pompas.at(i)->cadena) {
					if (pompas.at(i)->cadena == 1) {
						if (pompas.at(i)->enemigoContenido != NULL) {
							killCountBub++;
							lastPopped = pompas.at(i)->destRec;
							pompas.at(i)->killCount = killCountBub;
							pompas.at(i)->enemigoContenido->killCount = killCountBub-1;
							//std::cout << "-------------------------------     " + std::to_string(pompas.at(i)->killCount) << std::endl;
							//while (true) {}
							switch (killCountBub) {
							case 1:
								scores.puntuacion1 += 1000;
								break;
							case 2:
								scores.puntuacion1 += 2000;
								break;
							case 3:
								scores.puntuacion1 += 4000;
								break;
							case 4:
								scores.puntuacion1 += 8000;
								break;
							case 5:
								scores.puntuacion1 += 16000;
								break;
							case 6:
								scores.puntuacion1 += 32000;
								break;
							default:
								scores.puntuacion1 += 64000;
							};
						}
						else {
							scores.puntuacion1 += 10;
						}
					}
					else if (pompas.at(i)->cadena == 2) {
						if (pompas.at(i)->enemigoContenido != NULL) {
							killCountBob++;
							lastPopped = pompas.at(i)->destRec;
							pompas.at(i)->killCount = killCountBob;
							switch (killCountBob) {
							case 1:
								scores.puntuacion2 += 1000;
								break;
							case 2:
								scores.puntuacion2 += 2000;
								break;
							case 3:
								scores.puntuacion2 += 4000;
								break;
							case 4:
								scores.puntuacion2 += 8000;
								break;
							case 5:
								scores.puntuacion2 += 16000;
								break;
							case 6:
								scores.puntuacion2 += 32000;
								break;
							default:
								scores.puntuacion1 += 64000;
							};
						}
						else {
							scores.puntuacion2 += 10;
						}
					}
					for (int j = 0; j < pompas.size(); j++) {
						//Si es candidata a continuar la cadena y est� en contacto con la pompa de la cadena
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
								pompas.at(j)->cadena = pompas.at(i)->cadena;
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
				//Para borrar a SKULL
				for (int i = 0; i < enemigos.size(); i++) {
					enemigos.at(i)->muerto = true;
					if (enemigos.at(i)->tipo == -2) {
						enemigos = eliminaEnemigo(i); //Esto borra todos los enemigos (raz�n por la que el �ltimo no genera fruta?)
					}
				}
			}
			contadorFrames++;
			if (contadorFrames >= CUENTA_MAXIMA_FIN_NIVEL) {
				std::cout << "FIN DE MAPA" << std::endl;
				cambiaNivel = true;
			}
		}
		else if (enemigosPorMatar < 0) {
			enemigosPorMatar = 0; //Esto es porque por alguna raz�n algun enemigo se resta dos veces
		}
		if (enemigosPorMatar > 0) {
			//Contador de aparici�n de SKULL (Monsta blanco)
			if (contadorSkull < limiteContadorSkull) {
				contadorSkull++;
			}
			else if (j1.muriendo || j2.muriendo) {
				contadorSkull = 0;
				hurryUp = false;
			}
			else {
				if (contadorSkull == limiteContadorSkull) {
					hurryUp = true;
					//Crear a Skull y a�adirlo a enemigos.
					Rectangle destRob = { GetScreenWidth() / 2, 70, 32, 32 };
					Blanco skull;
					skull = Blanco("resources/enemyFantasma/fantasmaBasic.png", 2.0f, 40.0f, 1.0f, 2.0f, 60.0, destRob, j1, j2);
					enemigos.push_back(std::make_shared<Blanco>(skull));
					contadorSkull++;
					for (int j = 0; j < enemigos.size(); j++) {
						enemigos[j]->enfadado = true;
					}
				}
			}
			//Genera pompas del mapa
			std::vector<sh_Pompa> pompasGeneradas = generador.generarPompa();
			for (int i = 0; i < pompasGeneradas.size(); i++) {
				//std::cout << "Creo pompa" << std::endl;
				pompas.push_back(pompasGeneradas.at(i));
			}
		}
		if (lastKCBub == killCountBub) {
			if (killCountBub > 1) {
				switch (killCountBub) {
				case 2:
					pointSourceBub = { 254.0f,10.0f,31.0f,15.0f };
					break;
				case 3:
					pointSourceBub = { 289.0f,10.f,31.0f,15.0f };
					break;
				case 4:
					pointSourceBub = { 324.0f,10.0f,31.0f,15.0f };
					break;
				case 5:
					pointSourceBub = { 359.0f,10.0f,47.0f,15.0f };
					break;
				case 6:
					pointSourceBub = { 410.0f,10.0f,47.0f,15.0f };
					break;
				case 7:
					pointSourceBub = { 461.0f,10.0f,47.0f,15.0f };
					break;
				}
				pointsTimeBub = 300;
				lastHeight = GetScreenHeight();
				lastWidth = GetScreenWidth();
				destRecBub = { lastPopped.x, lastPopped.y,  ((float)GetScreenWidth() / 200) * pointSourceBub.width, ((float)GetScreenHeight() / 200) * pointSourceBub.height };
			}
			if (killCountBub != 0) {
				killCountBub = 0;
			}
		}
		else {
			lastKCBub = killCountBub;
		}
		if (lastKCBob == killCountBob) {
			if (killCountBob > 1) {
				switch (killCountBob) {
				case 2:
					pointSourceBob = { 254.0f,10.0f,31.0f,15.0f };
					break;
				case 3:
					pointSourceBob = { 289.0f,10.f,31.0f,15.0f };
					break;
				case 4:
					pointSourceBob = { 324.0f,10.0f,31.0f,15.0f };
					break;
				case 5:
					pointSourceBob = { 359.0f,10.0f,47.0f,15.0f };
					break;
				case 6:
					pointSourceBob = { 410.0f,10.0f,47.0f,15.0f };
					break;
				case 7:
					pointSourceBob = { 461.0f,10.0f,47.0f,15.0f };
					break;
				}
				pointsTimeBob = 300;
				lastHeight = GetScreenHeight();
				lastWidth = GetScreenWidth();
				destRecBob = { lastPopped.x, lastPopped.y,  ((float)GetScreenWidth() / 200) * pointSourceBob.width, ((float)GetScreenHeight() / 200) * pointSourceBob.height };
			}
			if (killCountBob != 0) {
				killCountBob = 0;
			}
		}
		else {
			lastKCBob = killCountBob;
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

	std::vector<sh_Frutas> eliminaFruta(int i) {
		std::vector<sh_Frutas> aux;
		for (int j = 0; j < frutas.size(); j++) {
			if (j != i) {
				aux.push_back(frutas.at(j));
			}
		}
		return aux;
	}

	void actualizaEnemigos(Plataformas& plataformas, Columnas& columnas) {
		for (int i = 0; i < enemigos.size(); i++) {
			if (enemigos.at(i)->borrame) {
				//auto aBorrar = pompas.begin() + i;
				//pompas.erase(aBorrar); //-->Necesita comparador entre pompas
				if (enemigos.at(i)->muertePorAgua) {
					enemigosPorMatar--;
					std::cout << "Quedan " << (int)enemigosPorMatar << " enemigos por matar." << std::endl;
				}
				enemigos = eliminaEnemigo(i);
				i--;
			}
			else {
				enemigos.at(i)->Actualizar(j1.posicionJugador);
				for (int j = 0; j < plataformas.listaPlataforma.size(); j++) {
					enemigos.at(i)->compruebaSuelo();
					enemigos.at(i)->compruebaColision(plataformas.listaPlataforma[j], i);
					enemigos.at(i)->compruebaPared(columnas);
				}
			}
		}
		if (enemigos.size() == 0) {
			//std::cout << "Sin enemigos que actualizar" << std::endl;
		}
	};

	void actualizaFrutas(Plataformas& plataformas) {
		for (int i = 0; i < (frutas.size()); i++) {
			if (frutas.at(i)->borrame) {
				frutas = eliminaFruta(i);
				i--;
			}
			else {
				frutas.at(i)->Actualizar();
				for (int j = 0; j < plataformas.listaPlataforma.size(); j++) {
					frutas.at(i)->compruebaSuelo();
					frutas.at(i)->compruebaColision(plataformas.listaPlataforma[j], (i + enemigos.size()));
				}
			}
		}
	}

	void dibujaEnemigos() {
		for (int i = 0; i < enemigos.size(); i++) {
			enemigos[i]->Dibujar();
		}
	};

	void dibujaFrutas() {
		for (int i = 0; i < frutas.size(); i++) {
			frutas[i]->Dibujar();
		}
	}
};