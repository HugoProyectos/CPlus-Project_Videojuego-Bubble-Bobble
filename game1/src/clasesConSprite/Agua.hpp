#pragma once
#include "TileAgua.hpp"
#include <iostream> //DEBUG

class Agua : public Sprite {

public:

    tileAgua stream[10];
    bool existe = false;
    int bubTile;
    Agua() = default;
    Agua(Rectangle destino, bool right, Texture2D sprite, int numPlat);

    void Actualizar(Plataformas& plat,Columnas& col);
    bool colisionBub(Rectangle bub, int framesInmunidad);

    void Dibujar();
};