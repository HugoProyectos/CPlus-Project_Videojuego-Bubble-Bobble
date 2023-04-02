#pragma once
#include "Sprite.hpp"
#include "../AdministradorPompas.cpp"
#include <string>
#include <cmath>
#include <iostream> //DEBUG


class tileAgua : public Sprite {

    int widthAnimation = 16;
    int heightAnimation = 16;
public:

};

class Agua : public Sprite {

public:
    // Referencia al administrador de pompas
    AdministradorPompas* admin;

    Agua() = default; //Debe llamarse a Inicializador
    Agua(Rectangle destino, AdministradorPompas& adm, bool right);
};