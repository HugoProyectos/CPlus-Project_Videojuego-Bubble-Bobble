#pragma once
#include "Enemigo.cpp"
#include <ctime>
class Bola : public Enemigo {
public:
    //Sprite pixels
    int pixels = 16; //El numero de pixeles del sprite

    //Animation
    Texture2D walkAnimation = LoadTexture("resources/enemyBola/bolas.png");
    Texture2D deadAnimation = LoadTexture("resources/enemyBola/bolasMuerte.png");
    int fWalkAnimation = 4; //Número de fotogramas de la animacion camniar
    int fDeadAnimation = 4; //Número de fotogramas de la animacion camniar
    int widthAnimation; // Se actualiza para cada animación activa
    int heightAnimation;
    Texture2D animations[2] = { walkAnimation, deadAnimation };
    int fAnimation[2] = { fWalkAnimation, fDeadAnimation };
    int ID;

    int lastWitdh = 800;
    int lastHeight = 450;
    double ratioX = 1;

    int animacionActiva = 0; //Indica la animación activa: 0->WalkAnimation, 1->DeadAnimation, 2->BallLeft,
    int indiceAnimacion = 0; //Indica el número de frame actual de la animación activa

    //Frames
    int targetFrames;
    int cuentaFrames = 0;
    int velocidadFrames = 2;

    bool direccion; // izquierda false derecha true
    Plataforma lastGround;
    clock_t temp;

    Bola() = default;

    Bola(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral, float _targetFPS, Rectangle destino, bool direccion, int ID, int lastW, int lastH) {
        Inicializador(rutaTextura, tamano, saltoMax, velSalto, velLateral);
        destRec = destino;
        lastWitdh = lastW;
        lastHeight = lastH;
        tipo = -2;
        widthAnimation = walkAnimation.width / fWalkAnimation;
        heightAnimation = walkAnimation.height;
        targetFrames = _targetFPS;
        enElAire = true;
        cayendo = false;
        this->direccion = direccion;
        temp = clock();
        this->ID = ID;
        borrame = false;
    };

