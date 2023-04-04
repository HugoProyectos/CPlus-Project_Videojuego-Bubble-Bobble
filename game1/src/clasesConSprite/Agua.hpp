#pragma once
#include "TileAgua.hpp"
#include "../AdministradorPompas.cpp"
#include <iostream> //DEBUG

class Agua : public Sprite {

    tileAgua stream[10];
    bool existe = false;

public:

    Agua(Rectangle destino, bool right, Texture2D sprite, int numPlat);

    void Actualizar(Plataformas& plat,Columnas& col);

    void Dibujar();
};