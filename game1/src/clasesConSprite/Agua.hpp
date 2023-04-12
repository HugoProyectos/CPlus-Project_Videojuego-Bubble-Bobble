#pragma once
#include "TileAgua.hpp"
#include "Bub.cpp"
#include "../AdministradorPompas.cpp"
#include <iostream> //DEBUG

class Agua : public Sprite {

public:

    tileAgua stream[10];
    bool existe = false;
    int bubTile;
    Agua() = default;
    Agua(Rectangle destino, bool right, Texture2D sprite, int numPlat);

    void Actualizar(Plataformas& plat,Columnas& col);
    void colisionBub(Bub& bub);

    void Dibujar();
};