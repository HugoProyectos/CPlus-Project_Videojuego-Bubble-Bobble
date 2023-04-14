#pragma once
#include "Enemigo.cpp"


class Rosa : public Enemigo {
public:

    //Sprite pixels
    int pixels = 16; //El numero de pixeles del sprite

    //Animation
    Texture2D walkAnimation = LoadTexture("resources/enemyRosa/rosaWalk.png");
    Texture2D deadAnimation = LoadTexture("resources/enemyRosa/rosaDead.png");
    Texture2D waterAnimation = LoadTexture("resources/enemyRosa/rosaWater.png");
    Texture2D angryAnimation = LoadTexture("resources/enemyRosa/rosaAngry.png");
    Texture2D animations[4] = { walkAnimation, deadAnimation, waterAnimation, angryAnimation };

    int fWalkAnimation = 2; //N�mero de fotogramas de la animacion camniar
    int fDeadAnimation = 4; //N�mero de fotogramas de la animacion muerte
    int fWaterAnimation = 2; //N�mero de fotogramas de la animacion muerte agua
    int fAngryAnimation = 2; //N�mero de fotogramas de la animacion enfado
    int fAnimation[4] = { fWalkAnimation , fDeadAnimation, fWaterAnimation, fAngryAnimation };

    int widthAnimation; // Se actualiza para cada animaci�n activa
    int heightAnimation;

    int animacionActiva = 0; //Indica la animaci�n activa: 0->WalkAnimation, 1->DeadAnimation, 2->WaterAniamtion, 3->AngryAnimation
    int indiceAnimacion = 0; //Indica el n�mero de frame actual de la animaci�n activa

    //Frames
    int targetFrames;
    int cuentaFrames = 0;
    int velocidadFrames = 2;

    //Colisiones
    Plataforma lastGround;

    //Variables l�gicas
    int direccionX = 0; //0->izquierda, 1->derecha
    int direccionY = 0; //0->abajo, 1->arriba

    //Muerto -> Ahora esta en Enemigo
    //bool muerto = false;

    Rosa(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral, float _targetFPS, Rectangle destino) {
        Inicializador(rutaTextura, tamano, saltoMax, velSalto, velLateral);
        destRec = destino;
        tipo = 1;
        widthAnimation = walkAnimation.width / fWalkAnimation;
        heightAnimation = walkAnimation.height;
        targetFrames = _targetFPS;
        enElAire = true;
        cayendo = true;
    };

    // Controlador de comportamiento
    void Actualizar(Rectangle playerPosition) override {
        if (muerto) {
            animacionActiva = 1;
            Caer();
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
                indiceAnimacion = (indiceAnimacion + 1) % fAnimation[1];
                widthAnimation = deadAnimation.width / fAnimation[1];
                heightAnimation = deadAnimation.height;
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
    void SeguirJugador() {
        //
        destRec.x -= velocidadLateral;
        destRec.y -= velocidadSalto;
        srcRec.width = pixels;
    }
    void MoverVertical() {
        destRec.y -= velocidadSalto;
        srcRec.width = pixels;
    }

    void MoverHorizontal() {
        destRec.x += velocidadLateral;
        srcRec.width = -pixels;
    }

    void Caer() {
        destRec.y += velocidadSalto;
    }

    void CaerLento() {
        destRec.y += velocidadSalto/2;
    }

    //Comprobacion de colisiones
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
            switch (s.aproach[enemyNum + 1]) {
            case 1:
                //Izquierda
                destRec.x = s.left - destRec.width / 2;
                direccionX = 1; //Colisiona izquierda, hora se mueve derecha
                //Se puede a�adir un movimiento random en eje Y
                break;
            case 2:
                //Derecha
                destRec.x = s.right + destRec.width / 2;
                direccionX = 0; //Colisiona derecha, ahora se mueve izquierda
                //Se puede a�adir un movimiento random en eje Y
                break;
            case 3:
                //Abajo
                destRec.y = s.top - destRec.height / 2;
                lastGround = s;
                direccionY = 1; //Colisiona abajo, ahora se mueve arriba
                //Se puede a�adir un movimiento random en eje X
                break;
            case 4:
                //Arriba
                destRec.y = s.bot + destRec.height / 2;
                direccionY = 0; //Colisiona arriba, ahora se mueve abajo
                //Se puede a�adir un movimiento random en eje X
                break;
            }
        }
        //Comprobamos si se esta acercando a la superficie desde alguna direcci�n
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
                s.aproach[enemyNum + 1] = 1;
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
                s.aproach[enemyNum + 1] = 2;
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
                s.aproach[enemyNum + 1] = 3;
            }
            //Abajo
            else {
                //Si no se cumplen anteriores asumimos que se acerca por debajo
                s.aproach[enemyNum + 1] = 4;
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
            // No colisiona con plataforma
            enElAire = true;
            cayendo = true;
        }
        else if (muerto) {
            enElAire = false;
            cayendo = false;
            borrame = true;
        }
        else {
            enElAire = false;
            cayendo = false;
        }
    }

    void compruebaPared(const Columnas& s) {
        //Comprobamos columna derecha
        if (s.left_der < (destRec.x + destRec.width / 2)) {
            destRec.x = s.left_der - destRec.width / 2;
        }
        //Comprobamos columna izquierda
        else if (s.right_izq > (destRec.x - destRec.width / 2)) {
            destRec.x = s.right_izq + destRec.width / 2;
        }
    }
};

typedef std::shared_ptr<Rosa> sh_Rosa;