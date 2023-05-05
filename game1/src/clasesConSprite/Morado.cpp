#pragma once
#include"AdministradorPompas.cpp"
#include "Enemigo.cpp"


class Morado : public Enemigo {
public:
    //Gestión de transición de nivel
    int8_t cambioMapa = 2; //2->Primera Iteración 1->Desplazándose 0->Ya no
    Rectangle posicionPartida = { 400, (float)50, 32, 32 };
    Rectangle posicionDestino = { 400, (float)50, 32, 32 };
    int cuentaFramesTraslacion = 0; //3 segundos = 3 * 60 frames = 180 frames
    const int LIMITE_FRAMES_TRASLACION = 180; //3 segundos = 3 * 60 frames = 180 frames
    double razonX = 0;
    double razonY = 0;
    bool primeraActualizacion = true;
    ////////////////////
    
    //Sprite pixels
    int pixels = 16; //El numero de pixeles del sprite

    //Animation
    Texture2D walkAnimation = LoadTexture("resources/enemyMorado/moradoWalk.png");
    Texture2D deadAnimation = LoadTexture("resources/enemyMorado/moradoDead.png");
    Texture2D waterAnimation = LoadTexture("resources/enemyMorado/moradoWater.png");
    Texture2D angryAnimation = LoadTexture("resources/enemyMorado/moradoAngry.png");
    Texture2D animations[4] = { walkAnimation, deadAnimation, waterAnimation, angryAnimation };

    int fWalkAnimation = 2; //Número de fotogramas de la animacion camniar
    int fDeadAnimation = 4; //Número de fotogramas de la animacion muerte
    int fWaterAnimation = 2; //Número de fotogramas de la animacion muerte agua
    int fAngryAnimation = 2; //Número de fotogramas de la animacion enfado
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
    Rectangle player;

    //Variables lógicas
    int direccionX = 0; //0->izquierda, 1->derecha
    int direccionY = 0; //0->abajo, 1->arriba
    AdministradorPompas* admin;
    bool frutaProducida = false;
    int num = 0;
    int prob = 90;
    bool IAoriginal = false;

    //Muerto -> Ahora esta en Enemigo
    //bool muerto = false;

    Morado() = default;

    Morado(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral, float _targetFPS, Rectangle destino, AdministradorPompas& admin, int modoIA) {
        Inicializador(rutaTextura, tamano, saltoMax, velSalto, velLateral);
        if (enfadado) {
            animacionActiva = 3;
        }
        destRec = posicionPartida;
        posicionDestino = destino;
        tipo = 4;
        widthAnimation = walkAnimation.width / fWalkAnimation;
        heightAnimation = walkAnimation.height;
        targetFrames = _targetFPS;
        enElAire = true;
        cayendo = true;
        this->admin = &admin;
        srand(time(NULL));
        player = destino;
        if (modoIA == 0) {
            IAoriginal = true;
        }
        else {
            IAoriginal = false;
        }
        if (((float)rand() / (float)RAND_MAX) < 0.5) {
            velXRebote = -(0.001 + (0.0025 * ((float)rand() / (float)RAND_MAX))) * GetScreenWidth();
        }
        else {
            velXRebote = (0.001 + (0.0025 * ((float)rand() / (float)RAND_MAX))) * GetScreenWidth();
        }
        velYRebote = -(0.002 + (0.005 * ((float)rand() / (float)RAND_MAX))) * GetScreenHeight();
    };

    void enfadar() {
        animacionActiva = 3;
    }

