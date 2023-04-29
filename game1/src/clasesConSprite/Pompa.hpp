#pragma once
#include "Sprite.hpp"
#include <string>
#include <cmath>
#include "../Controls.cpp"
#include "Enemigo.cpp"
#include "Scores.cpp"
#include <iostream> //DEBUG

//Valores de módulo acorde a lo que lleva dentro la pompa
//1->ROBOT
//2->AGUA

// Aquí debajo se pondrán las constantes para especificar 
// las animaciones de enemigos atrapados en pompas
const int ROBOT = 5;
const int FANTASMA = 8;

/* Tipo de dato que representa las burbujas que disparan Bub y Bob.
 * A continuación, la lista de animaciones que se contienen en su variable
 * <sprite> junto al número de fotogramas que tiene:
 * 0 -> Disparo -> 7
 * 1 -> Verde	-> 3
 * 2 -> Naranja -> 3
 * 3 -> Rojo	-> 3
 * 4 -> Explota -> 10
 * 5,6,7 -> Pompas verde-naranja-rojo (3-3-3) de un enemigo (así con todos)
 */
class Pompa : public Sprite {

	int contadorFrames = -1;
	const int maxFrames = 10;

	//Índices de color
	const int VERDE = 0;
	const int NARANJA = 1;
	const int ROJO = 2;


	const int widthAnimation = 16;
	const int heightAnimation = 16;

	const int fDisparo = 7;
	const int fExplota = 10;
	const int fAtrapado = 3;

	//Razón a la que se va restando la vida a la pompa
	const int VELOCIDAD_MUERTE = 1;

	//Modo oscilacion
	bool oscilando = false;
	bool abajo = true;
	bool ignorar = false;
	uint8_t contador = 0;

	//Abstracción de generación de enemigo al explotar
	sh_Enemigo extraeEnemigo(bool matalo);

public:
	int lastHeight = GetScreenHeight();
	int lastWidth = GetScreenWidth();

	double ratioX = 1;
	double ratioY = 1;

	//Controles del jugador
	static Controls controlesJugador;

	//Para las pompas con agua
	static const int INFINITA = -25;
	static const int MODULO_AGUA = 2;
	static const int MODULO_RAYO = 6;
	bool izquierdaRayo = true;
	int sentidoJugador = 1;
	bool rayoDeBub = true;

	//Indice de animación básicos
	static const int DISPARO = 0;
	static const int VACIA = 1;
	static const int EXPLOTA = 4;
	static const int NUM_FOTOGRAMAS = 3;

	static uint8_t ID_MAPA;	//Debe actualizarse cada vez que se cambie el mapa
	static uint32_t ID_GLOBAL;
	int ID = -1;
	static void init();

	//Vida de la pompa (asociado a naranja-rojo)
	int vidaMaxima = 0;
	int tVida = 0;
	bool matame = false;

	//Indica si inicia cadena de explosiones y que jugador ha iniciado la cadena
	int cadena = 0; // 0 = NO CADENA, 1 = BUB, 2 = BOB
	//Indica numero de enemigos muertos en la cadena
	int killCount = 0;
	//Enemigo contenido
	sh_Enemigo enemigoContenido = NULL;
	int modulo = 0;

	//Desplazamiento
	float velocidadDesplazamiento = 0;
	int sentidoLateral = 1; //1-> Sin sentido, 2 -> hacia la izquierda, 3->hacia la derecha
	//Relacionado con el modo disparo
	float distanciaRecorrida = 0;
	float distanciaDisparo = 0;
	int disparada = 2;
	static bool warpVertical;

	//Control de animaciones
	int animacionActiva;
	int indiceAnimacion = 0;
	int indiceExtra = 0;
	int cuentaFrames = 0;
	int velocidadFrames = 0;


	Pompa() = default;
	Pompa(Texture2D spriteSheet, const Rectangle origen, float velDisparo, float distanciaDisparo, bool esDisparada, int tiempoVida);

	bool operator==(const Pompa& p);

	void Inicializador(Texture2D spriteSheet, const Rectangle origen, float velDisparo, float distanciaDisparo, bool esDisparada, int tiempoVida);

	sh_Enemigo Actualizar(DatosJugador& j1, DatosJugador& j2, uint8_t& creaAgua, Columnas* col, Plataformas* plat, Scores* scr); //3->Derecha, 2->Izquierda

	Pompa& operator=(const Pompa& p);
	void explota();

	void Dibujar();
};

typedef std::shared_ptr<Pompa> sh_Pompa;