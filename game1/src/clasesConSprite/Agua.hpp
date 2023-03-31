#pragma once
#include "Sprite.hpp"
#include "../AdministradorPompas.cpp"
#include <string>
#include <cmath>
#include <iostream> //DEBUG


class Agua : public Sprite {

    int widthAnimation = 16;
    int heightAnimation = 16;

    // Referencia al administrador de pompas
    AdministradorPompas* admin;
    
    Agua() = default; //Debe llamarse a Inicializador
    Agua(float tamano, float saltoMax, float velSalto, float velLateral, int _targetFrames, Rectangle destino, AdministradorPompas& adm);
    void Inicializador(float tamano, float saltoMax, float velSalto, float velLateral, int _targetFrames, AdministradorPompas& adm);
}