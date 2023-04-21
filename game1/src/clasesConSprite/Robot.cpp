#pragma once
#include"AdministradorPompas.cpp"
#include "Enemigo.cpp"


class Robot : public Enemigo {
public:
    //Gestión de transición de nivel
    int8_t cambioMapa = 2; //2->Primera Iteración 1->Desplazándose 0->Ya no
    Rectangle posicionPartida = { (float)GetScreenWidth()/2, (float)50, 32, 32};
    int cuentaFramesTraslacion = 0; //3 segundos = 3 * 60 frames = 180 frames
    const int LIMITE_FRAMES_TRASLACION = 180; //3 segundos = 3 * 60 frames = 180 frames
    double razonX = 0;
    double razonY = 0;
    ////////////////////
    
    //Sprite pixels
    int pixels = 16; //El numero de pixeles del sprite
    
    //Animation
    Texture2D walkAnimation = LoadTexture("resources/enemyRobot/robotWalk.png");
    Texture2D deadAnimation = LoadTexture("resources/enemyRobot/robotDead.png");
    Texture2D waterAnimation = LoadTexture("resources/enemyRobot/robotWater.png");
    Texture2D angryAnimation = LoadTexture("resources/enemyRobot/robotAngry.png");
    Texture2D animations[4] = { walkAnimation, deadAnimation, waterAnimation, angryAnimation };

    int fWalkAnimation = 4; //Número de fotogramas de la animacion camniar
    int fDeadAnimation = 4; //Número de fotogramas de la animacion muerte
    int fWaterAnimation = 4; //Número de fotogramas de la animacion muerte agua
    int fAngryAnimation = 4; //Número de fotogramas de la animacion enfado
    int fAnimation[4] = { fWalkAnimation , fDeadAnimation, fWaterAnimation, fAngryAnimation };

    int widthAnimation; // Se actualiza para cada animación activa
    int heightAnimation;

    int animacionActiva = 0; //Indica la animación activa: 0->WalkAnimation, 1->DeadAnimation, 2->WaterAniamtion, 3->AngryAnimation
    int indiceAnimacion = 0; //Indica el número de frame actual de la animación activa

    //Frames
    int targetFrames;
    int cuentaFrames = 0;
    int velocidadFrames = 4;

    //Colisiones
    Plataforma lastGround;
    AdministradorPompas* admin;


    //Lógica
    int direccionX = 1; //0 para izquierda, 1 para derecha

    //Muerto -> Ahora esta en Enemigo
    //bool muerto = false;

    Robot(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral, float _targetFPS, Rectangle destino, AdministradorPompas& admin) {
        Inicializador(rutaTextura, tamano, saltoMax, velSalto, velLateral);
        if (enfadado) {
            animacionActiva = 3;
            velocidadLateral *= 2;
            enfadado = false;
        }
        destRec = destino;
        tipo = 1;
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
    if (cambioMapa > 0){
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
    } else {
            if (muerto) {
                animacionActiva = 1;
                Caer();
            }
            else if (enfadado) {
                enfadar();
                enfadado = false;
            }
            else if (!saltando && enElAire) {
                CaerLento();
            }
            else if (saltando || (destRec.y > playerPosition.y && destRec.x > playerPosition.x - 10 && destRec.x < playerPosition.x + 10)) { //Si el personaje esta encima
                Salto();
            }
            else if (destRec.y != playerPosition.y) {
                if (direccionX == 0) {
                    //Izquierda
                    MoverIzq();
                }
                else {
                    //Derecha
                    MoverDer();
                }
            }
            else if (destRec.x > playerPosition.x + 5) { //Si el personaje esta a la izquierda
                MoverIzq();
            }
            else if (destRec.x < playerPosition.x - 5) { //Si el personaje esta a la derecha
                MoverDer();
            }

            //Actualizar posicion no salir de la pantalla
            if (destRec.y > 500) {
                destRec.y = -10;
                enElAire = true;
                cayendo = true;
            }
            else if (destRec.y < -50) {
                destRec.y = 450;
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
    void MoverIzq() {
        destRec.x -= velocidadLateral;
        srcRec.width = pixels;
        direccionX = 0;
    }

    void MoverDer() {
        destRec.x += velocidadLateral;
        srcRec.width = -pixels;
        direccionX = 1;
    }

    void Caer() {
        destRec.y += velocidadSalto;
    }

    void CaerLento() {
        destRec.y += velocidadSalto/2;
    }

    void Salto() {
        //Gestión de salto
        if (!saltando) {
            //std::cout << "Inicio Salto" << std::endl;
            saltando = true;
            finSaltando = false;
            enElAire = true;
            cayendo = false;
        }
        //Subiendo
        else if ((saltoRecorrido <= distanciaSaltoMax) && !finSaltando) {
            //std::cout << "Subida Salto" << std::endl;
            destRec.y -= velocidadSalto;
            saltoRecorrido += velocidadSalto;
        }
        //Hemos llegado al máximo
        else if (saltoRecorrido >= distanciaSaltoMax) {
            //std::cout << "max salto" << std::endl;
            finSaltando = true;
            destRec.y += velocidadSalto;
            saltoRecorrido -= velocidadSalto;
        }
        //Bajar
        else if (saltoRecorrido > 0 && finSaltando && enElAire) {
            //std::cout << "Bajar Salto" << std::endl;
            destRec.y += velocidadSalto;
            saltoRecorrido -= velocidadSalto;
        }
        else if (saltoRecorrido <= 0) {
            //std::cout << " Salto acabado" << std::endl;
            saltando = false;
            finSaltando = true;
        }
        else {
            //std::cout << " Salto perdido" << std::endl;

        }
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
                    destRec.x = s.left - destRec.width / 2;
                    direccionX = 0; //Colisiona derecha, ahora se mueve izquierda
                    //Se puede añadir un movimiento random en eje Y
                    break;
                case 2:
                    //Izquierda
                    destRec.x = s.right + destRec.width / 2;
                    direccionX = 1; //Colisiona izquierda, hora se mueve derecha
                    //Se puede añadir un movimiento random en eje Y
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
                animacionActiva = 1;
                enElAire = false;
                cayendo = false;

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

typedef std::shared_ptr<Robot> sh_Robot;