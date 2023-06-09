﻿#include "generadorPompas.hpp"
#include <stdlib.h>
#include <time.h>

GeneradorPompas::GeneradorPompas(uint8_t idMapa) {
	this->idMapa = idMapa;
	srand(time(NULL));
}

void GeneradorPompas::cambiarModo(uint8_t idMapa) {
	//std::cout << "Cambiando al mapa " << (int)idMapa << std::endl;
	this->idMapa = idMapa;
	Pompa::ID_MAPA = idMapa;
	contadorCiclos = 0;
}

std::vector<sh_Pompa> GeneradorPompas::generarPompa() {
	std::vector<sh_Pompa> resultado;
	//std::vector<Pompa> pompas;
	contadorCiclos++;
	if (contadorCiclos >= ciclosPompa[idMapa]) {
		//std::cout << "Chekeo de generaci�n de pompas" << std::endl;
		contadorCiclos = 0;
		switch (idMapa) {
		case 2:
		{
			Pompa p;
			Rectangle r[2] = { { GetScreenWidth() * 0.3125, GetScreenHeight() * 1.0711, 32, 32}, { GetScreenWidth() * 0.6875, GetScreenHeight() * 1.0711, 32, 32 } };
			//resultado.push_back(std::make_shared<Pompa>(p1));
			//resultado.push_back(std::make_shared<Pompa>(p2));
			int numRand = rand() % 100;
			int numRand2 = rand() % 2;
			//Selección del color de la pompa
			if (numRand < 50) { //verde
				p = Pompa(verde, r[numRand2], 0, 0, false, 60 * 60);
			}
			else { //azul
				p = Pompa(azul, r[numRand2], 0, 0, false, 60 * 60);
			}

			if (lastHeight != GetScreenHeight()) {
				p.destRec.height = GetScreenHeight() / 14.0625f;
				p.origin.y = p.destRec.height / 2;
			}
			if (lastWidth != GetScreenWidth()) {
				p.destRec.width = GetScreenWidth() / 25.0f;
				p.origin.x = p.destRec.width / 2;
			}
			resultado.push_back(std::make_shared<Pompa>(p));

			break;
		}
		case 3:
		{
			Pompa p;
			Rectangle r[2] = { { GetScreenWidth() * 0.3125, GetScreenHeight() * 1.0711, 32, 32}, { GetScreenWidth() * 0.6875, GetScreenHeight() * 1.0711, 32, 32 } };
			//resultado.push_back(std::make_shared<Pompa>(p1));
			//resultado.push_back(std::make_shared<Pompa>(p2));
			int numRand = rand() % 100;
			int numRand2 = rand() % 2;
			//Selección del color de la pompa
			if (numRand < 50) { //verde
				p = Pompa(verde, r[numRand2], 0, 0, false, 60 * 120);
			}
			else { //azul
				p = Pompa(azul, r[numRand2], 0, 0, false, 60 * 120);
			}
			if (lastHeight != GetScreenHeight()) {
				p.destRec.height = GetScreenHeight() / 14.0625f;
				p.origin.y = p.destRec.height / 2;
			}
			if (lastWidth != GetScreenWidth()) {
				p.destRec.width = GetScreenWidth() / 25.0f;
				p.origin.x = p.destRec.width / 2;
			}
			resultado.push_back(std::make_shared<Pompa>(p));

			break;
		}
		case 4: //Mapa del agua
		{
			Pompa p;
			Rectangle r[2] = { { GetScreenWidth() * 0.3125, -(GetScreenHeight() * 0.0711), 32, 32}, { GetScreenWidth() * 0.6875, -(GetScreenHeight() * 0.0711), 32, 32 } };
			//resultado.push_back(std::make_shared<Pompa>(p1));
			//resultado.push_back(std::make_shared<Pompa>(p2));
			int numRand = rand() % 100;
			//for (int i = 0; i < 2; i++) {
			int i = rand() % 2;
			//Selección del color de la pompa
			if (numRand < 50) { //verde
				p = Pompa(verde, r[i], 0, 0, false, 60 * 120);
			}
			else { //azul
				p = Pompa(azul, r[i], 0, 0, false, 60 * 120);
			}
			numRand = rand() % 100;
			if (numRand < 50) { //Que contenga agua
				p.modulo = Pompa::MODULO_AGUA;
				p.tVida = Pompa::INFINITA;
				p.indiceAnimacion = 6;
			}
			if (lastHeight != GetScreenHeight()) {
				p.destRec.height = GetScreenHeight() / 14.0625f;
				p.origin.y = p.destRec.height / 2;
				/*p.destRec.y = GetScreenHeight() * (p.destRec.y / lastHeight);
				*/
				//p.lastHeight = lastHeight;
			}
			if (lastWidth != GetScreenWidth()) {
				p.destRec.width = GetScreenWidth() / 25.0f;
				p.origin.x = p.destRec.width / 2;
				/*p.destRec.x = GetScreenWidth() * (p.destRec.x / lastWidth);
				*/
				//p.lastWidth = lastWidth;
			}
			resultado.push_back(std::make_shared<Pompa>(p));
			break;
		}
		case 5: //Mapa del agua
		{
			Pompa p;
			Rectangle r[2] = { { GetScreenWidth() * 0.3125, -(GetScreenHeight() * 0.0711), 32, 32}, { GetScreenWidth() * 0.6875, -(GetScreenHeight() * 0.0711), 32, 32 } };
			//resultado.push_back(std::make_shared<Pompa>(p1));
			//resultado.push_back(std::make_shared<Pompa>(p2));
			int numRand = rand() % 100;
			//for (int i = 0; i < 2; i++) {
			int i = rand() % 2;
			//Selección del color de la pompa
			if (numRand < 50) { //verde
				p = Pompa(verde, r[i], 0, 0, false, 60 * 120);
			}
			else { //azul
				p = Pompa(azul, r[i], 0, 0, false, 60 * 120);
			}
			numRand = rand() % 100;
			if (numRand < 50) { //Que contenga agua
				p.modulo = Pompa::MODULO_AGUA;
				p.tVida = Pompa::INFINITA;
				p.indiceAnimacion = 6;
			}
			if (lastHeight != GetScreenHeight()) {
				p.destRec.height = GetScreenHeight() / 14.0625f;
				p.origin.y = p.destRec.height / 2;
			}
			if (lastWidth != GetScreenWidth()) {
				p.destRec.width = GetScreenWidth() / 25.0f;
				p.origin.x = p.destRec.width / 2;
			}
			resultado.push_back(std::make_shared<Pompa>(p));
			break;
		}
		case 6:
		{
			Pompa p;
			Rectangle r[2] = { { GetScreenWidth() * 0.35, -(GetScreenHeight() * 0.0711), 32, 32}, { GetScreenWidth() * 0.65, -(GetScreenHeight() * 0.0711), 32, 32 } };
			int numRand = rand() % 100;
			//for (int i = 0; i < 2; i++) {
			int i = rand() % 2;
			//Selección del color de la pompa
			if (numRand < 50) { //verde
				p = Pompa(verde, r[i], 0, 0, false, 60 * 120);
			}
			else { //azul
				p = Pompa(azul, r[i], 0, 0, false, 60 * 120);
			}
			if (lastHeight != GetScreenHeight()) {
				p.destRec.height = GetScreenHeight() / 14.0625f;
				p.origin.y = p.destRec.height / 2;
			}
			if (lastWidth != GetScreenWidth()) {
				p.destRec.width = GetScreenWidth() / 25.0f;
				p.origin.x = p.destRec.width / 2;
			}
			resultado.push_back(std::make_shared<Pompa>(p));
			break;
		}case 7: //Mapa del agua
		{
			Pompa p;
			Rectangle r[2] = { { GetScreenWidth() * 0.35, -(GetScreenHeight() * 0.0711), 32, 32}, { GetScreenWidth() * 0.65, -(GetScreenHeight() * 0.0711), 32, 32 } };
			//resultado.push_back(std::make_shared<Pompa>(p1));
			//resultado.push_back(std::make_shared<Pompa>(p2));
			int numRand = rand() % 100;
			//for (int i = 0; i < 2; i++) {
			int i = rand() % 2;
			//Selección del color de la pompa
			if (numRand < 50) { //verde
				p = Pompa(verde, r[i], 0, 0, false, 60 * 120);
			}
			else { //azul
				p = Pompa(azul, r[i], 0, 0, false, 60 * 120);
			}
			numRand = rand() % 100;
			if (numRand < 50) { //Que contenga agua
				p.modulo = Pompa::MODULO_AGUA;
				p.tVida = Pompa::INFINITA;
				p.indiceAnimacion = 6;
			}
			if (lastHeight != GetScreenHeight()) {
				p.destRec.height = GetScreenHeight() / 14.0625f;
				p.origin.y = p.destRec.height / 2;
			}
			if (lastWidth != GetScreenWidth()) {
				p.destRec.width = GetScreenWidth() / 25.0f;
				p.origin.x = p.destRec.width / 2;
			}
			resultado.push_back(std::make_shared<Pompa>(p));
			break;
		}
		case 8: //Mapa del rayo
		{
			Pompa p;
			Rectangle r[2] = { { GetScreenWidth() * 0.35, -(GetScreenHeight() * 0.0711), 32, 32}, { GetScreenWidth() * 0.65, -(GetScreenHeight() * 0.0711), 32, 32 } };
			//resultado.push_back(std::make_shared<Pompa>(p1));
			//resultado.push_back(std::make_shared<Pompa>(p2));
			int numRand = rand() % 100;
			//for (int i = 0; i < 2; i++) {
			int i = rand() % 2;
			//Selección del color de la pompa
			if (numRand < 50) { //verde
				p = Pompa(verde, r[i], 0, 0, false, 60 * 120);
			}
			else { //azul
				p = Pompa(azul, r[i], 0, 0, false, 60 * 120);
			}
			numRand = rand() % 100;
			if (numRand < 50) { //Que contenga un rayo
				p.modulo = Pompa::MODULO_RAYO;
				p.tVida = Pompa::INFINITA; 
				p.indiceAnimacion = 18;
			}
			if (lastHeight != GetScreenHeight()) {
				p.destRec.height = GetScreenHeight() / 14.0625f;
				p.origin.y = p.destRec.height / 2;
			}
			if (lastWidth != GetScreenWidth()) {
				p.destRec.width = GetScreenWidth() / 25.0f;
				p.origin.x = p.destRec.width / 2;
			}
			resultado.push_back(std::make_shared<Pompa>(p));
			break;
		}
		case 9: //Mapa del rayo 2
		{
			Pompa p;
			Rectangle r[2] = { { GetScreenWidth() * 0.35, GetScreenHeight() * 1.0711, 32, 32}, { GetScreenWidth() * 0.65, GetScreenHeight() * 1.0711, 32, 32 } };
			//resultado.push_back(std::make_shared<Pompa>(p1));
			//resultado.push_back(std::make_shared<Pompa>(p2));
			int numRand = rand() % 100;
			//for (int i = 0; i < 2; i++) {
			int i = rand() % 2;
			//Selección del color de la pompa
			if (numRand < 50) { //verde
				p = Pompa(verde, r[i], 0, 0, false, 60 * 120);
			}
			else { //azul
				p = Pompa(azul, r[i], 0, 0, false, 60 * 120);
			}
			p.modulo = Pompa::MODULO_RAYO;
			p.indiceAnimacion = 18;
			p.tVida = Pompa::INFINITA;
			if (lastHeight != GetScreenHeight()) {
				p.destRec.height = GetScreenHeight() / 14.0625f;
				p.origin.y = p.destRec.height / 2;
			}
			if (lastWidth != GetScreenWidth()) {
				p.destRec.width = GetScreenWidth() / 25.0f;
				p.origin.x = p.destRec.width / 2;
			}
			resultado.push_back(std::make_shared<Pompa>(p));
			break;
		}
		default:
			break;
		}
	}
	return resultado;
}