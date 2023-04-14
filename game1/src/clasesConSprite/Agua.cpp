#include "Agua.hpp"

Agua::Agua(Rectangle destino, bool right, Texture2D sprite, int numPlat)
{
	existe = true;
	for (int i = 0; i < 10; i++) {
		stream[i] = tileAgua(destino, right, sprite, numPlat);
	}
}

void Agua::Actualizar(Plataformas& plat, Columnas& col) {
	if (existe) {
		Response next = stream[0].ActualizarHead(plat);
		for (int i = 1; i < 10; i++) {
			next = stream[i].Actualizar(next);
		}
		stream[0].comprobarColisionParedes(col);
	}
}

void Agua::colisionBub(Bub& bub) {
	if (bub.waterlessFrames == 0) {
		for (int i = 0; i < 10; i++) {
			if (stream[i].colisionBub(bub)) {
				bubTile = i;
				return;
			}
		}
	}
}

void Agua::colisionEnemigo(Enemigo& e) {
	for (int i = 0; i < 10; i++) {
		(stream[i].colisionEnemigo(e));
	}
}

void Agua::Dibujar() {
	if (existe) {
		for (int i = 0; i < 10; i++) {
			stream[i].Dibujar(i);
		}
	}
}