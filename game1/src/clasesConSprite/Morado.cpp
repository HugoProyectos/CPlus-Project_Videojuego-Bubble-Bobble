#pragma once
#include"AdministradorPompas.cpp"
#include "Enemigo.cpp"


class Morado : public Enemigo {
public:
    //Gesti�n de transici�n de nivel
    int8_t cambioMapa = 2; //2->Primera Iteraci�n 1->Desplaz�ndose 0->Ya no
    Rectangle posicionPartida = { (float)GetScreenWidth() / 2, (float)50, 32, 32 };
    int cuentaFramesTraslacion = 0; //3 segundos = 3 * 60 frames = 180 frames
    const int LIMITE_FRAMES_TRASLACION = 180; //3 segundos = 3 * 60 frames = 180 frames
    double razonX = 0;
    double razonY = 0;
    ////////////////////
    
    //Sprite pixels
    int pixels = 16; //El numero de pixeles del sprite

    //Animation
    Texture2D walkAnimation = LoadTexture("resources/enemyMorado/moradoWalk.png");
    Texture2D deadAnimation = LoadTexture("resources/enemyMorado/moradoDead.png");
    Texture2D waterAnimation = LoadTexture("resources/enemyMorado/moradoWater.png");
    Texture2D angryAnimation = LoadTexture("resources/enemyMorado/moradoAngry.png");
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
    int velocidadFrames = 4;

    //Colisiones
    Plataforma lastGround;

    //Variables l�gicas
    int direccionX = 0; //0->izquierda, 1->derecha
    int direccionY = 0; //0->abajo, 1->arriba
    AdministradorPompas* admin;

    //Muerto -> Ahora esta en Enemigo
    //bool muerto = false;

    Morado() = default;

    Morado(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral, float _targetFPS, Rectangle destino, AdministradorPompas& admin) {
        Inicializador(rutaTextura, tamano, saltoMax, velSalto, velLateral);
        if (enfadado) {
            animacionActiva = 3;
            velocidadLateral *= 2;
        }
        destRec = destino;
        tipo = 4;
        widthAnimation = walkAnimation.width / fWalkAnimation;
        heightAnimation = walkAnimation.height;
        targetFrames = _targetFPS;
        enElAire = true;
        cayendo = true;
        this->admin = &admin;
    };

    void enfadar() {
        animacionActiva = 3;
        velocidadLateral *= 2;
    }

    // Controlador de comportamiento
    void Actualizar(Rectangle playerPosition) override {
        if (cambioMapa > 0) {
            if (cambioMapa == 2) {
                cambioMapa = 1;
                razonX = (destRec.x - posicionPartida.x) / LIMITE_FRAMES_TRASLACION;
                razonY = (destRec.y - posicionPartida.y) / LIMITE_FRAMES_TRASLACION;
                destRec.x = posicionPartida.x;
                destRec.y = posicionPartida.y;
            }
            destRec.x += razonX;
            destRec.y += razonY;

            cuentaFramesTraslacion++;
            if (cuentaFramesTraslacion >= LIMITE_FRAMES_TRASLACION) {
                cambioMapa = 0;
            }
        }
        else {
            if (muerto) {
                animacionActiva = 1;
                Caer();
            }
            else if (enfadado) {
                enfadar();
                enfadado = false;
            }
            //Si va a la izquierda
            else if (direccionX == 0) {
                //Si va hacia abajo
                if (direccionY == 0) {
                    MoverIzqAbajo();
                }
                //Si va hacia arriba
                else {
                    MoverIzqArriba();
                }
            }
            //Si va hacia la derecha
            else if (direccionX == 1) {
                //Si va hacia abajo
                if (direccionY == 0) {
                    MoverDerAbajo();
                }
                //Si va hacia arriba
                else {
                    MoverDerArriba();
                }
            }

            //Actualizar posicion no salir de la pantalla
            if (destRec.y > 450) {
                destRec.y = 40;
                enElAire = true;
                cayendo = true;
            }
            else if (destRec.y < 20) {
                destRec.y = 430;
            }
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
                if (indiceAnimacion == 3) {
                    if (!muertePorAgua) {
                        Frutas f = Frutas();
                        f = Frutas("resources/frutas/cereza.png", 1.0f, 2.0f, (unsigned int)500, 60, destRec, admin->scores);
                        admin->frutas.push_back(std::make_shared<Frutas>(f));
                    }
                    borrame = true;
                }
                break;
            case 2:
                //Actualizar width&height animacion
                indiceAnimacion = (indiceAnimacion + 1) % fAnimation[2];
                widthAnimation = waterAnimation.width / fAnimation[2];
                heightAnimation = waterAnimation.height;
                break;
            case 3:
                //Actualizar width&height animacion
                indiceAnimacion = (indiceAnimacion + 1) % fAnimation[3];
                widthAnimation = angryAnimation.width / fAnimation[3];
                heightAnimation = angryAnimation.height;
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
    void MoverIzqArriba() {
        destRec.x -= velocidadLateral;
        destRec.y -= velocidadSalto;
        srcRec.width = pixels;
    }
    void MoverIzqAbajo() {
        destRec.x -= velocidadLateral;
        destRec.y += velocidadSalto;
        srcRec.width = pixels;
    }

    void MoverDerArriba() {
        destRec.x += velocidadLateral;
        destRec.y -= velocidadSalto;
        srcRec.width = -pixels;
    }
    void MoverDerAbajo() {
        destRec.x += velocidadLateral;
        destRec.y += velocidadSalto;
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
        if (cambioMapa == 0) {
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
                switch (s.aproach[enemyNum + 2]) {
                case 1:
                    //Derecha
                    //destRec.x = s.left - destRec.width / 2;
                    direccionX = 0; //Colisiona derecha, ahora se mueve izquierda
                    //Se puede a�adir un movimiento random en eje Y
                    break;
                case 2:
                    //Izquierda
                    //destRec.x = s.right + destRec.width / 2;
                    direccionX = 1; //Colisiona izquierda, hora se mueve derecha
                    //Se puede a�adir un movimiento random en eje Y
                    break;
                case 3:
                    //Abajo
                    //destRec.y = s.top - destRec.height / 2;
                    lastGround = s;
                    direccionY = 1; //Colisiona abajo, ahora se mueve arriba
                    //Se puede a�adir un movimiento random en eje X
                    break;
                case 4:
                    //Arriba
                    //destRec.y = s.bot + destRec.height / 2;
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
                    s.aproach[enemyNum + 2] = 1;
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
                    s.aproach[enemyNum + 2] = 2;
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
                    s.aproach[enemyNum + 2] = 3;
                }
                //Abajo
                else {
                    //Si no se cumplen anteriores asumimos que se acerca por debajo
                    s.aproach[enemyNum + 2] = 4;
                }
            }
        }
    }


    //Comprobacion de si debe caer
    void compruebaSuelo() override {
        if (cambioMapa == 0) {
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
    }

    void compruebaPared(const Columnas& s) override {
        if (cambioMapa == 0) {
            //Comprobamos columna derecha
            if (s.left_der < (destRec.x + destRec.width / 2)) {
                destRec.x = s.left_der - destRec.width / 2;
                direccionX = 0;
            }
            //Comprobamos columna izquierda
            else if (s.right_izq > (destRec.x - destRec.width / 2)) {
                destRec.x = s.right_izq + destRec.width / 2;
                direccionX = 1;
            }
        }
    }
};

typedef std::shared_ptr<Morado> sh_Morado;