#pragma once
#include "Enemigo.cpp"


class Blanco : public Enemigo {
public:

    //Sprite pixels
    int pixels = 16; //El numero de pixeles del sprite

    //Animation
    Texture2D walkAnimation = LoadTexture("resources/enemyBlanco/blancoWalk.png");
    Texture2D animations[1] = { walkAnimation };

    int fWalkAnimation = 2; //Número de fotogramas de la animacion camniar
    int fAnimation[1] = { fWalkAnimation };

    int widthAnimation; // Se actualiza para cada animación activa
    int heightAnimation;

    int animacionActiva = 0; //Indica la animación activa: 0->WalkAnimation, 1->DeadAnimation, 2->WaterAniamtion, 3->AngryAnimation
    int indiceAnimacion = 0; //Indica el número de frame actual de la animación activa

    //Frames
    int targetFrames;
    int cuentaFrames = 0;
    int velocidadFrames = 4;

    //Lógica
    int contador = 0;
    bool moviendo = false;
    int sentidoX = 0;
    int sentidoY = 0;

    Blanco(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral, float _targetFPS, Rectangle destino) {
        Inicializador(rutaTextura, tamano, saltoMax, velSalto, velLateral);
        destRec = destino;
        tipo = -2;
        widthAnimation = walkAnimation.width / fWalkAnimation;
        heightAnimation = walkAnimation.height;
        targetFrames = _targetFPS;
    };

    // Controlador de comportamiento
    void Actualizar(Rectangle playerPosition) override {

        //Contar segundos 
        //Si ha pasado el tiempo mover durante x segundos
        if (contador < 180) {
            contador++;
        }
        else {
            if (moviendo) {
                //Si se estaba moviendo ahora se detiene
                moviendo = false;
                sentidoX = 0;
                sentidoY = 0;
            }
            else {
                //Si no se estaba moviendo ahora se mueve
                float difY = destRec.y - playerPosition.y;
                float difX = destRec.x - playerPosition.x;
                if (abs(difX) >= abs(difY)) {
                    //Se mueve en horizontal
                    if (difX > 0) {
                        //Se mueve a la izquierda
                        sentidoX = -1;
                        sentidoY = 0;
                        srcRec.width = pixels;
                    }
                    else{
                        //Se mueve a la derecha
                        sentidoX = 1;
                        sentidoY = 0;
                        srcRec.width = -pixels;
                    }
                }
                else {
                    //Se mueve en vertical
                    if (difY > 0) {
                        //Se mueve a arriba
                        sentidoX = 0;
                        sentidoY = 1;
                    }
                    else {
                        //Se mueve a abajo
                        sentidoX = 0;
                        sentidoY = -1;
                    }
                }
                moviendo = true;
            }
            contador = 0;
        }

        //Mover 
        Mover(playerPosition);

        //Actualizar puntero de animacion
        cuentaFrames++;
        if (cuentaFrames >= (targetFrames / velocidadFrames)) {
            cuentaFrames = 0;
            switch (animacionActiva) {
            case 0:
                //Actualizar width&height animacion
                indiceAnimacion = (indiceAnimacion + 1) % fWalkAnimation;
                widthAnimation = walkAnimation.width / fWalkAnimation;
                heightAnimation = walkAnimation.height;
                break;
            default:
                break;
            }
        }
    }

    void Dibujar() override {
        srcRec.x = (float)widthAnimation * (float)indiceAnimacion;
        DrawTexturePro(animations[animacionActiva], srcRec, destRec, origin, 0.0f, WHITE);
    }

    //funciones de comportamiento
    void Mover(Rectangle playerPosition) {
        if (sentidoX != 0) {
            //El movimiento horizontal depende del jugador
            if (destRec.x > playerPosition.x - 5) {
                //Estamos por izq del player, restamos posicion
                destRec.x -= velocidadLateral;
            }
            else if (destRec.x < playerPosition.x + 5) {
                //Estamos por derecha del player, sumamos posicion
                destRec.x += velocidadLateral;
            }
        }
        else if (sentidoY != 0) {
            if (destRec.y > playerPosition.y + 5) {
                //Estamos por debajo del player, restamos posicion
                destRec.y -= velocidadLateral;
            }
            else if (destRec.y < playerPosition.y - 5) {
                //Estamos por encima del player, sumamos posicion
                destRec.y += velocidadLateral;
            }
        }

        //Comprobar que no se sale de los bordes del mapa
    }

    void compruebaColision(Plataforma& s, int enemyNum) override {
        return;
    }


    //Comprobacion de si debe caer
    void compruebaSuelo() override {
        return;
    }

    void compruebaPared(const Columnas& s) override {
        return;
    }
};

typedef std::shared_ptr<Blanco> sh_Blanco;