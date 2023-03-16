#pragma once
#include "Sprite.hpp"
#include <string>
#include <cmath>
#include <iostream> //DEBUG

// Aquí debajo se pondrán las constantes para especificar 
// las animaciones de enemigos atrapados en pompas
const int ROBOT = 5;
const int FANTASMA = 8;

/* Tipo de dato que representa las burbujas que disparan Bub y Bob.
 * A continuación, la lista de animaciones que se contienen en su variable
 * <sprite> junto al número de fotogramas que tiene:
 * 0 -> Disparo -> 7
 * 1 -> Verde	-> 3
 * 2 -> Naranja -> 3
 * 3 -> Rojo	-> 3
 * 4 -> Explota -> 10
 * 5,6,7 -> Pompas verde-naranja-rojo (3-3-3) de un enemigo (así con todos)
 */
class Pompa : public Sprite {
	const int DISPARO = 0;
	const int VACIA = 1;
	const int EXPLOTA = 4;

	//Índices de color
	const int VERDE = 0;
	const int NARANJA = 1;
	const int ROJO = 2;

	const int widthAnimation = 16;
	const int heightAnimation = 16;

	const int fDisparo = 7;
	const int fExplota = 10;

	//Razón a la que se va restando la vida a la pompa
	const int VELOCIDAD_MUERTE = 1;
public:
	//Vida de la pompa (asociado a naranja-rojo)
	int vidaMaxima = 0;
	int tVida = 0;
	bool matame = false;

	//Enemigo contenido
	int enemigoContenido = 0;

	//Desplazamiento
	float velocidadDesplazamiento = 0;
	//Relacionado con el modo disparo
	float distanciaRecorrida = 0;
	float distanciaDisparo = 0;
	int disparada = 2;

	//Control de animaciones
	int animacionActiva;
	int indiceAnimacion = 0;
	int indiceExtra = 0;
	int cuentaFrames = 0;
	int velocidadFrames = 0;


	Pompa() = default;
	Pompa(const Rectangle origen, float velDisparo, float distanciaDisparo, bool esDisparada, int tiempoVida) {
		Inicializador(origen, velDisparo, distanciaDisparo, disparada, tiempoVida);
	};

	void Inicializador(const Rectangle origen, float velDisparo, float distanciaDisparo, bool esDisparada, int tiempoVida) {
		sprite = LoadTexture("resources/Players/Bobblun/pompa.png");

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

	};

	void Actualizar() {
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
			//Con las interacciones debe hacerlo solo si no tiene enemigo dentro
			if (tVida == 0) {
				animacionActiva = EXPLOTA;
				indiceAnimacion = 0;
				tVida--;
			} else if (tVida > 0){
				disparada = 0;
				tVida -= VELOCIDAD_MUERTE;
				animacionActiva = VACIA;

				//Que vaya hacia arriba poco a poco
				destRec.y += -1;
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
				indiceAnimacion = 0;
				break;
			};
		}

	};

	void Dibujar() {
		
		
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
	};
};