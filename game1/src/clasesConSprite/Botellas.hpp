#pragma once
#include "Enemigo.cpp"
#include <ctime>

class Botellas : public Enemigo {

	// Render --------------------------------------------------

	int8_t cambioMapa = 2;
	Rectangle posicionPartida =
	{
		(float)GetScreenWidth() / 2,
		(float)50, 32, 32
	};
	int cuentaFramesTraslacion = 0;
	const int LIMITE_FRAMES_TRASLACION = 180;
	double razonX = 0;
	double razonY = 0;

	float _targetFPS;
	int lastHeight = 450;
	int lastWidth = 800;
	Rectangle destino;

	// --------------------------------------------------------
	// Animacion ----------------------------------------------

	int pixels = 16;
	Texture2D walk = LoadTexture("resources/enemyBotellas/walk.png");
	int fWalk = 4;

	int widthAnimation; // Se actualiza para cada animación activa
	int heightAnimation;
	Texture2D animations[1] = { walk };
	int fAnimation[2] = { fWalk };
	int ID;


	int animacionActiva = 0; 
	int indiceAnimacion = 0; 

	int targetFrames;
	int cuentaFrames = 0;
	int velocidadFrames = 2;


	// --------------------------------------------------------
	// Colisiones ---------------------------------------------

	Plataforma lastGround;
	bool enElAire = true;
	bool cayendo = true;

	// --------------------------------------------------------
	// Logica -------------------------------------------------

	int direccionX;
	int direccionY;
	int ID;

	// --------------------------------------------------------

	Botellas() = default;
	Botellas(float tamano, float velSalto, float velLateral, float _targetFPS, Rectangle destino, int ID, int direccion);
	void Actualizar();
	void Dibujar();
	void MoverIzq();
	void MoverDer();
	void compruebaColision(Plataforma& s, int enemyNum);
	void compruebaSuelo();
	void compruebaPared(const Columnas& s);

};

typedef std::shared_ptr<Botellas> sh_Botellas;