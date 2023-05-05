#pragma once
#include "Enemigo.cpp"
#include "Bola.cpp"
#include "AdministradorPompas.cpp"
#include <ctime>

class Fantasma : public Enemigo {
public:
    //Gestión de transición de nivel
    int8_t cambioMapa = 2; //2->Primera Iteración 1->Desplazándose 0->Ya no
    Rectangle posicionPartida = { (float)400, (float)50, 32, 32 };
    Rectangle posicionDestino = { (float)400, (float)50, 32, 32 };
    int cuentaFramesTraslacion = 0; //3 segundos = 3 * 60 frames = 180 frames
    const int LIMITE_FRAMES_TRASLACION = 180; //3 segundos = 3 * 60 frames = 180 frames
    double razonX = 0;
    double razonY = 0;
    bool primeraActualizacion = true;
    ////////////////////

    //Sprite pixels
    int pixels = 16; //El numero de pixeles del sprite

    //Animation
    Texture2D walkAnimation = LoadTexture("resources/enemyFantasma/fantasmaWalk.png");
    Texture2D angryWalkAnimation = LoadTexture("resources/enemyFantasma/fantasmaAngryWalk.png");
    Texture2D deadAnimation = LoadTexture("resources/enemyFantasma/fantasmaMuerte.png");
    Texture2D angryBallAnimation = LoadTexture("resources/enemyFantasma/fantasmaAngryBola.png");
    Texture2D ballAnimation = LoadTexture("resources/enemyFantasma/fantasmaBola.png");
    Texture2D animations[6] = { walkAnimation, deadAnimation, ballAnimation, angryWalkAnimation, angryBallAnimation };

    //sh_Enemigo bola = std::make_shared<Bola>(Bola("resources/enemyBola/bolaBasic.png", 2.0f, 40.0f, 1.0f, 1.0f, targetFrames, destRec, dir));

    int indexBolas = 0;
    
    int fWalkAnimation = 2; //Número de fotogramas de la animacion camniar
    int fAngryWalkAnimation = 4; //Número de fotogramas de la animacion camniar
    int fDeadAnimation = 2; //Número de fotogramas de la animacion muerte
    int fBallAnimation = 4;
    int fAngryBallAnimation = 4;
    int fAnimation[6] = { fWalkAnimation , fDeadAnimation, fBallAnimation, fAngryWalkAnimation, fAngryBallAnimation };

    int widthAnimation; // Se actualiza para cada animación activa
    int heightAnimation;

    int animacionActiva = 0; //Indica la animación activa: 0->WalkAnimation, 1->DeadAnimation, 2->BallLeft,
    int indiceAnimacion = 0; //Indica el número de frame actual de la animación activa

    //Frames
    int targetFrames;
    int cuentaFrames = 0;
    int velocidadFrames = 2;

    //Del propio fantasma
    bool disparando;
    bool dir;
    bool hayBola;

    sh_Enemigo bolas[3];
    int i = 0;
    //Colisiones
    Plataforma lastGround;
    AdministradorPompas* admin;
    int IDBola = 0;

    clock_t temp;
    int direccionX = 1;
    float anchosX = 15.0f;
    bool postEnfado = false;
    bool sueloArriba = false;
    bool sueloDerecha = false;
    bool sueloIzquierda = false;
    bool saltandoDerCorto = false;
    bool saltandoIzqCorto = false;
    bool saltandoRecto = false;
    int contadorPreSalto = 0;
    int contador = 0;


    bool frutaProducida = false;
    bool IAoriginal = false;


    //Muerto -> Ahora esta en Enemigo
    //bool muerto = false;

