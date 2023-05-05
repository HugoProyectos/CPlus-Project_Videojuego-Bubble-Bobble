#include "Rayo.hpp"
#include <iostream>

bool Rayo::mute_sound = false;

Rayo::Rayo(Texture2D spriteSheet, const Rectangle origen, bool izquierda) {
	Inicializador(spriteSheet, origen, izquierda);
}

void Rayo::Inicializador(Texture2D spriteSheet, const Rectangle origen, bool izquierda) {
	sprite = spriteSheet;
	animacionActiva = 0;
	izquierda = izquierda;
	velocidadFrames = 8;
	destRec = origen; 
	srcRec = { 0.0f, 0.0f, (float)heightAnimation, (float)widthAnimation };
	origin = { origen.width / 2, origen.height / 2 }; //En principio no lo necesitamos
}

void Rayo::Actualizar(DatosJugador& j1, DatosJugador& j2) {
	if (lastHeight != GetScreenHeight()) {
		destRec.height = GetScreenHeight() / 14.0625f;
		destRec.y = GetScreenHeight() * (destRec.y / lastHeight);
		origin.y = destRec.height / 2;

		lastHeight = GetScreenHeight();
	}
	if (lastWidth != GetScreenWidth()) {
		destRec.width = GetScreenWidth() / 25.0f;
		destRec.x = GetScreenWidth() * (destRec.x / lastWidth);
		origin.x = destRec.width / 2;

		lastWidth = GetScreenWidth();
	}
	ratioX = destRec.width / 32;

	if (animacionActiva == 0) {
		if (izquierda) {
			destRec.x -= velocidadDesplazamiento * ratioX;
		}
		else {
			destRec.x += velocidadDesplazamiento * ratioX;
		}

		//Comprueba colisión con los jugadores
		if ((!j1.muriendo && !j1.electrocutado && j1.thunderLessFrames <= 0 && !j1.heIsTheThunder && !golpeJefe
			&& ((destRec.y + destRec.height / 2.0f) >= (j1.posicionJugador.y + j1.posicionJugador.height / 2.0f)
				&& (destRec.y - destRec.height / 2.0f) <= (j1.posicionJugador.y + j1.posicionJugador.height / 2.0f)
				|| (destRec.y + destRec.height / 2.0f) >= (j1.posicionJugador.y - j1.posicionJugador.height / 2.0f)
				&& (destRec.y - destRec.height / 2.0f) <= (j1.posicionJugador.y - j1.posicionJugador.height / 2.0f))
			&& ((destRec.x + destRec.width / 2 - destRec.width * 0.125) >= (j1.posicionJugador.x - j1.posicionJugador.width / 2.0f)
				&& (destRec.x - destRec.width / 2 + destRec.width * 0.125) <= (j1.posicionJugador.x - j1.posicionJugador.width / 2.0f)
				|| (destRec.x + destRec.width / 2 - destRec.width * 0.125) >= (j1.posicionJugador.x + j1.posicionJugador.width / 2.0f)
				&& (destRec.x - destRec.width / 2 + destRec.width * 0.125) <= (j1.posicionJugador.x + j1.posicionJugador.width / 2.0f)))) {
			j1.electrocutalo = true;
			if (animacionActiva == 0) {
				animacionActiva = 1;
				indiceAnimacion = 0;
				if (!mute_sound) {
					PlaySound(sonidoRayo);
				}
			}
		}
		if ((!j2.muriendo && !j2.electrocutado && j2.thunderLessFrames <= 0 && !j2.heIsTheThunder && !golpeJefe
			&& ((destRec.y + destRec.height / 2.0f) >= (j2.posicionJugador.y + j2.posicionJugador.height / 2.0f)
				&& (destRec.y - destRec.height / 2.0f) <= (j2.posicionJugador.y + j2.posicionJugador.height / 2.0f)
				|| (destRec.y + destRec.height / 2.0f) >= (j2.posicionJugador.y - j2.posicionJugador.height / 2.0f)
				&& (destRec.y - destRec.height / 2.0f) <= (j2.posicionJugador.y - j2.posicionJugador.height / 2.0f))
			&& ((destRec.x + destRec.width / 2 - destRec.width * 0.125) >= (j2.posicionJugador.x - j2.posicionJugador.width / 2.0f)
				&& (destRec.x - destRec.width / 2 + destRec.width * 0.125) <= (j2.posicionJugador.x - j2.posicionJugador.width / 2.0f)
				|| (destRec.x + destRec.width / 2 - destRec.width * 0.125) >= (j2.posicionJugador.x + j2.posicionJugador.width / 2.0f)
				&& (destRec.x - destRec.width / 2 + destRec.width * 0.125) <= (j2.posicionJugador.x + j2.posicionJugador.width / 2.0f)))) {
			j2.electrocutalo = true;
			if (animacionActiva == 0) {
				animacionActiva = 1;
				indiceAnimacion = 0;
				if (!mute_sound) {
					PlaySound(sonidoRayo);
				}
			}
		}

		if (destRec.x + destRec.width / 2 < 0 || destRec.x - destRec.width / 2 > GetScreenWidth()) {
			borrame = true;
		}
	}

	//Actualización de la animación
	cuentaFrames++;
	if (cuentaFrames >= (60 / velocidadFrames)) {
		cuentaFrames = 0;
		switch (animacionActiva) {
		case 1: //Hit
			indiceAnimacion++;
			if (indiceAnimacion >= fHit) {
				indiceAnimacion = 0;
				animacionActiva = 0;
				if (golpeJefe) {
					borrame = true;
				}
			}
			break;
		default:
			indiceAnimacion++;
			if (indiceAnimacion >= fMove) {
				indiceAnimacion = 0;
			}

			break;
		};
	}
	
}

void Rayo::Dibujar() {
	srcRec.y = (float)heightAnimation * (float)animacionActiva; //Cambia la animación activa
	srcRec.x = (float)widthAnimation * (float)indiceAnimacion;  //Cambia el fotograma
	DrawTexturePro(sprite, srcRec, destRec, origin, 0.0f, WHITE);
}