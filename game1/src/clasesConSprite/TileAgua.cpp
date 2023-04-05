#include "TileAgua.hpp"

tileAgua::tileAgua(Rectangle destino, bool right, Texture2D sprite, int plats) {
	destRec = destino;
	srcRec = { 8.0f, 0.0f, (float)height, (float)width };
	direccionDerecha = right;
	spritesheet = sprite;
	numPlataformas = plats;
}

Response tileAgua::ActualizarHead(Plataformas& plat) {

	Response current;
	current.rect = destRec; current.enElAire = enElAire;
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

Response tileAgua::Actualizar(Response input) {
	Response output;
	output.rect = destRec; output.enElAire = enElAire;
	destRec = input.rect; enElAire = input.enElAire;
	return output;
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

bool tileAgua::colisionBub(Bub& bub) {
	if (
		(
			//Comprobamos colision esquina inferior derecha
			(((destRec.y + destRec.height) >= (bub.destRec.y + bub.destRec.height / 2)) &&
				((bub.destRec.y + bub.destRec.height / 2) >= (destRec.y - destRec.height))
				) && (
					((destRec.x + destRec.width) >= (bub.destRec.x + bub.destRec.width / 2)) &&
					((bub.destRec.x + bub.destRec.width / 2) >= (destRec.x - destRec.width))
					)
			) ||
		(
			//Comprobamos colision esquina superior derecha
			(((destRec.y + destRec.height) >= (bub.destRec.y - bub.destRec.height / 2)) &&
				((bub.destRec.y - bub.destRec.height / 2) >= (destRec.y - destRec.height))
				) && (
					((destRec.x + destRec.width) >= (bub.destRec.x + bub.destRec.width / 2)) &&
					((bub.destRec.x + bub.destRec.width / 2) >= (destRec.x - destRec.width))
					)
			) ||
		(
			//Comprobamos colision esquina superior izquierda
			(((destRec.y + destRec.height) >= (bub.destRec.y - bub.destRec.height / 2)) &&
				((bub.destRec.y - bub.destRec.height / 2) > (destRec.y - destRec.height))
				) && (
					((destRec.x + destRec.width) >= (bub.destRec.x - bub.destRec.width / 2)) &&
					((bub.destRec.x - bub.destRec.width / 2) >= (destRec.x - destRec.width))
					)
			) ||
		(
			//Comprobamos colision esquina inferior izquierda
			(((destRec.y + destRec.height) >= (bub.destRec.y + bub.destRec.height / 2)) &&
				((bub.destRec.y + bub.destRec.height / 2) >= (destRec.y - destRec.height))
				) && (
					((destRec.x + destRec.width) >= (bub.destRec.x - bub.destRec.width / 2)) &&
					((bub.destRec.x - bub.destRec.width / 2) >= (destRec.x - destRec.width))
					)
			)
		) {
		std::cout << destRec.x << " " << destRec.y << std::endl;
		std::cout << bub.destRec.x << " " << bub.destRec.y << std::endl;
		bub.enElAgua = true;
		return true;
	}
	else {
		return false;
	}
}


void tileAgua::Dibujar(int pos) {
	/*if (pos == 0) {
		if (enElAire) {

		}
		else {

		}
	}
	else if (pos == 9) {
		if (enElAire) {

		}
		else {

		}
	}
	else {
		if (enElAire) {

		}
		else {

		}
	}*/
	if (enElAire) {
		srcRec.x = 72;
	}
	else {
		srcRec.x = 64;
	}
	DrawTexturePro(spritesheet, srcRec, destRec, origin, 0.0f, WHITE); //de donde está sacando origin
}