    Fantasma(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral, float _targetFPS, Rectangle destino, AdministradorPompas& admin, int modoIA) {
        Inicializador(rutaTextura, tamano, saltoMax, velSalto, velLateral);
        if (enfadado) {
            animacionActiva = 3;
            enfadado = false;
            postEnfado = true;
        }
        destRec = posicionPartida;
        posicionDestino = destino;
        tipo = 3;
        widthAnimation = walkAnimation.width / fWalkAnimation;
        heightAnimation = walkAnimation.height;
        targetFrames = _targetFPS;
        enElAire = true;
        cayendo = true;
        disparando = false;
        this->admin = &admin;
        hayBola = false;
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
        postEnfado = true;
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
            distanciaSaltoMax = distanciaSaltoMax * ((float)GetScreenHeight() / (float)lastHeight);
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
        else if (rebotando) {
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

            if (IAoriginal) {
                if (enfadado) {
                    animacionActiva = 3;
                    postEnfado = true;
                    velocidadLateral = 2 * destRec.width / 16.0f;
                    velocidadSalto = destRec.height / 10.0f;
                }
                else {
                    velocidadLateral = destRec.width / 16.0f;
                    velocidadSalto = destRec.height / 10.0f;
                }

                if ((clock() - temp) > 5 * CLOCKS_PER_SEC && hayBola) {
                    velocidadFrames = 2;
                    hayBola = false;
                }

                if (muerto) {
                    animacionActiva = 1;
                    CaerLento();
                }
                else if (!saltando && enElAire) {
                    CaerLento();
                }
                else if (!saltandoRecto && (saltandoDerCorto || destRec.y <= playerPosition.y && destRec.y + destRec.height >= playerPosition.y && !sueloDerecha && destRec.x < playerPosition.x - anchosX)) {
                    Salto(playerPosition, 2);
                }
                else if (!saltandoRecto && (saltandoIzqCorto || destRec.y <= playerPosition.y && destRec.y + destRec.height >= playerPosition.y && !sueloIzquierda && destRec.x > playerPosition.x + anchosX)) {
                    Salto(playerPosition, -2);
                }
                else if (saltando || contador > 180 && sueloArriba && playerPosition.y + destRec.height < destRec.y) {
                    Salto(playerPosition);
                }
                else if ( (destRec.y != playerPosition.y) && (direccionX == 0) ) {
                        //Izquierda
                        std::cout << destRec.y << std::endl;
                        std::cout << playerPosition.y << std::endl;
                        MoverIzq();
                        contador++;
                }
                else if ( (destRec.y != playerPosition.y) && ( direccionX == 1) ) {
                        //Derecha
                        std::cout << "cabron" << std::endl;
                        MoverDer();
                        contador++;
                    
                }
                else if (destRec.x > (playerPosition.x + anchosX) * 10 && !disparando) { //Si el personaje esta a la izquierda      
                    std::cout << "gil" << std::endl;
                    MoverIzq();
                    contador = 0;
                }
                else if (destRec.x < (playerPosition.x - anchosX) * 10 && !disparando) { //Si el personaje esta a la derecha
                    std::cout << "gil" << std::endl;
                    MoverDer();
                    contador = 0;
                }
                else if (destRec.x > playerPosition.x + anchosX && !hayBola) { //Si el personaje esta suficientemente cerca a la izquierda lanza bola
                    BolaIzq();
                    contador = 0;
                }
                else if (destRec.x < playerPosition.x - anchosX && !hayBola) { //Si el personaje esta suficientemente cerca a la izquierda lanza bola
                    BolaDer();
                    contador = 0;
                }
            }
            else {
                if (enfadado) {
                    animacionActiva = 3;
                    postEnfado = true;
                    velocidadLateral = 2 * destRec.width / 16.0f;
                    velocidadSalto = destRec.height / 10.0f;
                }
                else {
                    velocidadLateral = destRec.width / 16.0f;
                    velocidadSalto = destRec.height / 10.0f;
                }

                if ((clock() - temp) > 5 * CLOCKS_PER_SEC) {
                    velocidadFrames = 2;
                    hayBola = false;
                }

                if (muerto) {
                    animacionActiva = 1;
                    CaerLento();
                }
                else if (!saltando && enElAire) {
                    CaerLento();
                }
                else if (!saltandoRecto && (saltandoDerCorto || destRec.y <= playerPosition.y && destRec.y + destRec.height >= playerPosition.y && !sueloDerecha && destRec.x < playerPosition.x - anchosX)) {
                    Salto(playerPosition, 2);
                }
                else if (!saltandoRecto && (saltandoIzqCorto || destRec.y <= playerPosition.y && destRec.y + destRec.height >= playerPosition.y && !sueloIzquierda && destRec.x > playerPosition.x + anchosX)) {
                    Salto(playerPosition, -2);
                }
                else if (saltando || (sueloArriba && destRec.y > playerPosition.y && destRec.x > playerPosition.x - anchosX*5 && destRec.x < playerPosition.x + anchosX*5)) { //Si el personaje esta encima
                    Salto(playerPosition);
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
                else if (destRec.x > playerPosition.x + anchosX * 10 && !disparando) { //Si el personaje esta a la izquierda      
                    MoverIzq();
                }
                else if (destRec.x < playerPosition.x - anchosX * 10 && !disparando) { //Si el personaje esta a la derecha
                    MoverDer();
                }
                else if (destRec.x > playerPosition.x + anchosX && !hayBola) { //Si el personaje esta suficientemente cerca a la izquierda lanza bola
                    BolaIzq();
                }
                else if (destRec.x < playerPosition.x - anchosX && !hayBola) { //Si el personaje esta suficientemente cerca a la izquierda lanza bola
                    BolaDer();
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
                indiceAnimacion = (indiceAnimacion + 1) % fAnimation[2];
                widthAnimation = animations[2].width / fAnimation[2];
                heightAnimation = animations[2].height;
                if (indiceAnimacion == 3) {
                    disparando = false;
                    animacionActiva = 0;
                    Bola b;
                    b =Bola("resources/enemyBola/bolaBasic.png", 2.0f, 40.0f, 1.0f, 1.0f, targetFrames, destRec, dir, IDBola, lastWidth, lastHeight, origin); 
                    b.lastHeight = lastHeight;
                    b.lastWidth = lastWidth;
                    admin->enemigos.push_back(std::make_shared<Bola>(b));
                    i = (i + 1) % 3;
                    IDBola++;
                    indexBolas = (indexBolas + 1) % 3; 
                    temp = clock();
                    hayBola = true;
                }
                break;
            case 3:
                //Actualizar width&height animacion
                indiceAnimacion = (indiceAnimacion + 1) % fWalkAnimation;
                widthAnimation = walkAnimation.width / fWalkAnimation;
                heightAnimation = walkAnimation.height;
                break;
            case 4:
                indiceAnimacion = (indiceAnimacion + 1) % fAnimation[2];
                widthAnimation = animations[2].width / fAnimation[2];
                heightAnimation = animations[2].height;
                if (indiceAnimacion == 3) {
                    disparando = false;
                    animacionActiva = 3;
                    Bola b;
                    b = Bola("resources/enemyBola/bolaBasic.png", 2.0f, 40.0f, 1.0f, 1.0f, targetFrames, destRec, dir, IDBola, lastWidth, lastHeight, origin);
                    b.lastHeight = lastHeight;
                    b.lastWidth = lastWidth;
                    admin->enemigos.push_back(std::make_shared<Bola>(b));
                    i = (i + 1) % 3;
                    IDBola++;
                    indexBolas = (indexBolas + 1) % 3;
                    temp = clock();
                    hayBola = true;
                }
                break;
            default:
                break;
            }
        }
        sueloArriba = false;
        sueloDerecha = false;
        sueloIzquierda = false;
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
        destRec.y += velocidadSalto / 2;
    }

    void BolaIzq() {
        velocidadFrames = 2 * velocidadFrames;
        dir = false;
        disparando = true;
        if(!postEnfado){ animacionActiva = 2; }
        else { animacionActiva = 4; }
    }

    void BolaDer() {
        velocidadFrames = 2 * velocidadFrames;
        dir = true;
        disparando = true;
        if (!postEnfado) { animacionActiva = 2; }
        else { animacionActiva = 4; }
    }

    void Salto(Rectangle player, int tipo = 0) {
        //Gestion de salto
        if (!saltando) {
            saltando = true;
            finSaltando = false;
            enElAire = true;
            cayendo = false;
        }
        else if (contadorPreSalto < 30 && tipo == 0) {
            contadorPreSalto++;
            srcRec.width = pixels;
            saltandoRecto = true;
        }
        else if (contadorPreSalto < 60 && tipo == 0) {
            contadorPreSalto++;
            srcRec.width = -pixels;
            saltandoRecto = true;
        }
        //Subiendo
        else if ((saltoRecorrido <= distanciaSaltoMax) && !finSaltando) {
            if (tipo == -2) {
                destRec.x -= velocidadSalto * 2;
                srcRec.width = pixels;
                direccionX = 0;
                saltandoIzqCorto = true;
                destRec.y -= velocidadSalto;
                saltoRecorrido += velocidadSalto * 3;
            }
            else if (tipo == 2) {
                destRec.x += velocidadSalto * 2;
                srcRec.width = -pixels;
                direccionX = 1;
                saltandoDerCorto = true;
                destRec.y -= velocidadSalto;
                saltoRecorrido += velocidadSalto * 3;
            }
            else {
                destRec.y -= velocidadSalto;
                saltoRecorrido += velocidadSalto;
                saltandoRecto = true;
            }
        }
        //Hemos llegado al maximo
        else if (saltoRecorrido >= distanciaSaltoMax) {
            finSaltando = true;
            destRec.y += velocidadSalto;
            saltoRecorrido -= velocidadSalto;
        }
        //Bajar
        else if (saltoRecorrido > 0 && finSaltando && enElAire) {
            if (tipo == -2) {
                destRec.x -= velocidadSalto * 2;
                srcRec.width = pixels;
                direccionX = 0;
                saltandoIzqCorto = true;
                destRec.y += velocidadSalto;
                saltoRecorrido -= velocidadSalto * 3;
            }
            else if (tipo == 2) {
                destRec.x += velocidadSalto * 2;
                srcRec.width = -pixels;
                direccionX = 1;
                saltandoDerCorto = true;
                destRec.y += velocidadSalto;
                saltoRecorrido -= velocidadSalto * 3;
            }
            else {
                destRec.y += velocidadSalto;
                saltoRecorrido -= velocidadSalto * 2;
            }
        }
        else if (saltoRecorrido < 5) {
            saltando = false;
            finSaltando = true;
            saltandoDerCorto = false;
            saltandoIzqCorto = false;
            saltandoRecto = false;

            contadorPreSalto = 0;

            if (player.x > destRec.x) {
                direccionX = 1;
            }
            else {
                direccionX = 0;
            }
        }
        else {
            saltando = false;
            finSaltando = true;
            contadorPreSalto = 0;
            if (player.x > destRec.x) {
                direccionX = 1;
            }
            else {
                direccionX = 0;
            }
        }
    }

    //Comprobacion de colisiones
    void compruebaColision(Plataforma& s, int enemyNum) override {
        if ((cambioMapa == 0) && (!rebotando)) {
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

            destRec.y = destRec.y - distanciaSaltoMax + destRec.height;
            //Comporbaciones adicionales
            if ((((s.bot) > (destRec.y)) && ((destRec.y) > (s.top)) &&
                ((s.right - 8) > (destRec.x)) && ((destRec.x) > (s.left + 8)))) {
                sueloArriba = true;
            }
            destRec.y = destRec.y + distanciaSaltoMax - destRec.height;

            destRec.y = destRec.y + destRec.height * 3 / 4;
            destRec.x = destRec.x - destRec.width * 3 / 4;
            //Comporbaciones adicionales
            if ((((s.bot) > (destRec.y)) && ((destRec.y) > (s.top)) &&
                ((s.right) > (destRec.x)) && ((destRec.x) > (s.left)))) {
                sueloIzquierda = true;
            }
            destRec.y = destRec.y - destRec.height * 3 / 4;
            destRec.x = destRec.x + destRec.width * 3 / 4;

            destRec.y = destRec.y + destRec.height * 3 / 4;
            destRec.x = destRec.x + destRec.width * 3 / 4;
            //Comporbaciones adicionales
            if ((((s.bot) > (destRec.y)) && ((destRec.y) > (s.top)) &&
                ((s.right) > (destRec.x)) && ((destRec.x) > (s.left)))) {
                sueloDerecha = true;
            }
            destRec.y = destRec.y - destRec.height * 3 / 4;
            destRec.x = destRec.x - destRec.width * 3 / 4;

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
                animacionActiva = 1;
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

typedef std::shared_ptr<Fantasma> sh_Fantasma;