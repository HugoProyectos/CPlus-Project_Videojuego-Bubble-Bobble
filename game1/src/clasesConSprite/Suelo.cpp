#pragma once
#include "raylib.h"
#include "sprite.hpp"
#include <string>

class Suelo : public Sprite {
    
public:

    int aproach;

    Suelo() = default; //Debe llamarsse a Inicializador

    Suelo(std::string rutaTextura) {
        Inicializador(rutaTextura);
    };
    void Inicializador(std::string rutaTextura)
    {
        sprite = LoadTexture(rutaTextura.c_str());// Texture loading

        frameWidth = sprite.width;
        frameHeight = sprite.height;

        // Source rectangle (part of the texture to use for drawing)
        srcRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };

        Inicializador();
    }
    void Inicializador() {
        tamano = 1;

        // Destination rectangle (screen rectangle where drawing part of texture)
        // Destination rectangle (screen rectangle where drawing part of texture)
        destRec = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f, (float)frameWidth, (float)frameHeight }; //Dos primeros, ubicacion. Dos ultimos, dimensiones

        // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
        origin = { (float)frameWidth * tamano / 2, (float)frameHeight * tamano / 2 };
        //Vector2 origin = { 0,0 };
    };

    ~Suelo() {
        UnloadTexture(sprite);
    };

    void UnloadAsteroide() {
        UnloadTexture(sprite);
    };

    void Actualizar() {

    };

    void Dibujar() {
        DrawTexturePro(sprite, srcRec, destRec, origin, 0.0f, WHITE);
    }
};