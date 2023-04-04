#include "TileAgua.hpp"

tileAgua::tileAgua(Rectangle destino, bool right, Texture2D sprite, int plats) {
	destRec = destino;
	direccionDerecha = right;
	spritesheet = sprite;
	numPlataformas = plats;
}

Rectangle tileAgua::ActualizarHead(Plataformas& plat) {

	Rectangle current = destRec;
	//PARTE ESPECIAL PARA PRUEBAS************************************
	if (destRec.y > 500) {
		destRec = { 150, 100, 16, 16 };
		return current;
	}
	//***************************************************************

	if (enElAire) {
		destRec.y += 16;
		//Comprobamos si hemos llegado al suelo
		for (int i = 0; i < numPlataformas; i++) {
			if (comprobarColisionY(plat.listaPlataforma[i])) {
				lastGroundNum = i;
				return current;
			}
		}
	}
	else {
		if (direccionDerecha) {
			destRec.x += 16;
			for (int i = 0; i < numPlataformas; i++) {
				//Comprobamos si hemos chocado con una pared
				if (comprobarColisionX(plat.listaPlataforma[i])) {
					return current;
				}
			}
			//Comprobamos si seguimos tocando el suelo
			comprobarSuelo(plat.listaPlataforma[lastGroundNum]);
		}
		else {
			destRec.x -= 16;
			for (int i = 0; i < numPlataformas; i++) {
				//Comprobamos si hemos chocado con una pared
				if (comprobarColisionX(plat.listaPlataforma[i])) {
					return current;
				}
			}
			//Comprobamos si seguimos tocando el suelo
			comprobarSuelo(plat.listaPlataforma[lastGroundNum]);
		}
	}
	return current;
}

Rectangle tileAgua::Actualizar(Rectangle pos) {
	Rectangle current = destRec;
	destRec = pos;
	return current;
}

bool tileAgua::comprobarColisionY(Plataforma& plat) {
	if (
		(
			(plat.left < (destRec.x + destRec.width / 2)) &&
			(plat.right > (destRec.x + destRec.width / 2)) &&
			(plat.top < (destRec.y + destRec.width / 2)) &&
			(plat.bot > (destRec.y + destRec.width / 2))
			) ||
		(
			(plat.left < (destRec.x - destRec.width / 2)) &&
			(plat.right > (destRec.x - destRec.width / 2)) &&
			(plat.top < (destRec.y + destRec.width / 2)) &&
			(plat.bot > (destRec.y + destRec.width / 2))
			)
		) {
		destRec.y = plat.top - destRec.height;
		enElAire = false;
		//lastGround = plat;
	}
	return false;
}

bool tileAgua::comprobarColisionX(Plataforma& plat) {
	if (direccionDerecha) {
		if (
			(
			(plat.left < (destRec.x + destRec.width / 2)) &&
			(plat.right > (destRec.x + destRec.width / 2)) &&
			(plat.top < (destRec.y + destRec.width / 2)) && 
			(plat.bot > (destRec.y + destRec.width / 2))
			) ||
			(
			(plat.left < (destRec.x + destRec.width / 2)) &&
			(plat.right > (destRec.x + destRec.width / 2)) &&
			(plat.top < (destRec.y - destRec.width / 2)) &&
			(plat.bot > (destRec.y - destRec.width / 2))
			)
			) {
			destRec.x = plat.left - destRec.width;
			direccionDerecha = false;
			return true;
		}
	}
	else {
		if (
		    (
			(plat.left < (destRec.x - destRec.width / 2)) &&
			(plat.right > (destRec.x - destRec.width / 2)) &&
			(plat.top < (destRec.y + destRec.width / 2)) &&
			(plat.bot > (destRec.y + destRec.width / 2))
			) ||
			(
			(plat.left < (destRec.x - destRec.width / 2)) &&
			(plat.right > (destRec.x - destRec.width / 2)) &&
			(plat.top < (destRec.y - destRec.width / 2)) &&
			(plat.bot > (destRec.y - destRec.width / 2))
			)
			) {
			destRec.x = plat.left + destRec.width;
			direccionDerecha = true;
			return true;
		}
	}
	return false;
}

void tileAgua::comprobarColisionParedes(Columnas& col) {
	//Comprobamos columna derecha
	if (direccionDerecha) {
		if (col.left_der < (destRec.x + destRec.width / 2)) {
			destRec.x = col.left_der - destRec.width / 2;
			direccionDerecha = false;
		}
	}
	else {
		//Comprobamos columna izquierda
		if (col.right_izq > (destRec.x - destRec.width / 2)) {
			destRec.x = col.right_izq + destRec.width / 2;
			direccionDerecha = true;
		}
	}
}

void tileAgua::comprobarSuelo(Plataforma& lastGround) {
	
	if (!(
		(
			(lastGround.left < (destRec.x - destRec.width / 2)) &&
			(lastGround.right > (destRec.x - destRec.width / 2)) &&
			(lastGround.top < (destRec.y + destRec.width / 2 + 1)) &&
			(lastGround.bot > (destRec.y + destRec.width / 2 + 1))
			) ||
		(
			(lastGround.left < (destRec.x + destRec.width / 2)) &&
			(lastGround.right > (destRec.x + destRec.width / 2)) &&
			(lastGround.top < (destRec.y + destRec.width / 2 + 1)) &&
			(lastGround.bot > (destRec.y + destRec.width / 2 + 1))
			)
		)
		) {
		enElAire = true;
	}
}

void tileAgua::Dibujar() {
	DrawTexturePro(spritesheet, srcRec, destRec, origin, 0.0f, WHITE);
}