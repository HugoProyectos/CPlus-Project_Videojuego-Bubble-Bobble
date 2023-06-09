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
    int fWalkAnimation = 4; //N�mero de fotogramas de la animacion camniar
    int fDeadAnimation = 4; //N�mero de fotogramas de la animacion camniar
    int widthAnimation; // Se actualiza para cada animaci�n activa
    int heightAnimation;
    Texture2D animations[2] = { walkAnimation, deadAnimation };
    int fAnimation[2] = { fWalkAnimation, fDeadAnimation };
    int ID;

    int lastWitdh = 800;
    int lastHeight = 450;
    double ratioX = 1;

    int animacionActiva = 0; //Indica la animaci�n activa: 0->WalkAnimation, 1->DeadAnimation, 2->BallLeft,
    int indiceAnimacion = 0; //Indica el n�mero de frame actual de la animaci�n activa

    //Frames
    int targetFrames;
    int cuentaFrames = 0;
    int velocidadFrames = 2;

    bool direccion; // izquierda false derecha true
    Plataforma lastGround;
    clock_t temp;

    Bola() = default;

    Bola(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral, float _targetFPS, Rectangle destino, bool direccion, int ID, int lastW, int lastH, Vector2 origin) {
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
        this->origin = origin;
    };

    void Actualizar(Rectangle playerPosition1, Rectangle playerPosition2) override {
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

        //Posicion para pasarle a BOB
        posicion = destRec;

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
        //Colision con pared desde la derecha | <-
        if ( (
            (((s.bot) > (destRec.y - destRec.height / 2.0f)) &&
                ((destRec.y - destRec.height / 2.0f) > (s.top))
                ) && (
                    ((s.right) > (destRec.x + destRec.width / 2 + destRec.width / 6)) &&
                    ((destRec.x + destRec.width / 2 + destRec.width / 6) > (s.left))
                    )
            )
            ||
            
            (
                (((s.bot) > (destRec.y + destRec.height * 0.49)) &&
                    ((destRec.y + destRec.height * 0.49) > (s.top))
                    ) && (
                        ((s.right) > (destRec.x + destRec.width / 2 + destRec.width / 6)) &&
                        ((destRec.x + destRec.width / 2 + destRec.width / 6) > (s.left))
                        )
                )
            )
        
        {
            velocidadLateral = 0;
            animacionActiva = 1;
        }
        //Colision con pared desde la izquierda -> |
        else if (
            //Comprobamos colision esquina superior derecha
            (
                (((s.bot) > (destRec.y - destRec.height / 2.0f)) &&
                    ((destRec.y - destRec.height / 2.0f) > (s.top))
                    ) && (
                        ((s.right) > (destRec.x - destRec.width / 2 - destRec.width / 6)) &&
                        ((destRec.x - destRec.width / 2 - destRec.width / 6) > (s.left))
                        )
                )
            ||
            //Comprobamos colision esquina inferior derecha
            (
                (((s.bot) > (destRec.y + destRec.height * 0.49)) &&
                    ((destRec.y + destRec.height * 0.49) > (s.top))
                    ) && (
                        ((s.right) > (destRec.x - destRec.width / 2 - destRec.width / 6)) &&
                        ((destRec.x - destRec.width / 2 - destRec.width / 6) > (s.left))
                        )
                )
            ) {
            velocidadLateral = 0;
            animacionActiva = 1;
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