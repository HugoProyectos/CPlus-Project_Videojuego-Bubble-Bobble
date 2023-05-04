#pragma once
#include "AdministradorPompas.cpp"
#include "Enemigo.cpp"

class Rosa : public Enemigo {
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
    Texture2D walkAnimation = LoadTexture("resources/enemyRosa/rosaWalk.png");
    Texture2D deadAnimation = LoadTexture("resources/enemyRosa/rosaDead.png");
    Texture2D waterAnimation = LoadTexture("resources/enemyRosa/rosaWater.png");
    Texture2D angryAnimation = LoadTexture("resources/enemyRosa/rosaAngry.png");
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

    //Variables lógicas
    int direccionX = 0; //0->izquierda, 1->derecha
    int direccionY = 0; //0->abajo, 1->arriba
    int velocidadX = 0; //0->izquierda, 1->derecha
    int velocidadY = 0; //0->abajo, 1->arriba
    int colision = 0; //0->No hay colision, 1->colisiona con paredes(debe variar el movimiento vertical), 2->colision techo(varia movimeitno horizontal)
    int colisionAux = 0;
    bool frutaGenerada = false;
    AdministradorPompas* admin;
    float velMax = 0;
    float velMin = 0;
    float anchosX = 15.0f;
    float anchosY = 15.0f; 
    bool frutaProducida = false;
    int num = 0;
    int contadorCambio = 0;
    bool IAoriginal = false;

    //Muerto -> Ahora esta en Enemigo
    //bool muerto = false;

    Rosa(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral, float _targetFPS, Rectangle destino, AdministradorPompas& admin, int modoIA) {
        Inicializador(rutaTextura, tamano, saltoMax, velSalto, velLateral);
        if (enfadado) {
            animacionActiva = 3;
        }
        velMax = velLateral;
        velMin = velLateral / 2;
        srand(time(NULL));
        num = (rand() % (int)(velMax - velMin)) + velMin;
        velocidadX = num;
        if (rand() % 2 == 0) {
            velocidadX = -velocidadX;
        }
        velocidadY = velMax;

        destRec = posicionPartida;
        posicionDestino = destino;
        tipo = 5;
        widthAnimation = walkAnimation.width / fWalkAnimation;
        heightAnimation = walkAnimation.height;
        targetFrames = _targetFPS;
        enElAire = true;
        cayendo = true;
        this->admin = &admin;
        if (modoIA == 0) {
            IAoriginal = true;

        }
        else {
            IAoriginal = false;

        }
    };

    void enfadar() {
        animacionActiva = 3;
    }

