#pragma once
#include "Enemigo.cpp"


class Blanco : public Enemigo {
public:

    //Sprite pixels
    int pixels = 16; //El numero de pixeles del sprite

    //Animation
    Texture2D walkAnimation = LoadTexture("resources/enemyBlanco/blancoWalk.png");
    Texture2D deadAnimation = LoadTexture("resources/enemyBlanco/blancoDead.png");
    Texture2D animations[2] = { walkAnimation, deadAnimation };

    int fWalkAnimation = 2; //Número de fotogramas de la animacion camniar
    int fDeadAnimation = 3; //Número de fotogramas de la animacion camniar
    int fAnimation[2] = { fWalkAnimation, fDeadAnimation };

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
    bool empezando = true;

    //Admin->Saber cuando borrar
    DatosJugador *j1;
    DatosJugador *j2;

    Blanco() = default;

    Blanco(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral, float _targetFPS, Rectangle destino, DatosJugador& _j1, DatosJugador& _j2) {
        Inicializador(rutaTextura, tamano, saltoMax, velSalto, velLateral);
        destRec = destino;
        tipo = -2;
        widthAnimation = walkAnimation.width / fWalkAnimation;
        heightAnimation = walkAnimation.height;
        targetFrames = _targetFPS;
        this->j1 = &_j1;
        this->j2 = &_j2;
    };

    // Controlador de comportamiento
    void Actualizar(Rectangle playerPosition1, Rectangle playerPosition2) override {

        //ver si borrar
        if ((j1->muriendo || j2->muriendo) && !muerto) {
            muerto = true;
            animacionActiva = 1;
            return;
        }
        //Ver si estamos empezando
        if (empezando) {
            animacionActiva = 1;
            empezando = false;
        }

        if (lastHeight != GetScreenHeight()) {
            destRec.height = GetScreenHeight() / 14.0625f;
            destRec.y = GetScreenHeight() * (destRec.y / lastHeight);
            distanciaSaltoMax = distanciaSaltoMax * ((float)GetScreenHeight() / (float)lastHeight);
            origin.y = destRec.height / 2;
            lastHeight = GetScreenHeight();
        }
        if (lastWidth != GetScreenWidth()) {
            destRec.width = GetScreenWidth() / 25.0f;
            destRec.x = GetScreenWidth() * (destRec.x / lastWidth);
            origin.x = destRec.width / 2;
            lastWidth = GetScreenWidth();
        }

        velocidadLateral = destRec.width / 16.0f;
        velocidadSalto = destRec.height / 10.0f;

        //Obtener el rectangulo del jugador a seguir
        Rectangle playerPosition;
        if (playerPosition2.x == -1 && playerPosition2.y == -1) {
            playerPosition = playerPosition1;
        }
        else if (playerPosition1.x == -1 && playerPosition1.y == -1) {
            playerPosition = playerPosition2;
        }
        else if (destRec.y <= playerPosition1.y && destRec.y + destRec.height >= playerPosition1.y) {
            playerPosition = playerPosition1;
        }
        else if (destRec.y <= playerPosition2.y && destRec.y + destRec.height >= playerPosition2.y) {
            playerPosition = playerPosition2;
        }
        else {
            float dist1 = sqrt(pow(playerPosition1.x - destRec.x, 2) + pow(playerPosition1.y - destRec.y, 2));
            float dist2 = sqrt(pow(playerPosition2.x - destRec.x, 2) + pow(playerPosition2.y - destRec.y, 2));
            if (dist1 <= dist2) {
                playerPosition = playerPosition1;
            }
            else {
                playerPosition = playerPosition2;
            }
        }

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
                    }
                    else{
                        //Se mueve a la derecha
                        sentidoX = 1;
                        sentidoY = 0;
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
        if (!muerto || !empezando) {
            Mover(playerPosition);
        }

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
            case 1:
                //Actualizar width&height animacion
                indiceAnimacion = (indiceAnimacion + 1) % fDeadAnimation;
                widthAnimation = deadAnimation.width / fDeadAnimation;
                heightAnimation = deadAnimation.height;
                if (indiceAnimacion == 2 && muerto) {
                    borrame = true;
                    muerto = true;
                }
                else if(indiceAnimacion == 2 && !muerto) {
                    animacionActiva = 0;
                }
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
            if (destRec.x > playerPosition.x + 10) {
                //Estamos por izq del player, restamos posicion
                destRec.x -= velocidadLateral;
                srcRec.width = pixels;

            }
            else if (destRec.x < playerPosition.x - 10) {
                //Estamos por derecha del player, sumamos posicion
                destRec.x += velocidadLateral;
                srcRec.width = -pixels;

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