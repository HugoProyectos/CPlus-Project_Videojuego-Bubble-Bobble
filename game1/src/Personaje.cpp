#pragma once
#include "Sprite.hpp"
#include "Suelo.cpp"
#include <iostream>

class Personaje : public Sprite {
public:
    bool enElAire = false;
    bool cayendo = false;
    float saltoRecorrido = 0;
    float distanciaSaltoMax = 0;
    float velocidadSalto = 0;
    float velocidadLateral = 0;

    Personaje() = default; //Debe llamarsse a Inicializador

    Personaje(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral) {
        Inicializador(rutaTextura, tamano, saltoMax, velSalto, velLateral);
    };
    void Inicializador(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral)
    {
        sprite = LoadTexture(rutaTextura.c_str());// Texture loading

        distanciaSaltoMax = saltoMax;//60.0f;
        velocidadSalto = velSalto;
        velocidadLateral = velLateral;

        frameWidth = sprite.width;
        frameHeight = sprite.height;

        // Source rectangle (part of the texture to use for drawing)
        srcRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
        this->tamano = tamano;

        // Destination rectangle (screen rectangle where drawing part of texture)
        destRec = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f, (float)frameWidth * tamano, (float)frameHeight * tamano }; //Dos primeros, ubicacion. Dos ultimos, dimensiones

        // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
        origin = { (float)frameWidth * tamano / 2, (float)frameHeight * tamano / 2 }; //En principio no lo necesitamos
    }
 
    ~Personaje() {
        UnloadTexture(sprite);
    };

    void Unload() {
        UnloadTexture(sprite);
    };

    void Actualizar() {
        //Gestión de salto
        if (IsKeyPressed(KEY_SPACE) && !enElAire) {
            std::cout << "Salto" << std::endl;
            enElAire = true;
            destRec.y -= velocidadSalto;
            saltoRecorrido += velocidadSalto;
        } else if (saltoRecorrido < distanciaSaltoMax && enElAire && !cayendo) {
            destRec.y -= velocidadSalto;
            saltoRecorrido += velocidadSalto;
        } else if (enElAire && cayendo) {
            destRec.y += velocidadSalto;
        } else if (enElAire) {
            cayendo = true;
            destRec.y += velocidadSalto;
        }

        //Gestión de desplazamiento lateral
        if (IsKeyDown(KEY_A)) {
            std::cout << "Muevo Izquierda" << std::endl;
            destRec.x -= velocidadLateral;
        } else if (IsKeyDown(KEY_S)) {
            destRec.x += velocidadLateral;
            std::cout << "Muevo Derecha" << std::endl;
        }
    };

    void Dibujar() {
        DrawTexturePro(sprite, srcRec, destRec, origin, 0.0f, WHITE);
    }

    void compruebaColision(const Suelo& s) {
        if ((destRec.y + destRec.height / 2) > (s.destRec.y - s.destRec.height / 2)) { //Choca abajo
            destRec.y = (s.destRec.y - s.destRec.height / 2) - destRec.height / 2;
            enElAire = false;
            cayendo = false;
            saltoRecorrido = 0;
        }
    }
};