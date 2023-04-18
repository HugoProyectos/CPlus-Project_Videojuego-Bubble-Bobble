#pragma once
#include "Pompa.hpp"


uint32_t Pompa::ID_MAPA = 0;
uint32_t Pompa::ID_GLOBAL = 0;

sh_Enemigo Pompa::extraeEnemigo(bool matalo) {
	sh_Enemigo resultado = NULL;
	if (modulo == 1) {
		enemigoContenido->destRec = destRec;
		enemigoContenido->borrame = false;
		enemigoContenido->enElAire = true;
		enemigoContenido->cayendo = true;
		enemigoContenido->muerto = matalo;
		enemigoContenido->enfadado = true;

		resultado = enemigoContenido;
	} else if (modulo == 3) {
		enemigoContenido->destRec = destRec;
		enemigoContenido->borrame = false;
		enemigoContenido->enElAire = true;
		enemigoContenido->cayendo = true;
		enemigoContenido->muerto = matalo;
		enemigoContenido->enfadado = true;

		resultado = enemigoContenido;
	} else if (modulo == 4) {
		enemigoContenido->destRec = destRec;
		enemigoContenido->borrame = false;
		enemigoContenido->enElAire = true;
		enemigoContenido->cayendo = true;
		enemigoContenido->muerto = matalo;
		enemigoContenido->enfadado = true;

		resultado = enemigoContenido;
	} else if (modulo == 5) {
		enemigoContenido->destRec = destRec;
		enemigoContenido->borrame = false;
		enemigoContenido->enElAire = true;
		enemigoContenido->cayendo = true;
		enemigoContenido->muerto = matalo;
		enemigoContenido->enfadado = true;

		resultado = enemigoContenido;
	}
	return resultado;
}

void Pompa::init() { //Solo se llama una vez, para reiniciar los identificadores de pompas
	Pompa::ID_GLOBAL = 0u;
}

Pompa::Pompa(Texture2D spriteSheet, const Rectangle origen, float velDisparo, float distanciaDisparo, bool esDisparada, int tiempoVida) {
	Inicializador(spriteSheet, origen, velDisparo, distanciaDisparo, esDisparada, tiempoVida);
}

bool Pompa::operator==(const Pompa& p) {
	return ID == p.ID;
}

void Pompa::Inicializador(Texture2D spriteSheet, const Rectangle origen, float velDisparo, float distanciaDisparo, bool esDisparada, int tiempoVida) {
	ID = ID_GLOBAL++; //Asigna identificador único (De no ser todo secuencial, habrá que gestionar sección crítica)

	sprite = spriteSheet;

	destRec = origen;//{ origen.x, origen.y, (float)widthAnimation * 2, (float)heightAnimation * 2 };
	tVida = tiempoVida;
	vidaMaxima = tiempoVida;
	velocidadFrames = 8;
	if (esDisparada) {
		//std::cout << "Disparada" << std::endl;
		disparada = 2;
		this->distanciaDisparo = distanciaDisparo;
		animacionActiva = DISPARO;
	}
	else {
		//std::cout << "NO Disparada" << std::endl;
		disparada = 0;
		animacionActiva = VACIA;
	}
	velocidadDesplazamiento = velDisparo;
	tamano = 1;

	destRec = { origen.x, origen.y, (float)widthAnimation * 2, (float)heightAnimation * 2 };
	srcRec = { 0.0f, 0.0f, (float)heightAnimation, (float)widthAnimation };
	origin = { origen.width / 2, origen.height / 2 }; //En principio no lo necesitamos

}

