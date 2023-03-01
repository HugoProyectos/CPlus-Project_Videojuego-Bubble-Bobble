#pragma once
#include "Sprite.hpp"
#include "Suelo.cpp"
#include <iostream>

//Se ha decidido que tengan sprites distintos para cada animación,
//La estructura difiere del modelo original genérico de personaje por ello.
class Bub : public Sprite {
public:
    Texture2D standingAnimation = LoadTexture("resources/Players/Bobblun/standing.png");
    int fStandingAnimation = 2; //Número de fotogramas de la StandingAnimation
    int widthAnimation;
    int heightAnimation;
    
    int animacionActiva = 0; //0->StandingAnimation
    int targetFrames;
    
    int indiceAnimacion = 0;
    int cuentaFrames = 0;
    int velocidadFrames = 2;
    bool enElAire = false;
    bool cayendo = false;
    float saltoRecorrido = 0;
    float distanciaSaltoMax = 0;
    float velocidadSalto = 0;
    float velocidadLateral = 0;

    Bub() = default; //Debe llamarsse a Inicializador

    Bub(float tamano, float saltoMax, float velSalto, float velLateral, int _targetFrames) {
        Inicializador(tamano, saltoMax, velSalto, velLateral, _targetFrames);
    };
    void Inicializador(float tamano, float saltoMax, float velSalto, float velLateral, int _targetFrames)
    {
        widthAnimation = standingAnimation.width / fStandingAnimation;
        heightAnimation = standingAnimation.height;
        targetFrames = _targetFrames;

        std::cout << "Anchura y altura de un fotograma" << std::endl;
        std::cout << widthAnimation << std::endl;
        std::cout << heightAnimation << std::endl;

        distanciaSaltoMax = saltoMax;//60.0f;
        velocidadSalto = velSalto;
        velocidadLateral = velLateral;

        // Source rectangle (part of the texture to use for drawing)
        srcRec = { 0.0f, 0.0f, (float)heightAnimation, (float)widthAnimation };
        this->tamano = tamano;

        // Destination rectangle (screen rectangle where drawing part of texture)
        destRec = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f, (float)widthAnimation * tamano, (float)heightAnimation * tamano }; //Dos primeros, ubicacion. Dos ultimos, dimensiones

        // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
        origin = { (float)widthAnimation * tamano / 2, (float)heightAnimation * tamano / 2 }; //En principio no lo necesitamos
    }

    ~Bub() {
        UnloadTexture(sprite);
    };

    void Unload() {
        UnloadTexture(sprite);
    };

    void Actualizar() {
        //Gestión de desplazamiento lateral
        if (IsKeyDown(KEY_A)) {
            std::cout << "Muevo Izquierda" << std::endl;
            destRec.x -= velocidadLateral;
        }
        else if (IsKeyDown(KEY_S)) {
            destRec.x += velocidadLateral;
            std::cout << "Muevo Derecha" << std::endl;
        }

        //Gestión de salto
        if (IsKeyPressed(KEY_SPACE) && !enElAire) {
            std::cout << "Salto" << std::endl;
            enElAire = true;
            destRec.y -= velocidadSalto;
            saltoRecorrido += velocidadSalto;
        }
        else if (saltoRecorrido < distanciaSaltoMax && enElAire && !cayendo) {
            destRec.y -= velocidadSalto;
            saltoRecorrido += velocidadSalto;
        }
        else if (enElAire && cayendo && saltoRecorrido > 0) {
            destRec.y += velocidadSalto;
            saltoRecorrido -= velocidadSalto;
        }
        else if (enElAire) {
            saltoRecorrido = 0.0f;
            cayendo = true;
            destRec.y += velocidadSalto / 2; //planeo
        }

        //Actualizar puntero de animacion
        cuentaFrames++;
        if (cuentaFrames >= (targetFrames / velocidadFrames) ) {
            std::cerr << "Actualiza" << std::endl;
            cuentaFrames = 0;
            switch (animacionActiva) {
            case 0:
                indiceAnimacion = (indiceAnimacion + 1) % fStandingAnimation;
                break;
            default:
                break;
            }
        }
    };

    void Dibujar() {
        srcRec.x = (float)widthAnimation * (float)indiceAnimacion;
        //std::cerr << "Indice Animacion: " << indiceAnimacion << std::endl;
        DrawTexturePro(standingAnimation, srcRec, destRec, origin, 0.0f, WHITE);
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