    // Controlador de comportamiento
    void Actualizar(Rectangle playerPosition1, Rectangle playerPosition2) override {
        if (lastHeight != GetScreenHeight()) {
            destRec.height = GetScreenHeight() / 14.0625f;
            destRec.y = GetScreenHeight() * (destRec.y / lastHeight);
            velYRebote = GetScreenHeight() * (velYRebote / lastHeight);
            deathHeight = GetScreenHeight() * (deathHeight / lastHeight);
            posicionDestino.y = GetScreenHeight() * (posicionDestino.y / lastHeight);
            if (cambioMapa > 0 && !primeraActualizacion) {
                razonY = ((posicionDestino.y - posicionDestino.y * 0.05) - destRec.y) / (LIMITE_FRAMES_TRASLACION - cuentaFramesTraslacion);
            }
            origin.y = destRec.height / 2;
            lastHeight = GetScreenHeight();
        }
        if (lastWidth != GetScreenWidth()) {
            destRec.width = GetScreenWidth() / 25.0f;
            destRec.x = GetScreenWidth() * (destRec.x / lastWidth);
            velXRebote = GetScreenWidth() * (velXRebote / lastWidth);
            posicionDestino.x = GetScreenWidth() * (posicionDestino.x / lastWidth);
            if (cambioMapa > 0 && !primeraActualizacion) {
                razonX = (posicionDestino.x - destRec.x) / (LIMITE_FRAMES_TRASLACION - cuentaFramesTraslacion);
            }
            origin.x = destRec.width / 2;
            lastWidth = GetScreenWidth();
        }

        //Posicion para pasarle a BOB
        posicion = destRec;

        if (cambioMapa > 0) {
            if (cambioMapa == 2) {
                primeraActualizacion = false;
                cambioMapa = 1;
                razonX = (posicionDestino.x - destRec.x) / LIMITE_FRAMES_TRASLACION;
                razonY = (posicionDestino.y - destRec.y) / LIMITE_FRAMES_TRASLACION;
            }
            destRec.x += razonX;
            destRec.y += razonY;

            cuentaFramesTraslacion++;
            if (cuentaFramesTraslacion >= LIMITE_FRAMES_TRASLACION) {
                cambioMapa = 0;
            }
        }else if (rebotando) {
            animacionActiva = 1;
            destRec.x += velXRebote;
            destRec.y += velYRebote;
            if (deathHeight == 0) {
                deathHeight = destRec.y;
            }
            if (destRec.y < 0) {
                velYRebote += 0.0001 * GetScreenHeight();
            }
            else {
                velYRebote += 0.00005 * GetScreenHeight();
            }
            if (deathHeight < destRec.y) {
                rebotando = false;
            }
        }else {
            //Obtener el rectangulo del jugador a seguir
            Rectangle playerPosition;
            if (playerPosition2.x == -1 && playerPosition2.y == -1) {
                playerPosition = playerPosition1;
            }
            else if (playerPosition1.x == -1 && playerPosition1.y == -1) {
                playerPosition = playerPosition2;
            }
            else if (destRec.y <= playerPosition1.y && destRec.y + destRec.height >= playerPosition1.y && !(destRec.y <= playerPosition2.y && destRec.y + destRec.height >= playerPosition2.y)) {
                playerPosition = playerPosition1;
            }
            else if (destRec.y <= playerPosition2.y && destRec.y + destRec.height >= playerPosition2.y && !(destRec.y <= playerPosition1.y && destRec.y + destRec.height >= playerPosition1.y)) {
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

            player = playerPosition;

            if (enfadado) {
                animacionActiva = 3;
                velocidadLateral = 1.5 * destRec.width / 16.0f;
                velocidadSalto = destRec.height / 10.0f;
            }
            else {
                velocidadLateral = destRec.width / 16.0f;
                velocidadSalto = destRec.height / 10.0f;
            }

            if (muerto) {
                animacionActiva = 1;
                CaerLento();
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
            if (destRec.y > GetScreenHeight() + 50) {
                destRec.y = -10;
                enElAire = true;
                cayendo = true;
            }
            else if (destRec.y < -50) {
                destRec.y = GetScreenHeight() + 5;
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
        if ( (cambioMapa == 0) && (!rebotando)) {
            //Comprobamos si colisiona con la superficie
            if (
                (
                    //Comprobamos colision esquina inferior derecha
                    (((s.bot) > (destRec.y + destRec.height * 0.49)) &&
                        ((destRec.y + destRec.height * 0.49) > (s.top))
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
                    (((s.bot) > (destRec.y + destRec.height * 0.49)) &&
                        ((destRec.y + destRec.height * 0.49) > (s.top))
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
                    num = 1 + rand() % (101 - 1);
                    if (!IAoriginal && num > prob) {
                        if (player.y > destRec.y) {
                            direccionY = 1;
                        }
                        else {
                            direccionY = 0;
                        }
                    }
                    //Se puede añadir un movimiento random en eje Y
                    break;
                case 2:
                    //Izquierda
                    //destRec.x = s.right + destRec.width / 2;
                    direccionX = 1; //Colisiona izquierda, hora se mueve derecha
                    num = 1 + rand() % (101 - 1);
                    if (!IAoriginal && num > prob) {
                        if (player.y > destRec.y) {
                            direccionY = 1;
                        }
                        else {
                            direccionY = 0;
                        }
                    }
                    //Se puede añadir un movimiento random en eje Y
                    break;
                case 3:
                    //Abajo
                    //destRec.y = s.top - destRec.height / 2;
                    lastGround = s;
                    direccionY = 1; //Colisiona abajo, ahora se mueve arriba
                    num = 1 + rand() % (101 - 1);
                    if (!IAoriginal && num > prob) {
                        if (player.x > destRec.x) {
                            direccionX = 1;
                        }
                        else {
                            direccionX = 0;
                        }
                    }
                    //Se puede añadir un movimiento random en eje X
                    break;
                case 4:
                    //Arriba
                    //destRec.y = s.bot + destRec.height / 2;
                    direccionY = 0; //Colisiona arriba, ahora se mueve abajo
                    num = 1 + rand() % (101 - 1);
                    if (!IAoriginal && num > prob) {
                        if (player.x > destRec.x) {
                            direccionX = 1;
                        }
                        else {
                            direccionX = 0;
                        }
                    }
                    //Se puede añadir un movimiento random en eje X
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
                        (((s.bot) > (destRec.y + destRec.height * 0.49)) &&
                            ((destRec.y + destRec.height * 0.49) > (s.top))
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
                        (((s.bot) > (destRec.y + destRec.height * 0.49)) &&
                            ((destRec.y + destRec.height * 0.49) > (s.top))
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
        if ((cambioMapa == 0) && (!rebotando)) {
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
                if (!frutaProducida) {
                    frutaProducida = true;
                    //std::cout << "-------------------------------     " + std::to_string(killCount) << std::endl;
                    //while (true) {}
                    Rectangle  aux = destRec;
                    if (muertePorRayo) {
                        Frutas f = Frutas();
                        f = Frutas("resources/frutas/platano.png", 1.0f, 2.0f, (unsigned int)8000, 60, aux, admin->scores, lastHeight, lastWidth, razonX, razonY, origin);
                        admin->frutas.push_back(std::make_shared<Frutas>(f));
                    }
                    else if (muertePorAgua) {
                        Frutas f = Frutas();
                        f = Frutas("resources/frutas/platano.png", 1.0f, 2.0f, (unsigned int)7000, 60, aux, admin->scores, lastHeight, lastWidth, razonX, razonY, origin);
                        admin->frutas.push_back(std::make_shared<Frutas>(f));
                    }
                    else if (killCount == 0) {
                        Frutas f = Frutas();
                        f = Frutas("resources/frutas/platano.png", 1.0f, 2.0f, (unsigned int)500, 60, aux, admin->scores, lastHeight, lastWidth, razonX, razonY, origin);
                        admin->frutas.push_back(std::make_shared<Frutas>(f));
                    }
                    else if (killCount == 1) {
                        Frutas f = Frutas();
                        f = Frutas("resources/frutas/platano.png", 1.0f, 2.0f, (unsigned int)1000, 60, aux, admin->scores, lastHeight, lastWidth, razonX, razonY, origin);
                        admin->frutas.push_back(std::make_shared<Frutas>(f));

                    }
                    else if (killCount == 2) {
                        Frutas f = Frutas();
                        f = Frutas("resources/frutas/platano.png", 1.0f, 2.0f, (unsigned int)2000, 60, aux, admin->scores, lastHeight, lastWidth, razonX, razonY, origin);
                        admin->frutas.push_back(std::make_shared<Frutas>(f));

                    }
                    else if (killCount == 3) {
                        Frutas f = Frutas();
                        f = Frutas("resources/frutas/platano.png", 1.0f, 2.0f, (unsigned int)3000, 60, aux, admin->scores, lastHeight, lastWidth, razonX, razonY, origin);
                        admin->frutas.push_back(std::make_shared<Frutas>(f));

                    }
                    else if (killCount == 4) {
                        Frutas f = Frutas();
                        f = Frutas("resources/frutas/platano.png", 1.0f, 2.0f, (unsigned int)4000, 60, aux, admin->scores, lastHeight, lastWidth, razonX, razonY, origin);
                        admin->frutas.push_back(std::make_shared<Frutas>(f));

                    }
                    else if (killCount == 5) {
                        Frutas f = Frutas();
                        f = Frutas("resources/frutas/platano.png", 1.0f, 2.0f, (unsigned int)5000, 60, aux, admin->scores, lastHeight, lastWidth, razonX, razonY, origin);
                        admin->frutas.push_back(std::make_shared<Frutas>(f));

                    }
                    else if (killCount == 6) {
                        Frutas f = Frutas();
                        f = Frutas("resources/frutas/platano.png", 1.0f, 2.0f, (unsigned int)6000, 60, aux, admin->scores, lastHeight, lastWidth, razonX, razonY, origin);
                        admin->frutas.push_back(std::make_shared<Frutas>(f));

                    }

                }
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
                if (rebotando) {
                    velXRebote = -velXRebote;
                }
            }
            //Comprobamos columna izquierda
            else if (s.right_izq > (destRec.x - destRec.width / 2)) {
                destRec.x = s.right_izq + destRec.width / 2;
                direccionX = 1;
                if (rebotando) {
                    velXRebote = -velXRebote;
                }
            }
        }
    }
};

typedef std::shared_ptr<Morado> sh_Morado;