    void Actualizar(Rectangle playerPosition) override {
        if (lastHeight != GetScreenHeight()) {
            destRec.height = GetScreenHeight() / 14.0625f;
            destRec.y = GetScreenHeight() * (destRec.y / lastHeight);
            origin.y = destRec.height / 2;

            lastHeight = GetScreenHeight();
        }
        if (lastWidth != GetScreenWidth()) {
            destRec.width = GetScreenWidth() / 25.0f;
            destRec.x = GetScreenWidth() * (destRec.x / lastWidth);
            origin.x = destRec.width / 2;

            lastWidth = GetScreenWidth();
        }
        ratioX = destRec.width / 32;

        if (!direccion && animacionActiva == 0) { //Si el personaje esta a la izquierda
            MoverIzq();
        }
        else if (direccion && animacionActiva == 0) { //Si el personaje esta a la derecha
            MoverDer();
        }

        


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
                indiceAnimacion = (indiceAnimacion + 1) % fAnimation[1];
                widthAnimation = deadAnimation.width / fAnimation[1];
                heightAnimation = deadAnimation.height;
                if (indiceAnimacion == 3) {
                    borrame = true;
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

    void MoverIzq() {
        destRec.x -= velocidadLateral * ratioX;
        srcRec.width = pixels;
    }

    void MoverDer() {
        destRec.x += velocidadLateral * ratioX;
        srcRec.width = -pixels;
    }


    void compruebaColision(Plataforma& s, int enemyNum) override {
        //Comprobamos si colisiona con la superficie
        if (
            (
                //Comprobamos colision esquina inferior derecha
                (((s.bot) > (destRec.y + destRec.height / 2)) &&
                    ((destRec.y + destRec.height / 2) > (s.top))
                    ) && (
                        ((s.right) > (destRec.x + destRec.width / 2)) &&
                        ((destRec.x + destRec.width / 2) > (s.left))
                        )
                ) ||
            (
                //Comprobamos colision esquina superior derecha
                (((s.bot) > (destRec.y - destRec.height / 2)) &&
                    ((destRec.y - destRec.height / 2) > (s.top))
                    ) && (
                        ((s.right) > (destRec.x + destRec.width / 2)) &&
                        ((destRec.x + destRec.width / 2) > (s.left))
                        )
                ) ||
            (
                //Comprobamos colision esquina superior izquierda
                (((s.bot) > (destRec.y - destRec.height / 2)) &&
                    ((destRec.y - destRec.height / 2) > (s.top))
                    ) && (
                        ((s.right) > (destRec.x - destRec.width / 2)) &&
                        ((destRec.x - destRec.width / 2) > (s.left))
                        )
                ) ||
            (
                //Comprobamos colision esquina inferior izquierda
                (((s.bot) > (destRec.y + destRec.height / 2)) &&
                    ((destRec.y + destRec.height / 2) > (s.top))
                    ) && (
                        ((s.right) > (destRec.x - destRec.width / 2)) &&
                        ((destRec.x - destRec.width / 2) > (s.left))
                        )
                )
            ) {
            switch (s.aproach[enemyNum + 20]) {
            case 1:
                destRec.x = s.left - destRec.width / 2;
                break;
            case 2:
                destRec.x = s.right + destRec.width / 2;
                break;
            case 3:
                destRec.y = s.top - destRec.height / 2;
                enElAire = false;
                cayendo = false;
                saltoRecorrido = 0;
                lastGround = s;
                break;
            }
        }
        //Comprobamos si se esta acercando a la superficie desde alguna dirección
        else {
            //Izquierda
            if (
                //Comprobamos colision esquina superior derecha
                (
                    (((s.bot) > (destRec.y - destRec.height / 2)) &&
                        ((destRec.y - destRec.height / 2) > (s.top))
                        ) && (
                            ((s.right) > (destRec.x + destRec.width / 2 + 5)) &&
                            ((destRec.x + destRec.width / 2 + 5) > (s.left))
                            )
                    )
                ||
                //Comprobamos colision esquina inferior derecha
                (
                    (((s.bot) > (destRec.y + destRec.height / 2)) &&
                        ((destRec.y + destRec.height / 2) > (s.top))
                        ) && (
                            ((s.right) > (destRec.x + destRec.width / 2 + 5)) &&
                            ((destRec.x + destRec.width / 2 + 5) > (s.left))
                            )
                    )
                ) {
                s.aproach[enemyNum + 20] = 1;
            }
            //Derecha
            else if (
                //Comprobamos colision esquina superior derecha
                (
                    (((s.bot) > (destRec.y - destRec.height / 2)) &&
                        ((destRec.y - destRec.height / 2) > (s.top))
                        ) && (
                            ((s.right) > (destRec.x - destRec.width / 2 - 5)) &&
                            ((destRec.x - destRec.width / 2 - 5) > (s.left))
                            )
                    )
                ||
                //Comprobamos colision esquina inferior derecha
                (
                    (((s.bot) > (destRec.y + destRec.height / 2)) &&
                        ((destRec.y + destRec.height / 2) > (s.top))
                        ) && (
                            ((s.right) > (destRec.x - destRec.width / 2 - 5)) &&
                            ((destRec.x - destRec.width / 2 - 5) > (s.left))
                            )
                    )
                ) {
                s.aproach[enemyNum + 20] = 2;
            }
            //Arriba
            else if (
                //Comprobamos colision esquina inferior derecha
                (
                    (((s.bot) > (destRec.y + destRec.height / 2 + 5)) &&
                        ((destRec.y + destRec.height / 2 + 5) > (s.top))
                        ) && (
                            ((s.right) > (destRec.x + destRec.width / 2)) &&
                            ((destRec.x + destRec.width / 2) > (s.left))
                            )
                    )
                ||
                //Comprobamos colision esquina inferior izquierda
                (
                    (((s.bot) > (destRec.y + destRec.height / 2 + 5)) &&
                        ((destRec.y + destRec.height / 2 + 5) > (s.top))
                        ) && (
                            ((s.right) > (destRec.x - destRec.width / 2)) &&
                            ((destRec.x - destRec.width / 2) > (s.left))
                            )
                    )
                ) {
                s.aproach[enemyNum + 20] = 3;
            }
            //Abajo
            else {
                //Si no se cumplen anteriores asumimos que se acerca por debajo
                s.aproach[enemyNum + 20] = 4;
            }
        }
    }

    //Comprobacion de si debe caer
    void compruebaSuelo() override {
        if (
            !(
                //Comprobamos colision esquina inferior derecha
                (((lastGround.bot) > (destRec.y + destRec.height / 2)) &&
                    ((destRec.y + destRec.height / 2 + 1) > (lastGround.top))
                    ) && (
                        ((lastGround.right) > (destRec.x + destRec.width / 2)) &&
                        ((destRec.x + destRec.width / 2) > (lastGround.left))
                        )
                ) &&
            !(
                //Comprobamos colision esquina inferior izquierda
                (((lastGround.bot) > (destRec.y + destRec.height / 2)) &&
                    ((destRec.y + destRec.height / 2 + 1) > (lastGround.top))
                    ) && (
                        ((lastGround.right) > (destRec.x - destRec.width / 2)) &&
                        ((destRec.x - destRec.width / 2) > (lastGround.left))
                        )
                )
            ) {

        }
        else if (muerto) {
            enElAire = false;
            cayendo = false;
            animacionActiva = 1;
        }
    }

    void compruebaPared(const Columnas& s) override {
        //Comprobamos columna derecha
        if (s.left_der < (destRec.x + destRec.width / 2)) {
            velocidadLateral = 0;
            animacionActiva = 1;
        }
        //Comprobamos columna izquierda
        else if (s.right_izq > (destRec.x - destRec.width / 2)) {
            velocidadLateral = 0;
            animacionActiva = 1;
        }
    }

};