#pragma once
#include "Sprite.hpp"
#include "mapa.cpp"
#include "Bub.cpp"
#include <iostream> //DEBUG

struct Response {
    Rectangle rect;
    bool enElAire;
    bool derecha;
    bool bend;
};

class tileAgua : public Sprite {



    //Variables animacion
    int width = 8;
    int height = 8;
    int bend = 1; //0 = No_bend 1=Right_bend 2=Left_bend
    Texture2D spritesheet = Texture2D();
    //Rectangle srcRect = { 8.0f,0.0f,(float)height, (float)width }; //Se ha pasado la asignación a srcRec en el constructor


    //Variables fisica
    bool direccionDerecha = true;
    bool enElAire = true;
    int lastGroundNum = 0; //Incializar a -1 da error

    //Variables optimizacion
    int numPlataformas = -1;

public:

    Rectangle destRect = { -1,-1,-1,-1 };

    tileAgua() = default;
    tileAgua(Rectangle destino, bool right, Texture2D sprite, int numPlataformas);
    
    Response ActualizarHead(Plataformas& plat);
    Response Actualizar(Response input);

    bool comprobarColisionY(Plataforma& plat);
    bool comprobarColisionX(Plataforma& plat);
    void comprobarColisionParedes(Columnas& col);
    void comprobarSuelo(Plataforma& lastGround);
    bool colisionBub(Bub& bub);

    void Dibujar(int pos);
};