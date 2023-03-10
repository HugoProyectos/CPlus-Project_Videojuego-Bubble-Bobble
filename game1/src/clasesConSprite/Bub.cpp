#pragma once
#include "Sprite.hpp"
#include "Suelo.cpp"
#include <iostream>



//Se ha decidido que tengan sprites distintos para cada animación,
//La estructura difiere del modelo original genérico de personaje por ello.
class Bub : public Sprite {
    const int STANDING = 0;
    const int MOVING = 1;
    const int JUMPING = 2;
    const int FALLING = 3;
    const int SHOOTING = 4;
    const int DYING = 5;

public:
    Texture2D standingAnimation = LoadTexture("resources/Players/Bobblun/standing.png");
    int fStandingAnimation = 4; //Número de fotogramas de la StandingAnimation
    Texture2D movingAnimation = LoadTexture("resources/Players/Bobblun/move.png");
    int fMovingAnimation = 4; //Número de fotogramas
    Texture2D jumpingAnimation = LoadTexture("resources/Players/Bobblun/Subida.png");
    int fJumpingAnimation = 4; //Número de fotogramas
    Texture2D fallingAnimation = LoadTexture("resources/Players/Bobblun/Caida.png");
    int fFallingAnimation = 4; //Número de fotogramas
    Texture2D glidingAnimation = LoadTexture("resources/Players/Bobblun/Caida.png");
    int fGlidingAnimation = 4; //Número de fotogramas

    int widthAnimation;
    int heightAnimation;
    int switchOrientacion = 1;
    int orientacionActual = 2;
    
    int animacionActiva = 0; //0->StandingAnimation 1->MoveAnimation 2->JumpAnimation 3->FallAnimation 5->ShootingAnimation 6->DyingAninimation
    int targetFrames;
    
    int indiceAnimacion = 0;
    int cuentaFrames = 0;
    int velocidadFrames = 8;
    bool enElAire = false;
    bool cayendo = false;
    float saltoRecorrido = 0;
    float distanciaSaltoMax = 0;
    float velocidadSalto = 0;  //Añadir aceleracion, y hacer que velocidad nueva = velocidad anterior + aceleracion. Empezar con una aceleracion inicial, y que se le vayan restando valores. A partir de cierta velocidad, capar. Hay que comprobar que el tope coincida con el planeo.
    float velocidadLateral = 0;

    Bub() = default; //Debe llamarse a Inicializador

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
        if (IsKeyDown(KEY_A) || (enElAire && saltoRecorrido > 0 && switchOrientacion == 2)) {
            animacionActiva = MOVING;
            switchOrientacion = 2;
           // std::cout << "Muevo Izquierda, orientacion: " << switchOrientacion << std::endl;
            destRec.x -= velocidadLateral;
        }
        else if (IsKeyDown(KEY_S) || (enElAire && saltoRecorrido > 0 && switchOrientacion == 3)) {
            animacionActiva = MOVING;
            switchOrientacion = 3;
            destRec.x += velocidadLateral;
           // std::cout << "Muevo Derecha, orientacion: " << switchOrientacion << std::endl;
        } else {
            if (!(enElAire && saltoRecorrido == 0)) {
                switchOrientacion = 1;
            }
            animacionActiva = 0;
           // std::cout << "Dejo de moverme, orientacion: " << switchOrientacion << std::endl;
        }

        //Gestión de salto
        if (IsKeyPressed(KEY_SPACE) && !enElAire) {
            std::cout << "Salto" << std::endl;
            animacionActiva = JUMPING;
            enElAire = true;
            destRec.y -= velocidadSalto;
            saltoRecorrido += velocidadSalto;
        }
        else if (saltoRecorrido < distanciaSaltoMax && enElAire && !cayendo) {
            animacionActiva = JUMPING;
            destRec.y -= velocidadSalto;
            saltoRecorrido += velocidadSalto;
        }
        else if (enElAire && cayendo && saltoRecorrido > 0) {
            animacionActiva = FALLING;
            destRec.y += velocidadSalto;
            saltoRecorrido -= velocidadSalto;
        } else if (enElAire && cayendo) { //Planeando
            std::cout << "I'm gliding" << std::endl;
            animacionActiva = FALLING;
            destRec.y += velocidadSalto / 2;
            saltoRecorrido -= velocidadSalto / 2;
        }
        else if (enElAire) { //Inicio caída
            animacionActiva = FALLING;
            cayendo = true;
            destRec.y += velocidadSalto; //planeo
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
            case 1:
                indiceAnimacion = (indiceAnimacion + 1) % fMovingAnimation;
                break;
            case 2:
                indiceAnimacion = (indiceAnimacion + 1) % fJumpingAnimation;
                break;
            case 3:
                indiceAnimacion = (indiceAnimacion + 1) % fFallingAnimation;
                break;
            default:
                break;
            }
        }
    };

    void Dibujar() {
        srcRec.x = (float)widthAnimation * (float)indiceAnimacion; //Cambia el fotograma   
        if ((switchOrientacion == 2 && orientacionActual == 3) || (switchOrientacion == 3 && orientacionActual == 2)) {
            std::cout << "Cambio de orientacion" << std::endl;
            srcRec.width *= -1; //Cambia la orientacion
            orientacionActual = switchOrientacion;
        }
        //std::cerr << "Indice Animacion: " << indiceAnimacion << std::endl;
        switch (animacionActiva) { //A los switch no le gustan las constantes, así que a morir al palo
        case 0:
            DrawTexturePro(standingAnimation, srcRec, destRec, origin, 0.0f, WHITE);
            break;
        case 1:
            DrawTexturePro(movingAnimation, srcRec, destRec, origin, 0.0f, WHITE);
            break;
        case 2:
            DrawTexturePro(jumpingAnimation, srcRec, destRec, origin, 0.0f, WHITE);
            break;
        case 3:
            DrawTexturePro(fallingAnimation, srcRec, destRec, origin, 0.0f, WHITE);
            break;
        default:
            break;
        }
    }

    void compruebaColision(const Suelo& s) {
        if (((destRec.y + destRec.height / 2) > (s.destRec.y - s.destRec.height / 2)) && 
            ((destRec.x - destRec.width/2) < (s.destRec.x + s.destRec.width/2)) && //No se sale por la derecha
            ((destRec.x + destRec.width / 2) > (s.destRec.x - s.destRec.width / 2)) //No se sale por la izquierda
            ) { //Choca abajo
            destRec.y = (s.destRec.y - s.destRec.height / 2) - destRec.height / 2;
            enElAire = false;
            cayendo = false;
            saltoRecorrido = 0;
        }
    }
};