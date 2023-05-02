#pragma once
#include "Sprite.hpp"
#include <memory>

class Rayo : public Sprite{
	const int widthAnimation = 16;
	const int heightAnimation = 16;

	const int fHit = 6;
	const int fMove = 3;

public:
	int lastHeight = GetScreenHeight();
	int lastWidth = GetScreenWidth();
	static const int DANYO_RAYO = 2;

	double ratioX = 1;

	//Desplazamiento
	float velocidadDesplazamiento = 6;
	bool izquierda = true;

	//A quien dar puntos
	bool soyDeBub = true;

	//Control de animaciones
	int animacionActiva;
	int indiceAnimacion = 0;
	int cuentaFrames = 0;
	int velocidadFrames = 0;
	bool borrame = false;
	bool golpeJefe = false;


	Rayo() = default;
	Rayo(Texture2D spriteSheet, const Rectangle origen, bool izquierda);

	void Inicializador(Texture2D spriteSheet, const Rectangle origen, bool izquierda);

	void Actualizar(DatosJugador& j1, DatosJugador& j2);

	void Dibujar();
};

typedef std::shared_ptr<Rayo> sh_Rayo;