#pragma once
#include "Pompa.hpp"


uint32_t Pompa::ID_GLOBAL = 0;

sh_Enemigo Pompa::extraeEnemigo(bool matalo) {
	sh_Enemigo resultado = NULL;
	if (modulo == 1) {
		enemigoContenido->destRec = destRec;
		enemigoContenido->borrame = false;
		enemigoContenido->enElAire = true;
		enemigoContenido->cayendo = true;
		enemigoContenido->muerto = matalo;
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
	ID = ID_GLOBAL++; //Asigna identificador �nico (De no ser todo secuencial, habr� que gestionar secci�n cr�tica)

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

sh_Enemigo Pompa::Actualizar(Rectangle pJ1, bool cayendoJ1, int sentidoJ1, bool muriendoJ1, int& j1DebeRebotar) {
	sh_Enemigo result = NULL;

	if (disparada == 2) {
		//std::cout << "PIUM" << std::endl;
		distanciaRecorrida += velocidadDesplazamiento; //Actualiza el contador
		//std::cout << distanciaRecorrida << std::endl;
		//std::cout << distanciaDisparo << std::endl;
		destRec.x += velocidadDesplazamiento;
		if (abs(distanciaRecorrida) >= abs(distanciaDisparo)) {
			disparada = 1; //�ltimo frame en que atrapa enemigos
		}
	}
	else {
		//Si ha iniciado cadena y ya est� explotando, es solo en el primer frame que transfiere cadena
		if (cadena && animacionActiva == EXPLOTA) {
			cadena = false;
		} else if (cadena) { // Si a�n no ha explotado, inicia explosi�n
			animacionActiva = EXPLOTA;
			indiceAnimacion = 0;
			tVida = -1;
			std::cout << "Peto por cadena" << std::endl;

			result = extraeEnemigo(true);
		}

		//Con las interacciones debe hacerlo solo si no tiene enemigo dentro
		else if (tVida == 0) {
			animacionActiva = EXPLOTA;
			indiceAnimacion = 0;
			tVida--;
			
			result = extraeEnemigo(false);
			
		}
		else if (tVida > 0) {
			disparada = 0;
			tVida -= VELOCIDAD_MUERTE;
			animacionActiva = VACIA;

			if (!muriendoJ1){
				/////COMPROBACI�N DE COLISI�N CON EL JUGADOR
				bool contactoEspalda = ((destRec.x - destRec.width / 2) < (pJ1.x + pJ1.width / 2))
					&& ((destRec.x + destRec.width / 2) > (pJ1.x + pJ1.width / 2));
				bool contactoCabeza = (destRec.x - destRec.width / 2) <= (pJ1.x + pJ1.width / 2)
					&& (destRec.x + destRec.width / 2) >= (pJ1.x + pJ1.width / 2)
					|| (destRec.x + destRec.width / 2) >= (pJ1.x)
					&& (destRec.x - destRec.width / 2) < (pJ1.x);
				if (sentidoJ1 != 2) { //Si J1 mira derecha, hay que cambiar
					//std::cout << "RECTIFICANDO" << std::endl;
					contactoEspalda = ((destRec.x - destRec.width / 2) < (pJ1.x - pJ1.width / 2))
						&& ((destRec.x + destRec.width / 2) > (pJ1.x - pJ1.width / 2));
					contactoCabeza = (destRec.x - destRec.width / 2) <= (pJ1.x - pJ1.width / 2)
						&& (destRec.x + destRec.width / 2) >= (pJ1.x - pJ1.width / 2)
						|| (destRec.x + destRec.width / 2) >= (pJ1.x)
						&& (destRec.x - destRec.width / 2) < (pJ1.x);
					//std::cout << contactoEspalda<< std::endl;
				}

				if (((destRec.x - destRec.width / 2) < (pJ1.x + pJ1.width / 2) && (destRec.x + destRec.width / 2) > (pJ1.x + pJ1.width / 2)
					|| (destRec.x + destRec.width / 2) > (pJ1.x - pJ1.width) && (destRec.x - destRec.width / 2) < (pJ1.x - pJ1.width / 2))
					&& (pJ1.y + pJ1.height / 2) > (destRec.y - destRec.height / 2) && (pJ1.y + pJ1.height / 2) < (destRec.y + destRec.height / 2)
					&& cayendoJ1 && animacionActiva != EXPLOTA) { //Choque en ca�da
					if (j1DebeRebotar == 0 && !IsKeyDown(KEY_SPACE)) { // Explota la pompa
						animacionActiva = EXPLOTA;
						indiceAnimacion = 0;
						tVida = -1;
						std::cout << "Peto por caida" << std::endl;
						
						//Marca que su explosi�n debe transmitirse
						cadena = true; 

						result = extraeEnemigo(true);
					}
					else if (IsKeyDown(KEY_SPACE) && (pJ1.y + pJ1.height / 2) < (destRec.y - destRec.height / 4)) { //Debe rebotar sin explotar la pompa
						std::cout << "DEBES REBOTAR" << std::endl;
						//int u; //DEBUG bloqueante
						//std::cin >> u;
						if (indiceAnimacion == 0) {
							indiceAnimacion = 1;
							contadorFrames = 0;
						}
						j1DebeRebotar = 1;
					}


				}
				else if (contactoEspalda
					&& ((destRec.y - destRec.width / 2) < (pJ1.y + pJ1.height / 4) && (destRec.y + destRec.height / 2) > (pJ1.y + pJ1.height / 4)
						|| (destRec.y - destRec.width / 2) < (pJ1.y - pJ1.height / 2) && (destRec.y + destRec.height / 2) > (pJ1.y - pJ1.height / 2))) { //Choca por detr�s del jugador (primera linea->horizontal, segunda y tercera->posibilidades verticales)
					animacionActiva = EXPLOTA;
					indiceAnimacion = 0;
					tVida = -1;

					//Marca que su explosi�n debe transmitirse
					cadena = true;

					result = extraeEnemigo(true);

					std::cout << "Peto por espalda" << std::endl;
				}
				else if ((destRec.y + destRec.height / 2) >= (pJ1.y - pJ1.height / 2) && (destRec.y - destRec.height / 2) <= (pJ1.y - pJ1.height / 2)
					&& (contactoCabeza)) {	//Choque por arriba
					animacionActiva = EXPLOTA;
					indiceAnimacion = 0;
					tVida = -1;
					std::cout << "Peto por cabeza" << std::endl;

					//Marca que su explosi�n debe transmitirse
					cadena = true;

					result = extraeEnemigo(true);

				}
				
			}

			/////FIN COMPROBACI�N DE COLISI�N CON EL JUGADOR

			//destRec.x -= 1;

			//Que vaya hacia arriba poco a poco
			if (!oscilando) {
				if (destRec.y > 70) {
					destRec.y += -0.5;
				}
				else {
					oscilando = true;
				}
			}

			if (oscilando) {
				if (abajo) {
					destRec.y += 0.5;
				} else {
					destRec.y += -0.5;
				}
				contador++;
				if (contador >= 20) {
					abajo = !abajo;
					contador = 0;
				}
			}
			
		}

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
				if (indiceAnimacion == 1 && contadorFrames < maxFrames && contadorFrames > 0) {
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

void Pompa::Dibujar() {


	if (tVida <= vidaMaxima / 4 && tVida > 0) { //Pompa Naranja
		//std::cout << "ROJA" << std::endl;
		srcRec.y = (float)heightAnimation * (float)(animacionActiva + ROJO); //Cambia el fotograma
	}
	else if (tVida <= vidaMaxima / 2 && tVida > 0) { //Pompa Roja
		//std::cout << "NARANJA" << std::endl;
		srcRec.y = (float)heightAnimation * (float)(animacionActiva + NARANJA); //Cambia el fotograma
	}
	else { //Pompa Verde y dem�s
		srcRec.y = (float)heightAnimation * (float)animacionActiva; //Cambia el fotograma
		//std::cout << "VERDE" << std::endl;
	}
	srcRec.x = (float)widthAnimation * (float)indiceAnimacion; //Cambia la animaci�n activa
	/*
	srcRec.x = 0.0f; //Pompa verde
	srcRec.y = 16.0f; //Pompa verde*/
	DrawTexturePro(sprite, srcRec, destRec, origin, 0.0f, WHITE);
}