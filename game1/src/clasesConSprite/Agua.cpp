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
	lastWidth = GetScreenWidth();
	lastHeight = GetScreenHeight();
	existe = true;
	for (int i = 0; i < 10; i++) {
		stream[i] = tileAgua(destino, right, sprite, numPlat);
	}
}

void Agua::Actualizar(Plataformas& plat, Columnas& col) {
	if (existe) {
		if ((GetScreenHeight() != lastHeight) || (GetScreenWidth() != lastWidth)) {
			for (int i = 0; i < 9; i++) {
				stream[i].destRec.height = GetScreenHeight() / 34.6f;
				stream[i].destRec.width = GetScreenWidth() / 61.5f;
				stream[i].destRec.y = (stream[0].destRec.y / lastHeight) * GetScreenHeight() - 5;
				stream[i].destRec.x = (stream[0].destRec.x / lastWidth) * GetScreenWidth();
			}
			lastHeight = GetScreenHeight();
			lastWidth = GetScreenWidth();
		}
		Response next = stream[0].ActualizarHead(plat);
		if (stream[9].destRec.y < (GetScreenHeight() * 1.1f) ) {
			for (int i = 1; i < 10; i++) {
				next = stream[i].Actualizar(next);
			}
			stream[0].comprobarColisionParedes(col);
		}
		else {
			for (int i = 0; i < 10; i++) {
				stream[i].destRec.x = -20;
				stream[i].destRec.y = -20;
			}
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