#include "Agua.hpp"
/*
Agua::Agua()
{
	existe = false;
	Rectangle dest = { -1,-1,-1,-1 };
	for (int i = 0; i < 10; i++) {
		stream[i] = tileAgua(dest, false, sprite, 0);
	}
}*/

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
		if (stream[0].destRec.y < 500) {
			for (int i = 1; i < 10; i++) {
				next = stream[i].Actualizar(next);
			}
			stream[0].comprobarColisionParedes(col);
		}
		else {
			existe = false;
		}
	}
}

bool Agua::colisionBub(Rectangle bub, int framesInmunidad) {
	if (framesInmunidad == 0) {
		for (int i = 0; i < 10; i++) {
			if (stream[i].colisionBub(bub)) {
				bubTile = i;
				return true;
			}
		}
	}
	return false;
}

void Agua::Dibujar() {
	if (existe) {
		for (int i = 0; i < 10; i++) {
			stream[i].Dibujar(i);
		}
	}
}