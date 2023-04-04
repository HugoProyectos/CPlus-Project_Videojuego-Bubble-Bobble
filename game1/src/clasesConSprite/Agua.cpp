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
		Rectangle next = stream[0].ActualizarHead(plat);
		for (int i = 1; i < 10; i++) {
			next = stream[i].Actualizar(next);
		}
		stream[0].comprobarColisionParedes(col);
	}
}

void Agua::Dibujar() {
	if (existe) {
		for (int i = 0; i < 10; i++) {
			stream[i].Dibujar();
		}
	}
}