sh_Enemigo Pompa::Actualizar(DatosJugador& j1, DatosJugador& j2, uint8_t& creaAgua) {
	sh_Enemigo result = NULL;

	if (disparada == 2) {
		//std::cout << "PIUM" << std::endl;
		distanciaRecorrida += velocidadDesplazamiento; //Actualiza el contador
		//std::cout << distanciaRecorrida << std::endl;
		//std::cout << distanciaDisparo << std::endl;
		destRec.x += velocidadDesplazamiento;
		if (abs(distanciaRecorrida) >= abs(distanciaDisparo)) {
			disparada = 1; //Último frame en que atrapa enemigos
		}
	}
	else {
		//Si ha iniciado cadena y ya está explotando, es solo en el primer frame que transfiere cadena
		if (cadena && animacionActiva == EXPLOTA) {
			cadena = false;
		} else if (cadena) { // Si aún no ha explotado, inicia explosión
			animacionActiva = EXPLOTA;
			indiceAnimacion = 0;
			tVida = -1;
			//std::cout << "Peto por cadena" << std::endl;

			if (modulo == MODULO_AGUA) {
				creaAgua = j1.sentidoJugador;
			}
			else {
				result = extraeEnemigo(true);
			}
		}

		//Con las interacciones debe hacerlo solo si no tiene enemigo dentro
		else if (tVida == 0) {
			animacionActiva = EXPLOTA;
			indiceAnimacion = 0;
			tVida--;
			
			result = extraeEnemigo(false);
			
		}
		else if (tVida > 0 || tVida == INFINITA) {
			disparada = 0;
			if(tVida != INFINITA) tVida -= VELOCIDAD_MUERTE;
			animacionActiva = VACIA;
			/////COMPROBACIÓN DE COLISIÓN CON EL JUGADOR BUB
			if (!j1.muriendo){
				
				bool contactoFrente = ((destRec.x - destRec.width / 2) < (j1.posicionJugador.x - j1.posicionJugador.width / 2))
					&& ((destRec.x + destRec.width / 2) > (j1.posicionJugador.x - j1.posicionJugador.width / 2));
				bool contactoEspalda = ((destRec.x - destRec.width / 2) < (j1.posicionJugador.x + j1.posicionJugador.width / 2))
					&& ((destRec.x + destRec.width / 2) > (j1.posicionJugador.x + j1.posicionJugador.width / 2));
				bool contactoCabeza = (destRec.x - destRec.width / 2) <= (j1.posicionJugador.x + j1.posicionJugador.width / 2)
					&& (destRec.x + destRec.width / 2) >= (j1.posicionJugador.x + j1.posicionJugador.width / 2)
					|| (destRec.x + destRec.width / 2) >= (j1.posicionJugador.x)
					&& (destRec.x - destRec.width / 2) < (j1.posicionJugador.x);
				if (j1.sentidoJugador != 2) { //Si J1 mira derecha, hay que cambiar
					//std::cout << "RECTIFICANDO" << std::endl;
					contactoFrente = ((destRec.x - destRec.width / 2) < (j1.posicionJugador.x + j1.posicionJugador.width / 2))
						&& ((destRec.x + destRec.width / 2) > (j1.posicionJugador.x + j1.posicionJugador.width / 2));
					contactoEspalda = ((destRec.x - destRec.width / 2) < (j1.posicionJugador.x - j1.posicionJugador.width / 2))
						&& ((destRec.x + destRec.width / 2) > (j1.posicionJugador.x - j1.posicionJugador.width / 2));
					contactoCabeza = (destRec.x - destRec.width / 2) <= (j1.posicionJugador.x - j1.posicionJugador.width / 2)
						&& (destRec.x + destRec.width / 2) >= (j1.posicionJugador.x - j1.posicionJugador.width / 2)
						|| (destRec.x + destRec.width / 2) >= (j1.posicionJugador.x)
						&& (destRec.x - destRec.width / 2) < (j1.posicionJugador.x);
					//std::cout << contactoEspalda<< std::endl;
				}

				if (((destRec.x - destRec.width / 2) < (j1.posicionJugador.x + j1.posicionJugador.width / 2) && (destRec.x + destRec.width / 2) > (j1.posicionJugador.x + j1.posicionJugador.width / 2)
					|| (destRec.x + destRec.width / 2) > (j1.posicionJugador.x - j1.posicionJugador.width) && (destRec.x - destRec.width / 2) < (j1.posicionJugador.x - j1.posicionJugador.width / 2))
					&& (j1.posicionJugador.y + j1.posicionJugador.height / 2) > (destRec.y - destRec.height / 2) && (j1.posicionJugador.y + j1.posicionJugador.height / 2) < (destRec.y + destRec.height / 2)
					&& j1.jugadorCayendo && animacionActiva != EXPLOTA) { //Choque en caída
					if (j1.debeRebotar == 0 && !IsKeyDown(KEY_SPACE)) { // Explota la pompa
						animacionActiva = EXPLOTA;
						indiceAnimacion = 0;
						tVida = -1;
						//std::cout << "Peto por caida" << std::endl;
						
						//Marca que su explosión debe transmitirse
						cadena = true; 

						if (modulo == MODULO_AGUA) {
							creaAgua = j1.sentidoJugador;
						}
						else {
							result = extraeEnemigo(true);
						}
					}
					else if (IsKeyDown(KEY_SPACE) && (j1.posicionJugador.y + j1.posicionJugador.height / 2) < (destRec.y - destRec.height / 4)) { //Debe rebotar sin explotar la pompa
						//std::cout << "DEBES REBOTAR" << std::endl;
						//int u; //DEBUG bloqueante
						//std::cin >> u;
						if (indiceAnimacion == 0) {
							indiceAnimacion = 1;
							contadorFrames = 0;
						}
						j1.debeRebotar = 1;
					}


				}
				else if (contactoEspalda
					&& ((destRec.y - destRec.width / 2) < (j1.posicionJugador.y + j1.posicionJugador.height / 4) && (destRec.y + destRec.height / 2) > (j1.posicionJugador.y + j1.posicionJugador.height / 4)
						|| (destRec.y - destRec.width / 2) < (j1.posicionJugador.y - j1.posicionJugador.height / 2) && (destRec.y + destRec.height / 2) > (j1.posicionJugador.y - j1.posicionJugador.height / 2))) { //Choca por detrás del jugador (primera linea->horizontal, segunda y tercera->posibilidades verticales)
					animacionActiva = EXPLOTA;
					indiceAnimacion = 0;
					tVida = -1;

					//Marca que su explosión debe transmitirse
					cadena = true;

					if (modulo == MODULO_AGUA) {
						creaAgua = j1.sentidoJugador;
					}
					else {
						result = extraeEnemigo(true);
					}

					//std::cout << "Peto por espalda" << std::endl;
				}
				else if ((destRec.y + destRec.height / 2) >= (j1.posicionJugador.y - j1.posicionJugador.height / 2) && (destRec.y - destRec.height / 2) <= (j1.posicionJugador.y - j1.posicionJugador.height / 2)
					&& (contactoCabeza)) {	//Choque por arriba
					animacionActiva = EXPLOTA;
					indiceAnimacion = 0;
					tVida = -1;
					//std::cout << "Peto por cabeza" << std::endl;

					//Marca que su explosión debe transmitirse
					cadena = true;

					if (modulo == MODULO_AGUA) {
						creaAgua = j1.sentidoJugador;
					}
					else {
						result = extraeEnemigo(true);
					}

				}
				else if (contactoFrente
					&& ((destRec.y - destRec.width / 2) < (j1.posicionJugador.y + j1.posicionJugador.height / 4) && (destRec.y + destRec.height / 2) > (j1.posicionJugador.y + j1.posicionJugador.height / 4)
						|| (destRec.y - destRec.width / 2) < (j1.posicionJugador.y - j1.posicionJugador.height / 2) && (destRec.y + destRec.height / 2) > (j1.posicionJugador.y - j1.posicionJugador.height / 2))) {
					
					if (j1.sentidoJugador == 2) {
						destRec.x -= j1.velLateral / 2;
					}
					else {
						destRec.x += j1.velLateral / 2;
					}
					if (indiceAnimacion == 0) {
						indiceAnimacion = 2;
						contadorFrames = 0;
					}
				}
				
			}

			/////FIN COMPROBACIÓN DE COLISIÓN CON EL JUGADOR BUB


			/////COMPROBACIÓN DE COLISIÓN CON EL JUGADOR BOB
			if (!j2.muriendo) {
				bool contactoFrente = ((destRec.x - destRec.width / 2) < (j2.posicionJugador.x - j2.posicionJugador.width / 2))
					&& ((destRec.x + destRec.width / 2) > (j2.posicionJugador.x - j2.posicionJugador.width / 2));
				bool contactoEspalda = ((destRec.x - destRec.width / 2) < (j2.posicionJugador.x + j2.posicionJugador.width / 2))
					&& ((destRec.x + destRec.width / 2) > (j2.posicionJugador.x + j2.posicionJugador.width / 2));
				bool contactoCabeza = (destRec.x - destRec.width / 2) <= (j2.posicionJugador.x + j2.posicionJugador.width / 2)
					&& (destRec.x + destRec.width / 2) >= (j2.posicionJugador.x + j2.posicionJugador.width / 2)
					|| (destRec.x + destRec.width / 2) >= (j2.posicionJugador.x)
					&& (destRec.x - destRec.width / 2) < (j2.posicionJugador.x);
				if (j2.sentidoJugador != 2) { //Si j2 mira derecha, hay que cambiar
					//std::cout << "RECTIFICANDO" << std::endl;
					contactoFrente = ((destRec.x - destRec.width / 2) < (j2.posicionJugador.x + j2.posicionJugador.width / 2))
						&& ((destRec.x + destRec.width / 2) > (j2.posicionJugador.x + j2.posicionJugador.width / 2));
					contactoEspalda = ((destRec.x - destRec.width / 2) < (j2.posicionJugador.x - j2.posicionJugador.width / 2))
						&& ((destRec.x + destRec.width / 2) > (j2.posicionJugador.x - j2.posicionJugador.width / 2));
					contactoCabeza = (destRec.x - destRec.width / 2) <= (j2.posicionJugador.x - j2.posicionJugador.width / 2)
						&& (destRec.x + destRec.width / 2) >= (j2.posicionJugador.x - j2.posicionJugador.width / 2)
						|| (destRec.x + destRec.width / 2) >= (j2.posicionJugador.x)
						&& (destRec.x - destRec.width / 2) < (j2.posicionJugador.x);
					//std::cout << contactoEspalda<< std::endl;
				}

				if (((destRec.x - destRec.width / 2) < (j2.posicionJugador.x + j2.posicionJugador.width / 2) && (destRec.x + destRec.width / 2) > (j2.posicionJugador.x + j2.posicionJugador.width / 2)
					|| (destRec.x + destRec.width / 2) > (j2.posicionJugador.x - j2.posicionJugador.width) && (destRec.x - destRec.width / 2) < (j2.posicionJugador.x - j2.posicionJugador.width / 2))
					&& (j2.posicionJugador.y + j2.posicionJugador.height / 2) > (destRec.y - destRec.height / 2) && (j2.posicionJugador.y + j2.posicionJugador.height / 2) < (destRec.y + destRec.height / 2)
					&& j2.jugadorCayendo && animacionActiva != EXPLOTA) { //Choque en caída
					if (j2.debeRebotar == 0 && !IsKeyDown(KEY_SPACE)) { // Explota la pompa
						animacionActiva = EXPLOTA;
						indiceAnimacion = 0;
						tVida = -1;
						//std::cout << "Peto por caida" << std::endl;

						//Marca que su explosión debe transmitirse
						cadena = true;

						if (modulo == MODULO_AGUA) {
							creaAgua = j2.sentidoJugador;
						}
						else {
							result = extraeEnemigo(true);
						}
					}
					else if (IsKeyDown(KEY_SPACE) && (j2.posicionJugador.y + j2.posicionJugador.height / 2) < (destRec.y - destRec.height / 4)) { //Debe rebotar sin explotar la pompa
						//std::cout << "DEBES REBOTAR" << std::endl;
						//int u; //DEBUG bloqueante
						//std::cin >> u;
						if (indiceAnimacion == 0) {
							indiceAnimacion = 1;
							contadorFrames = 0;
						}
						j2.debeRebotar = 1;
					}


				}
				else if (contactoEspalda
					&& ((destRec.y - destRec.width / 2) < (j2.posicionJugador.y + j2.posicionJugador.height / 4) && (destRec.y + destRec.height / 2) > (j2.posicionJugador.y + j2.posicionJugador.height / 4)
						|| (destRec.y - destRec.width / 2) < (j2.posicionJugador.y - j2.posicionJugador.height / 2) && (destRec.y + destRec.height / 2) > (j2.posicionJugador.y - j2.posicionJugador.height / 2))) { //Choca por detrás del jugador (primera linea->horizontal, segunda y tercera->posibilidades verticales)
					animacionActiva = EXPLOTA;
					indiceAnimacion = 0;
					tVida = -1;

					//Marca que su explosión debe transmitirse
					cadena = true;

					if (modulo == MODULO_AGUA) {
						creaAgua = j2.sentidoJugador;
					}
					else {
						result = extraeEnemigo(true);
					}

					//std::cout << "Peto por espalda" << std::endl;
				}
				else if ((destRec.y + destRec.height / 2) >= (j2.posicionJugador.y - j2.posicionJugador.height / 2) && (destRec.y - destRec.height / 2) <= (j2.posicionJugador.y - j2.posicionJugador.height / 2)
					&& (contactoCabeza)) {	//Choque por arriba
					animacionActiva = EXPLOTA;
					indiceAnimacion = 0;
					tVida = -1;
					//std::cout << "Peto por cabeza" << std::endl;

					//Marca que su explosión debe transmitirse
					cadena = true;

					if (modulo == MODULO_AGUA) {
						creaAgua = j2.sentidoJugador;
					}
					else {
						result = extraeEnemigo(true);
					}

				}
				else if (contactoFrente
					&& ((destRec.y - destRec.width / 2) < (j2.posicionJugador.y + j2.posicionJugador.height / 4) && (destRec.y + destRec.height / 2) > (j2.posicionJugador.y + j2.posicionJugador.height / 4)
						|| (destRec.y - destRec.width / 2) < (j2.posicionJugador.y - j2.posicionJugador.height / 2) && (destRec.y + destRec.height / 2) > (j2.posicionJugador.y - j2.posicionJugador.height / 2))) {

					if (j2.sentidoJugador == 2) {
						destRec.x -= j2.velLateral / 2;
					}
					else {
						destRec.x += j2.velLateral / 2;
					}
					if (indiceAnimacion == 0) {
						indiceAnimacion = 2;
						contadorFrames = 0;
					}
				}
			}
			/////FIN COMPROBACIÓN DE COLISIÓN CON EL JUGADOR BOB

			/////INICIO DESPLAZAMIENTO POR EL MAPA
			switch (ID_MAPA) {
			case 1: //Mapa del agua
			{
				if (destRec.x < GetScreenWidth() / 2) {
					if (destRec.y < (GetScreenHeight() * 0.85) && (destRec.y > (GetScreenHeight() * 0.80) && destRec.x >= 100)) {
						destRec.x += -0.5;
					} else if (destRec.y <= 70 && destRec.x < GetScreenWidth() / 2 - 50) {
						destRec.x += 0.5;
					} else if (destRec.y > 70 && !oscilando) {
						destRec.y -= 0.5;
					} else {
						oscilando = true;
					}
				}
				else {
					if ((destRec.y < (GetScreenHeight() * 0.85) && destRec.y > (GetScreenHeight() * 0.80) && destRec.x < GetScreenWidth() - 100)) {
						destRec.x += 0.5;
					}
					else if (destRec.y <= 70 && destRec.x > GetScreenWidth() / 2 + 50) {
						destRec.x -= 0.5;
					}
					else if(destRec.y > 70 && !oscilando){
						destRec.y -= 0.5;
					} else {
						oscilando = true;
					}
				}

				if (oscilando) {
					if (abajo) {
						destRec.y += 0.5;
					}
					else {
						destRec.y += -0.5;
					}
					contador++;
					if (contador >= 20) {
						abajo = !abajo;
						contador = 0;
					}
				}

				break;
			}

			default:
				if (!oscilando) {
					if (destRec.y > 70) {
						destRec.y += -0.5;
					}
					else {
						if (destRec.x < (GetScreenWidth() / 2) - GetScreenWidth() / 30) {
							destRec.x += 0.5;
							sentidoLateral = 2;
						}
						else if (destRec.x > (GetScreenWidth() / 2) + GetScreenWidth() / 30) {
							destRec.x -= 0.5;
							sentidoLateral = 3;
						}
						else {
							oscilando = true;
						}

					}
				}

				if (oscilando) {
					if (abajo) {
						destRec.y += 0.5;
					}
					else {
						destRec.y += -0.5;
					}
					contador++;
					if (contador >= 20) {
						abajo = !abajo;
						contador = 0;
					}
				}
			};
		}
		/////FIN DESPLAZAMIENTO POR EL MAPA

	}
	//Actualizar puntero de animacion
	cuentaFrames++;
	if (cuentaFrames >= (60 / velocidadFrames)) {
		cuentaFrames = 0;
		switch (animacionActiva) {
		case 0: //Disparo
			indiceAnimacion++;
			if (indiceAnimacion >= fDisparo) {
				indiceAnimacion = fDisparo - 2;
			}
			break;
		case 4: //Explota
			indiceAnimacion++;
			//std::cout << "Actualiza Anim a:" << indiceAnimacion << " fExplota: " << fExplota << std::endl;
			if (indiceAnimacion >= fExplota) {
				indiceAnimacion = 0;
				//std::cout << "brickeo: " << indiceAnimacion << std::endl;
				matame = true;
			}
			//std::cout << "Al salir es:" << indiceAnimacion << " fExplota: " << fExplota << std::endl;
			break;
		default:
			//std::cout << "WHAT" << std::endl;
			if (modulo > 0) {
				indiceAnimacion++;
				if (indiceAnimacion >= (modulo * NUM_FOTOGRAMAS) + NUM_FOTOGRAMAS) {
					indiceAnimacion = modulo * NUM_FOTOGRAMAS;
				}
			}
			else {
				if ((indiceAnimacion == 1 || indiceAnimacion == 2) && contadorFrames < maxFrames && contadorFrames > 0) {
					contadorFrames++;
				} else {
					contadorFrames = -1;
					indiceAnimacion = 0;
				}
			}
			
			break;
		};
	}
	return result;
}

