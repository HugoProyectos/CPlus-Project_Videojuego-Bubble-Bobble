#pragma once
#include "Sprite.hpp"
#include <string>
#include <cmath>

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
	const int EXPLOTA = 1;

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


	Pompa() = default;
	Pompa(const Rectangle origen, float velDisparo, float distanciaDisparo, bool esDisparada, int tiempoVida) {
		Inicializador(origen, velDisparo, distanciaDisparo, disparada, tiempoVida);
	};

	void Inicializador(const Rectangle origen, float velDisparo, float distanciaDisparo, bool esDisparada, int tiempoVida) {
		sprite = LoadTexture("resources/Players/Bobblun/pompa.png");
		
		destRec = origen;
		tVida = tiempoVida;
		if (esDisparada) {
			disparada = 2;
			animacionActiva = DISPARO;
		}else {
			disparada = 0;
			animacionActiva = VERDE;
		}
		velocidadDesplazamiento = velDisparo;

		destRec = origen;
		srcRec = { 0.0f, 0.0f, (float)heightAnimation, (float)widthAnimation };
	};

	void Actualizar() {
		if (disparada == 2) {
			distanciaRecorrida += velocidadDesplazamiento;
			if (abs(distanciaRecorrida) >= abs(distanciaDisparo)) {
				disparada = 1; //Último frame en que atrapa enemigos
			}
		}
		else {
			disparada = 0;
			tVida -= VELOCIDAD_MUERTE;


			//Con las interacciones debe hacerlo solo si no tiene enemigo dentro
			if (tVida == 0) {
				animacionActiva = EXPLOTA;
				indiceAnimacion = 0;
			}
			else {
				animacionActiva = VACIA;
			}
 
		}
		switch (animacionActiva) {
		case 0: //Disparo
			indiceAnimacion++;
			if (indiceAnimacion >= fDisparo) {
				indiceAnimacion = fDisparo - 1;
			}
			break;
		case 2: //Explota
			indiceAnimacion++;
			if (indiceAnimacion >= fExplota) {
				indiceAnimacion = 0;
				matame = true;
			}
			break;
		default:
			indiceAnimacion = 0;
			break;
		};

	};

	void Dibujar() {
		/*
		if (tVida <= vidaMaxima / 2) { //Pompa Naranja
			srcRec.x = (float)widthAnimation * (float)(indiceAnimacion + NARANJA); //Cambia el fotograma
		}
		else if (tVida <= vidaMaxima / 4) { //Pompa Roja
			srcRec.x = (float)widthAnimation * (float)(indiceAnimacion + ROJO); //Cambia el fotograma
		}
		else { //Pompa Verde y demás
			srcRec.x = (float)widthAnimation * (float)indiceAnimacion; //Cambia el fotograma
		}
		srcRec.y = (float)widthAnimation * (float)animacionActiva; //Cambia la animación activa
		*/
		DrawTexturePro(sprite, srcRec, destRec, origin, 0.0f, WHITE);
	};
};