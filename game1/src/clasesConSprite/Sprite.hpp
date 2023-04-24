#pragma once
#include "raylib.h"
#include <string>

//Clase que se puede utilizar como clase padre de Personaje y de Suelo
class Sprite {
public:
    Texture2D sprite;
    int frameWidth = 0;
    int frameHeight = 0;

    Rectangle srcRec, destRec;
    Vector2 origin;
    float tamano;

	//const double ratioAspecto = 800 / 450; //Para medir qué tamaño debe tener en realidad la pantalla dentro de la ventana
};

//Tipo de dato que utilizan muchas de las clases con Sprite
struct DatosJugador {
	Rectangle posicionJugador = { -1,-1,-1,-1 };
	bool jugadorCayendo = false;
	int sentidoJugador = 2; //3->mira a la derecha 2->mira a la izquierda: variable orientacionActual de Bub
	bool muriendo = false;
	int debeRebotar = 0; //0->No
	int velLateral = 0;
};