Pompa &Pompa::operator=(const Pompa& p) {
	this->ID = p.ID;
	this->sprite = p.sprite;

	this->tVida = p.tVida;
	this->vidaMaxima = p.vidaMaxima;
	this->velocidadFrames = p.velocidadFrames;
	
	this->disparada = p.disparada;
	this->distanciaDisparo = p.distanciaDisparo;
	this->animacionActiva = p.animacionActiva;
	
	this->velocidadDesplazamiento = p.velocidadDesplazamiento;
	this->tamano = p.tamano;

	this->destRec = p.destRec;
	this->srcRec = p.srcRec;
	this->origin = p.origin; //En principio no lo necesitamosç

	return *this;
}

void Pompa::explota() {
	animacionActiva = EXPLOTA;
	indiceAnimacion = 0;
	tVida = -1;
}

void Pompa::Dibujar() {


	if (tVida <= vidaMaxima / 4 && tVida > 0) { //Pompa Naranja
		//std::cout << "ROJA" << std::endl;
		srcRec.y = (float)heightAnimation * (float)(animacionActiva + ROJO); //Cambia el fotograma
	}
	else if (tVida <= vidaMaxima / 2 && tVida > 0) { //Pompa Roja
		//std::cout << "NARANJA" << std::endl;
		srcRec.y = (float)heightAnimation * (float)(animacionActiva + NARANJA); //Cambia el fotograma
	}
	else { //Pompa Verde y demás
		srcRec.y = (float)heightAnimation * (float)animacionActiva; //Cambia el fotograma
		//std::cout << "VERDE" << std::endl;
	}
	srcRec.x = (float)widthAnimation * (float)indiceAnimacion; //Cambia la animación activa
	/*
	srcRec.x = 0.0f; //Pompa verde
	srcRec.y = 16.0f; //Pompa verde*/
	DrawTexturePro(sprite, srcRec, destRec, origin, 0.0f, WHITE);
}