#pragma once
#include "Sprite.hpp"
#include "mapa.cpp"
#include <iostream> //DEBUG

class tileAgua : public Sprite {

    //Variables animacion
    int widthAnimation = 16;
    int heightAnimation = 16;
    Texture2D spritesheet;
    Rectangle srcRect = { 0,0,8,8 };
    Rectangle destRect = { -1,-1,-1,-1 };

    //Variables fisica
    bool direccionDerecha = true;
    bool enElAire = true;
    int lastGroundNum = 0; //Incializar a -1 da error

    //Variables optimizacion
    int numPlataformas = -1;

public:

    tileAgua() = default;
    tileAgua(Rectangle destino, bool right, Texture2D sprite, int numPlataformas);
    
    Rectangle ActualizarHead(Plataformas& plat);
    Rectangle Actualizar(Rectangle pos);

    bool comprobarColisionY(Plataforma& plat);
    bool comprobarColisionX(Plataforma& plat);
    void comprobarColisionParedes(Columnas& col);
    void comprobarSuelo(Plataforma& lastGround);
    
    void Dibujar();
};