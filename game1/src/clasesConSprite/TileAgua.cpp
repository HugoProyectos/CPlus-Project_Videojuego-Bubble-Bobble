#include "TileAgua.hpp"
#include "Enemigo.cpp"

tileAgua::tileAgua(Rectangle destino, bool right, Texture2D sprite, int plats) {
	destRec = destino;
	origin = { 0,0 };
	srcRec = { 0.0f, 0.0f, (float)height, (float)width };
	direccionDerecha = right;
	spritesheet = sprite;
	numPlataformas = plats;
}

Response tileAgua::ActualizarHead(Plataformas& plat) {

	Response current;
	current.rect = destRec; current.enElAire = enElAire;
	current.derecha = direccionDerecha; current.bend = bend;
	bend = false;
	//PARTE ESPECIAL PARA PRUEBAS************************************
	/*if (destRec.y > 500) {
		destRec.x = 150; destRec.y = 100;
		return current;
	}*/
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
			destRec.x += destRec.width;
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
			destRec.x -= destRec.width;
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
	output.derecha = direccionDerecha; output.bend = bend;
	destRec = input.rect; enElAire = input.enElAire; 
	direccionDerecha = input.derecha; bend = input.bend;
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
		bend = true;
		return true;
	}
	return false;
}

bool tileAgua::comprobarColisionX(Plataforma& plat) {
	if (direccionDerecha) {
		if (
			(
			(plat.left < (destRec.x + destRec.width)) &&
			(plat.right > (destRec.x + destRec.width)) &&
			(plat.top < (destRec.y + destRec.width)) && 
			(plat.bot > (destRec.y + destRec.width))
			) ||
			(
			(plat.left < (destRec.x + destRec.width)) &&
			(plat.right > (destRec.x + destRec.width)) &&
			(plat.top < (destRec.y)) &&
			(plat.bot > (destRec.y))
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
			(plat.left < (destRec.x)) &&
			(plat.right > (destRec.x)) &&
			(plat.top < (destRec.y + destRec.width)) &&
			(plat.bot > (destRec.y + destRec.width))
			) ||
			(
			(plat.left < (destRec.x)) &&
			(plat.right > (destRec.x)) &&
			(plat.top < (destRec.y)) &&
			(plat.bot > (destRec.y))
			)
			) {
			destRec.x = plat.right;
			direccionDerecha = true;
			return true;
		}
	}
	return false;
}

void tileAgua::comprobarColisionParedes(Columnas& col) {
	//Comprobamos columna derecha
	if (direccionDerecha) {
		if (col.left_der < (destRec.x + destRec.width)) {
			destRec.x = col.left_der - destRec.width;
			direccionDerecha = false;
		}
	}
	else {
		//Comprobamos columna izquierda
		if (col.right_izq > (destRec.x)) {
			destRec.x = col.right_izq;
			direccionDerecha = true;
		}
	}
}

void tileAgua::comprobarSuelo(Plataforma& lastGround) {
	
	if (!(
		(
			(lastGround.left < (destRec.x)) &&
			(lastGround.right > (destRec.x)) &&
			(lastGround.top < (destRec.y + destRec.width + 1)) &&
			(lastGround.bot > (destRec.y + destRec.width + 1))
			) ||
		(
			(lastGround.left < (destRec.x + destRec.width)) &&
			(lastGround.right > (destRec.x + destRec.width)) &&
			(lastGround.top < (destRec.y + destRec.width + 1)) &&
			(lastGround.bot > (destRec.y + destRec.width + 1))
			)
		)
		) {
		enElAire = true;
		bend = true;
	}
}

