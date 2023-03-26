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
    Texture2D deathAnimation = LoadTexture("resources/Players/Bobblun/Muerte.png");
    int fDeathAnimation = 30; //Numero de fotogramas

    int widthAnimation = 16;
    int heightAnimation = 16;
    int switchOrientacion = 1;
    int orientacionActual = 2;
    int dirCorrer = 0; //0 = parado, 1 = izquierda, 2 = derecha
    int dirAir = 0; //0 = parado, 1 = izquieda, 2 = derecha

    int animacionActiva = 0; //0->StandingAnimation 1->MoveAnimation 2->JumpAnimation 3->FallAnimation 4->ShootingAnimation 5->DyingAninimation
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
    float deceleracion = 0.1f;
    float velocidadActual = 0;

    Bub() = default; //Debe llamarse a Inicializador

    Bub(float tamano, float saltoMax, float velSalto, float velLateral, int _targetFrames) {
        Inicializador(tamano, saltoMax, velSalto, velLateral, _targetFrames);
    };
    void Inicializador(float tamano, float saltoMax, float velSalto,float velLateral, int _targetFrames)
    {
        sprite = LoadTexture("resources/Players/Bobblun/animation_set.png");
        //widthAnimation = standingAnimation.width / fStandingAnimation;
        //heightAnimation = standingAnimation.height;
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
        //Si cae al fondo del nivel reaparece en el techo
        if (destRec.y > 500) {
            destRec.y = 0;
        }
        //Gestión de desplazamiento lateral
        if (enElAire) {
            if (saltoRecorrido > 0) {
                if (dirCorrer == 1) {  //Salta izquierda
                    if (IsKeyDown(KEY_S)) {
                        //(OPCIONAL) Añadir que decelere en vez de cambiar repentinamente de velocidad
                        switchOrientacion = 3;
                        destRec.x -= velocidadLateral/3;
                        dirAir = 2;
                    }
                    else {
                        if (IsKeyDown(KEY_A)) {
                            switchOrientacion = 2;
                            dirAir = 1;
                            destRec.x -= velocidadLateral;
                        }
                        else if (dirAir == 1) {
                            destRec.x -= velocidadLateral;
                        }
                        else {
                            destRec.x -= velocidadLateral/3;
                        }
                    }
                }
                else if (dirCorrer == 2) {  //Salta derecha
                    if (IsKeyDown(KEY_A)) {
                        //(OPCIONAL) Añadir que decelere en vez de cambiar repentinamente de velocidad
                        switchOrientacion = 2;
                        destRec.x += velocidadLateral/3;
                        dirAir = 1;
                    }
                    else {
                        if (IsKeyDown(KEY_S)) {
                            switchOrientacion = 3;
                            dirAir = 2;
                            destRec.x += velocidadLateral;
                        }
                        else if (dirAir == 2) {
                            destRec.x += velocidadLateral;
                        }
                        else {
                            destRec.x += velocidadLateral/3;
                        }
                    }
                }
                else {
                    if (IsKeyDown(KEY_A)) {
                        switchOrientacion = 2;
                        destRec.x -= velocidadLateral / 2;
                    }
                    else if (IsKeyDown(KEY_S)) {
                        switchOrientacion = 3;
                        destRec.x += velocidadLateral / 2;
                    }
                }
            }else {
                if (IsKeyDown(KEY_A)) {
                    animacionActiva = MOVING;
                    switchOrientacion = 2;
                    destRec.x -= velocidadLateral/2;
                }else if(IsKeyDown(KEY_S)) {
                    animacionActiva = MOVING;
                    switchOrientacion = 3;
                    destRec.x += velocidadLateral/2;
                }
            }
        }else {
            if (IsKeyDown(KEY_A)) {
                animacionActiva = MOVING;
                switchOrientacion = 2;
                std::cout << "Muevo Izquierda, orientacion: " << switchOrientacion << std::endl;
                destRec.x -= velocidadLateral;
                dirCorrer = 1;
                dirAir = 1;
            }
            else if (IsKeyDown(KEY_S)) {
                animacionActiva = MOVING;
                switchOrientacion = 3;
                std::cout << "Muevo Derecha, orientacion: " << switchOrientacion << std::endl;
                destRec.x += velocidadLateral;
                dirCorrer = 2;
                dirAir = 2;
            }
            else {
                dirCorrer = 0;
                if (!(enElAire && saltoRecorrido == 0)) {
                    switchOrientacion = 1;
                }
                animacionActiva = 0;
            }
        }

        //Gestión de salto
        if (IsKeyDown(KEY_SPACE) && !enElAire) {
            //std::cout << "Salto" << std::endl;
            animacionActiva = JUMPING;
            enElAire = true;
            velocidadActual = velocidadSalto;
            destRec.y -= velocidadActual;
            saltoRecorrido += velocidadActual;
            velocidadActual -= deceleracion;
        }
        else if (velocidadActual > 0 && enElAire && !cayendo) {
            animacionActiva = JUMPING;
            destRec.y -= velocidadActual;
            saltoRecorrido += velocidadActual;
            velocidadActual -= deceleracion;
        }
        else if (enElAire && cayendo && saltoRecorrido > 0) {
            animacionActiva = FALLING;
            destRec.y -= velocidadActual;
            saltoRecorrido += velocidadActual;
            velocidadActual -= deceleracion;
        } else if (enElAire && cayendo) { //Planeando
            //std::cout << "I'm gliding" << std::endl;
            animacionActiva = FALLING;
            destRec.y += velocidadSalto / 2;
            saltoRecorrido -= velocidadSalto / 2;
        }
        else if (enElAire) { //Inicio caída
            animacionActiva = FALLING;
            cayendo = true;
            destRec.y -= velocidadActual;
            saltoRecorrido += velocidadActual; //planeo
            velocidadActual -= deceleracion;
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
            case 5:
                indiceAnimacion = (indiceAnimacion + 1) % fDeathAnimation;
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
        srcRec.y = (float)widthAnimation * (float)animacionActiva;
        DrawTexturePro(sprite, srcRec, destRec, origin, 0.0f, WHITE);
        /*
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
        }*/
    }

    void compruebaColision(Suelo& s) {
        //Comprobamos si colisiona con la superficie
        if ( 
            (
                //Comprobamos colision esquina inferior derecha
                ( ( (s.destRec.y + s.destRec.height / 2) > (destRec.y + destRec.height / 2) ) && 
                  ( (destRec.y + destRec.height / 2) > (s.destRec.y - s.destRec.height / 2) )
                ) && ( 
                  ( (s.destRec.x + s.destRec.width / 2) > (destRec.x + destRec.width / 2) ) &&
                  ( (destRec.x + destRec.width / 2) > (s.destRec.x - s.destRec.width / 2) )
                )
            ) ||
            (
                //Comprobamos colision esquina superior derecha
                ( ( (s.destRec.y + s.destRec.height / 2) > (destRec.y - destRec.height / 2) ) &&
                  ( (destRec.y - destRec.height / 2) > (s.destRec.y - s.destRec.height / 2) )
                ) && (
                  ( (s.destRec.x + s.destRec.width / 2) > (destRec.x + destRec.width / 2) ) &&
                  ( (destRec.x + destRec.width / 2) > (s.destRec.x - s.destRec.width / 2) )
                )
            ) ||
            (
                //Comprobamos colision esquina superior izquierda
                ( ( (s.destRec.y + s.destRec.height / 2) > (destRec.y - destRec.height / 2) ) &&
                  ( (destRec.y - destRec.height / 2) > (s.destRec.y - s.destRec.height / 2) )
                ) && (
                  ( (s.destRec.x + s.destRec.width / 2) > (destRec.x - destRec.width / 2) ) &&
                  ( (destRec.x - destRec.width / 2) > (s.destRec.x - s.destRec.width / 2) )
                )
            ) ||
            (
                //Comprobamos colision esquina inferior izquierda
                ( ( (s.destRec.y + s.destRec.height / 2) > (destRec.y + destRec.height / 2) ) &&
                  ( (destRec.y + destRec.height / 2) > (s.destRec.y - s.destRec.height / 2) )
                ) && (
                  ( (s.destRec.x + s.destRec.width / 2) > (destRec.x - destRec.width / 2) ) &&
                  ( (destRec.x - destRec.width / 2) > (s.destRec.x - s.destRec.width / 2) )
                )
            )
            ) {
            std::cout << "Choque" << std::endl;
            switch (s.aproach) {
            case 1:
                destRec.x = (s.destRec.x - s.destRec.width / 2) - destRec.width / 2;
                break;
            case 2:
                destRec.x = (s.destRec.x + s.destRec.width / 2) + destRec.width / 2;
                break;
            case 3:
                destRec.y = (s.destRec.y - s.destRec.height / 2) - destRec.height / 2;
                enElAire = false;
                cayendo = false;
                saltoRecorrido = 0;
                break;
            }
        }
        //Comprobamos si se esta acercando a la superficie desde alguna dirección
        else {
            //Izquierda
            if (
                //Comprobamos colision esquina superior derecha
                  (
                    ( ( (s.destRec.y + s.destRec.height / 2) > (destRec.y - destRec.height / 2) ) &&
                      ( (destRec.y - destRec.height / 2) > (s.destRec.y - s.destRec.height / 2) )
                    ) && (
                      ( (s.destRec.x + s.destRec.width / 2) > (destRec.x + destRec.width / 2 + 5) ) &&
                      ( (destRec.x + destRec.width / 2 + 5) > (s.destRec.x - s.destRec.width / 2) )
                    )
                  )
                ||
                //Comprobamos colision esquina inferior derecha
                  (
                    ( ( (s.destRec.y + s.destRec.height / 2) > (destRec.y + destRec.height / 2) ) &&
                      ( (destRec.y + destRec.height / 2) > (s.destRec.y - s.destRec.height / 2) )
                    ) && (
                      ( (s.destRec.x + s.destRec.width / 2) > (destRec.x + destRec.width / 2 + 5) ) &&
                      ( (destRec.x + destRec.width / 2 + 5) > (s.destRec.x - s.destRec.width / 2) )
                    )
                  )
                ) {
                std::cout << "Aproach izquierda" << std::endl;
                s.aproach = 1;
            }
            //Derecha
            else if (
                //Comprobamos colision esquina superior derecha
                  (
                    ( ( (s.destRec.y + s.destRec.height / 2) > (destRec.y - destRec.height / 2) ) &&
                      ( (destRec.y - destRec.height / 2) > (s.destRec.y - s.destRec.height / 2) )
                    ) && (
                       ( (s.destRec.x + s.destRec.width / 2) > (destRec.x - destRec.width / 2 - 5) ) &&
                       ( (destRec.x - destRec.width / 2 - 5) > (s.destRec.x - s.destRec.width / 2) )
                    )
                  )
                ||
                //Comprobamos colision esquina inferior derecha
                  (
                    ( ( (s.destRec.y + s.destRec.height / 2) > (destRec.y + destRec.height / 2) ) &&
                      ( (destRec.y + destRec.height / 2) > (s.destRec.y - s.destRec.height / 2) )
                    ) && (
                      ( (s.destRec.x + s.destRec.width / 2) > (destRec.x - destRec.width / 2 - 5) ) &&
                      ( (destRec.x - destRec.width / 2 - 5) > (s.destRec.x - s.destRec.width / 2) )
                    )
                  )
                ) {
                std::cout << "Aproach derecha" << std::endl;
                s.aproach = 2;
            }
            //Arriba
            else if (
                //Comprobamos colision esquina inferior derecha
                  (
                    ( ( (s.destRec.y + s.destRec.height / 2) > (destRec.y + destRec.height / 2 + 5) ) &&
                      ( (destRec.y + destRec.height / 2 + 5) > (s.destRec.y - s.destRec.height / 2) )
                    ) && (
                      ( (s.destRec.x + s.destRec.width / 2) > (destRec.x + destRec.width / 2) ) &&
                      ( (destRec.x + destRec.width / 2) > (s.destRec.x - s.destRec.width / 2) )
                    )
                  )
                ||
                //Comprobamos colision esquina inferior izquierda
                  (
                    ( ( (s.destRec.y + s.destRec.height / 2) > (destRec.y + destRec.height / 2 + 5) ) &&
                      ( (destRec.y + destRec.height / 2 + 5) > (s.destRec.y - s.destRec.height / 2) )
                    ) && (
                      ( (s.destRec.x + s.destRec.width / 2) > (destRec.x - destRec.width / 2) ) &&
                      ( (destRec.x - destRec.width / 2) > (s.destRec.x - s.destRec.width / 2) )
                    )
                  )
                ) {
                std::cout << "Aproach arriba" << std::endl;
                s.aproach = 3;
            }
            //Abajo
            else {
                //Si no se cumplen anteriores asumimos que se acerca por debajo
                s.aproach = 4;
            }
        }
    }
};