    // Controlador de comportamiento
    void Actualizar(Rectangle playerPosition1, Rectangle playerPosition2) override {
        if (lastHeight != GetScreenHeight()) {
            destRec.height = GetScreenHeight() / 14.0625f;
            destRec.y = GetScreenHeight() * (destRec.y / lastHeight);
            posicionDestino.y = GetScreenHeight() * (posicionDestino.y / lastHeight);
            if (cambioMapa > 0 && !primeraActualizacion) {
                razonY = ((posicionDestino.y - posicionDestino.y * 0.05) - destRec.y) / (LIMITE_FRAMES_TRASLACION - cuentaFramesTraslacion);
            }
            distanciaSaltoMax = distanciaSaltoMax * ((float)GetScreenHeight() / (float)lastHeight);
            anchosY = anchosY * ((float)GetScreenHeight() / (float)lastHeight);
            origin.y = destRec.height / 2;
            lastHeight = GetScreenHeight();
        }
        if (lastWidth != GetScreenWidth()) {
            destRec.width = GetScreenWidth() / 25.0f;
            destRec.x = GetScreenWidth() * (destRec.x / lastWidth);
            posicionDestino.x = GetScreenWidth() * (posicionDestino.x / lastWidth);
            if (cambioMapa > 0 && !primeraActualizacion) {
                razonX = (posicionDestino.x - destRec.x) / (LIMITE_FRAMES_TRASLACION - cuentaFramesTraslacion);
            }
            anchosX = anchosX * ((float)GetScreenWidth() / (float)lastWidth);
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
        }
        else {
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

            if (IAoriginal) {
                if (enfadado) {
                    animacionActiva = 3;
                    velocidadLateral = 1.5f * destRec.width / 16.0f;
                    velocidadSalto = 1.5f * destRec.height / 10.0f;
                    velMax = velocidadLateral;
                    velMin = velocidadLateral / 2;
                }
                else {
                    velocidadLateral = destRec.width / 16.0f;
                    velocidadSalto = destRec.height / 10.0f;
                    velMax = velocidadLateral;
                    velMin = velocidadLateral / 2;
                }

                if (muerto) {
                    animacionActiva = 1;
                    CaerLento();
                }
                else if (contadorCambio >= 120 || (contadorCambio>=60 && enfadado)) {
                    RecalcularVelocidad();
                    contadorCambio = 0;
                }
                else {
                    MoverRebotes();
                    contadorCambio++;
                }
            }
            else {
                if (enfadado) {
                    animacionActiva = 3;
                    velocidadLateral = 1.5f * destRec.width / 16.0f;
                    velocidadSalto = 1.5f * destRec.height / 10.0f;
                    velMax = velocidadLateral;
                    velMin = velocidadLateral / 2;
                }
                else {
                    velocidadLateral = destRec.width / 16.0f;
                    velocidadSalto = destRec.height / 10.0f;
                    velMax = velocidadLateral;
                    velMin = velocidadLateral / 2;
                }

                if (muerto) {
                    animacionActiva = 1;
                    CaerLento();
                }
                else if (colision == 0) {
                    SeguirJugador(playerPosition);
                }
                else if (colision == 2) {
                    MoverHorizontal(playerPosition);
                }
                else if (colision == 1) {
                    MoverVertical(playerPosition);
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
        colision = 0;
        colisionAux = 0;
    }

    void Dibujar() override {
        srcRec.x = (float)widthAnimation * (float)indiceAnimacion;
        DrawTexturePro(animations[animacionActiva], srcRec, destRec, origin, 0.0f, WHITE);
    }

    //Funcion de IA original
    void  MoverRebotes() {
        if (velocidadX > 0) {
            srcRec.width = -pixels;
        }
        else {
            srcRec.width = pixels;
        }
        destRec.y += velocidadY;
        destRec.x += velocidadX;
    }

    void RecalcularVelocidad() {
        velocidadX = (rand() % (int)(velMax - velMin)) + velMin;
        if (rand() % 2 == 0) {
            velocidadX = -velocidadX;
        }

        velocidadY = (rand() % (int)(velMax - velMin)) + velMin;
        if (rand() % 2 == 0) {
            velocidadY = -velocidadY;
        }
    }

    //funciones de comportamiento
    void SeguirJugador(Rectangle playerPosition) {
        //El movimiento vertical depende del jugador
        if (destRec.y > playerPosition.y + anchosY) {
            //Estamos por debajo del player, restamos posicion
            direccionY = 0;
            destRec.y -= velocidadLateral;
        }
        else if (destRec.y < playerPosition.y - anchosY) {
            //Estamos por encima del player, sumamos posicion
            direccionY = 1;
            destRec.y += velocidadLateral;
        }

        //El movimiento horizontal depende del jugador
        if (destRec.x > playerPosition.x + anchosX) {
            //Estamos por izq del player, restamos posicion
            direccionX = 0;
            destRec.x -= velocidadLateral;
        }
        else if (destRec.x < playerPosition.x - anchosX) {
            //Estamos por derecha del player, sumamos posicion
            direccionX = 1;
            destRec.x += velocidadLateral;
        }


        //Width
        srcRec.width = pixels;
    }

    void MoverHorizontal(Rectangle playerPosition) {
        //El movimiento vertical depende del jugador
        if (destRec.y > playerPosition.y + anchosY) {
            //Estamos por debajo del player, restamos posicion
            if (colisionAux == 4) {
                destRec.y -= 0.1;
            }
            else {
                destRec.y -= velocidadLateral;
            }
        }
        else if (destRec.y < playerPosition.y - anchosY) {
            //Estamos por encima del player, sumamos posicion
            if (colisionAux == 3) {
                destRec.y += 0.1;
            }
            else {
                destRec.y += velocidadLateral;
            }
        }
        else {
            destRec.y -= velocidadLateral;
        }
        //El movimiento horizontal depende del sentido
        if (direccionX == 0) {
            //Izquierda
            destRec.x -= velocidadLateral;
        }
        else {
            //Derecha
            destRec.x += velocidadLateral;
        }

        //Width
        srcRec.width = pixels;
    }

    void MoverVertical(Rectangle playerPosition) {
        //El movimiento horizontal depende del jugador
        if (destRec.x > playerPosition.x + anchosX) {
            //Estamos por izq del player, restamos posicion
            if (colisionAux == 1) {
                destRec.x -= 0.1;
            }
            else {
                destRec.x -= velocidadLateral;
            }
        }
        else if (destRec.x < playerPosition.x - anchosX) {
            //Estamos por derecha del player, sumamos posicion
            if (colisionAux == 2) {
                destRec.x += 0.1;
            }
            else {
                destRec.x += velocidadLateral;
            }
        }
        //El movimiento vertical depende del sentido
        if (direccionY == 0) {
            //Abajo
            destRec.y -= velocidadLateral;
        }
        else {
            //Arriba
            destRec.y += velocidadLateral;
        }

        //Width
        srcRec.width = pixels;
    }

    void Caer() {
        destRec.y += velocidadSalto;
    }

    void CaerLento() {
        destRec.y += velocidadSalto / 2;
    }

    //Comprobacion de colisiones
    void compruebaColision(Plataforma& s, int enemyNum) override {
        if (cambioMapa == 0) {
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
                    destRec.x = s.left - destRec.width / 2;
                    direccionX = 0; //Colisiona derecha, ahora se mueve izquierda
                    colision = 1;

                    if (IAoriginal) {
                        contadorCambio = 0;
                        velocidadX = -velMax;
                        velocidadY = (rand() % (int)(velMax-velMin)) + velMin;
                        if (rand() % 2 == 0) {
                            velocidadY = -velocidadY;
                        }
                    }
                    //Se puede añadir un movimiento random en eje Y
                    break;
                case 2:
                    //Izquierda
                    destRec.x = s.right + destRec.width / 2;
                    direccionX = 1; //Colisiona izquierda, hora se mueve derecha
                    colision = 1;

                    if (IAoriginal) {
                        contadorCambio = 0;
                        velocidadX = velMax;
                        velocidadY = (rand() % (int)(velMax - velMin)) + velMin;
                        if (rand() % 2 == 0) {
                            velocidadY = -velocidadY;
                        }
                    }
                    //Se puede añadir un movimiento random en eje Y
                    break;
                case 3:
                    //Abajo
                    destRec.y = s.top - destRec.height / 2;
                    lastGround = s;
                    direccionY = 1; //Colisiona abajo, ahora se mueve arriba
                    colision = 2;

                    if (IAoriginal) {
                        contadorCambio = 0;
                        velocidadY = -velMax;
                        velocidadX = (rand() % (int)(velMax - velMin)) + velMin;
                        if (rand() % 2 == 0) {
                            velocidadX = -velocidadX;
                        }
                    }

                    //Se puede añadir un movimiento random en eje X
                    break;
                case 4:
                    //Arriba
                    destRec.y = s.bot + destRec.height / 2;
                    direccionY = 0; //Colisiona arriba, ahora se mueve abajo
                    colision = 2;

                    if (IAoriginal) {
                        contadorCambio = 0;
                        velocidadY = velMax;
                        velocidadX = (rand() % (int)(velMax - velMin)) + velMin;
                        if (rand() % 2 == 0) {
                            velocidadX = -velocidadX;
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
                    colisionAux = 1;
                    colision = 1;

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
                    colisionAux = 2;
                    colision = 1;

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
                    colisionAux = 3;
                    colision = 2;

                }
                //Abajo
                else if (
                    //Comprobamos colision esquina inferior derecha
                    (
                        (((s.bot) > (destRec.y - destRec.height / 2 - 5)) &&
                            ((destRec.y - destRec.height / 2 - 5) > (s.top))
                            ) && (
                                ((s.right) > (destRec.x + destRec.width / 2)) &&
                                ((destRec.x + destRec.width / 2) > (s.left))
                                )
                        )
                    ||
                    //Comprobamos colision esquina inferior izquierda
                    (
                        (((s.bot) > (destRec.y - destRec.height / 2 - 5)) &&
                            ((destRec.y - destRec.height / 2 - 5) > (s.top))
                            ) && (
                                ((s.right) > (destRec.x - destRec.width / 2)) &&
                                ((destRec.x - destRec.width / 2) > (s.left))
                                )
                        )
                    ) {
                    //Si no se cumplen anteriores asumimos que se acerca por debajo
                    s.aproach[enemyNum + 2] = 4;
                    colisionAux = 4;
                    colision = 2;
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
                if (!muertePorAgua && !frutaProducida) {
                    frutaProducida = true;
                    Rectangle  aux = destRec;
                    if (killCount == 0) {
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
            }
            //Comprobamos columna izquierda
            else if (s.right_izq > (destRec.x - destRec.width / 2)) {
                destRec.x = s.right_izq + destRec.width / 2;
                direccionX = 1;
            }
        }
    }
};

typedef std::shared_ptr<Rosa> sh_Rosa;