bool tileAgua::colisionBub(Rectangle bub) {
	if (
		(
			//Comprobamos colision esquina inferior derecha
			(((destRec.y + destRec.height) >= (bub.y + bub.height / 2)) &&
				((bub.y + bub.height / 2) >= (destRec.y))
				) && (
					((destRec.x + destRec.width) > (bub.x + bub.width / 2)) &&
					((bub.x + bub.width / 2) > (destRec.x))
					)
			) ||
		(
			//Comprobamos colision esquina superior derecha
			(((destRec.y + destRec.height) >= (bub.y - bub.height / 2)) &&
				((bub.y - bub.height / 2) >= (destRec.y))
				) && (
					((destRec.x + destRec.width) > (bub.x + bub.width / 2)) &&
					((bub.x + bub.width / 2) > (destRec.x))
					)
			) ||
		(
			//Comprobamos colision esquina superior izquierda
			(((destRec.y + destRec.height) >= (bub.y - bub.height / 2)) &&
				((bub.y - bub.height / 2) >= (destRec.y))
				) && (
					((destRec.x + destRec.width) > (bub.x - bub.width / 2)) &&
					((bub.x - bub.width / 2) > (destRec.x))
					)
			) ||
		(
			//Comprobamos colision esquina inferior izquierda
			(((destRec.y + destRec.height) >= (bub.y + bub.height / 2)) &&
				((bub.y + bub.height / 2) >= (destRec.y))
				) && (
					((destRec.x + destRec.width) > (bub.x - bub.width / 2)) &&
					((bub.x - bub.width / 2) > (destRec.x))
					)
			)
		) {
		return true;
	}
	else {
		return false;
	}
}

bool tileAgua::colisionEnemigo(Enemigo& e) {
	if (
		(
			//Comprobamos colision esquina inferior derecha
			(((destRec.y + destRec.height) >= (e.destRec.y + e.destRec.height / 2)) &&
				((e.destRec.y + e.destRec.height / 2) >= (destRec.y))
				) && (
					((destRec.x + destRec.width) > (e.destRec.x + e.destRec.width / 2)) &&
					((e.destRec.x + e.destRec.width / 2) > (destRec.x))
					)
			) ||
		(
			//Comprobamos colision esquina superior derecha
			(((destRec.y + destRec.height) >= (e.destRec.y - e.destRec.height / 2)) &&
				((e.destRec.y - e.destRec.height / 2) >= (destRec.y))
				) && (
					((destRec.x + destRec.width) > (e.destRec.x + e.destRec.width / 2)) &&
					((e.destRec.x + e.destRec.width / 2) > (destRec.x))
					)
			) ||
		(
			//Comprobamos colision esquina superior izquierda
			(((destRec.y + destRec.height) >= (e.destRec.y - e.destRec.height / 2)) &&
				((e.destRec.y - e.destRec.height / 2) >= (destRec.y))
				) && (
					((destRec.x + destRec.width) > (e.destRec.x - e.destRec.width / 2)) &&
					((e.destRec.x - e.destRec.width / 2) > (destRec.x))
					)
			) ||
		(
			//Comprobamos colision esquina inferior izquierda
			(((destRec.y + destRec.height) >= (e.destRec.y + e.destRec.height / 2)) &&
				((e.destRec.y + e.destRec.height / 2) >= (destRec.y))
				) && (
					((destRec.x + destRec.width) > (e.destRec.x - e.destRec.width / 2)) &&
					((e.destRec.x - e.destRec.width / 2) > (destRec.x))
					)
			)
		) {
		e.muerto = true;
		e.muertePorAgua = true;
		return true;
	}
	else {
		return false;
	}
}


void tileAgua::Dibujar(int pos) {
	if (pos == 0) {
		if (enElAire) {
			srcRec.x = 8;
		}
		else {
			if (direccionDerecha) {
				srcRec.x = 24;
			}
			else {
				srcRec.x = 16;
			}
		}
	}
	else if (pos == 9) {
		if (enElAire) {
			srcRec.x = 0;
		}
		else {
			if (direccionDerecha) {
				srcRec.x = 16;
			}
			else {
				srcRec.x = 24;
			}
		}
	}
	else {
		if (bend) {
			if (direccionDerecha) {
				if (enElAire) {
					srcRec.x = 48;
				}
				else {
					srcRec.x = 32;
				}
			}
			else {
				if (enElAire) {
					srcRec.x = 56;
				}
				else {
					srcRec.x = 40;
				}
			}
		}
		else {
			if (enElAire) {
				srcRec.x = 72;
			}
			else {
				srcRec.x = 64;
			}
		}
	}
	DrawTexturePro(spritesheet, srcRec, destRec, origin, 0.0f, WHITE); //de donde está sacando origin
}