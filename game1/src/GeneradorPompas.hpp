#pragma once
#include "clasesConSprite/Pompa.hpp"
#include <vector>

class GeneradorPompas {
	uint8_t idMapa = 0;
	int contadorCiclos = 0;
	int ciclosPompa[10] = { 0, 60 * 5, 0, 0, 0, 0, 0, 0, 0, 0 }; //Cada posición es un mapa (60->iteraciones por segundo. n*60 = tras n segundos)
	const Texture2D verde = LoadTexture("resources/Players/Bobblun/pompa.png");
	const Texture2D azul = LoadTexture("resources/Players/Bobblun/pompa2.png");
public:
	
	

	GeneradorPompas() = default;
	GeneradorPompas(uint8_t idMapa);

	void cambiarModo(uint8_t idMapa);

	std::vector<sh_Pompa> generarPompa();

};