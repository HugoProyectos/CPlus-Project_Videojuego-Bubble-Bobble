#pragma once
#include "Enemigo.cpp"
#include "Bola.cpp"
#include "AdministradorPompas.cpp"
#include <ctime>

class Fantasma : public Enemigo {
public:
    //Gestión de transición de nivel
    int8_t cambioMapa = 2; //2->Primera Iteración 1->Desplazándose 0->Ya no
    Rectangle posicionPartida = { (float)GetScreenWidth() / 2, (float)50, 32, 32 };
    int cuentaFramesTraslacion = 0; //3 segundos = 3 * 60 frames = 180 frames
    const int LIMITE_FRAMES_TRASLACION = 180; //3 segundos = 3 * 60 frames = 180 frames
    double razonX = 0;
    double razonY = 0;
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

    //Muerto -> Ahora esta en Enemigo
    //bool muerto = false;

    Fantasma(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral, float _targetFPS, Rectangle destino, AdministradorPompas& admin) {
        Inicializador(rutaTextura, tamano, saltoMax, velSalto, velLateral);
        if (enfadado) {
            animacionActiva = 3;
            enfadado = false;
            postEnfado = true;
        }
        destRec = destino;
        tipo = 3;
        widthAnimation = walkAnimation.width / fWalkAnimation;
        heightAnimation = walkAnimation.height;
        targetFrames = _targetFPS;
        enElAire = true;
        cayendo = true;
        disparando = false;
        this->admin = &admin;
        hayBola = false;

    };

    void enfadar() {
        animacionActiva = 3;
        postEnfado = true;
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
                anchosX = anchosX * ((float)GetScreenWidth() / (float)lastWidth);
                origin.x = destRec.width / 2;
                lastWidth = GetScreenWidth();
            }

            if (postEnfado) {
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
                Caer();
            }
            else if (enfadado) {
                enfadar();
                enfadado = false;
            }
            else if (!saltando && enElAire) {
                CaerLento();
            }
            else if (saltando || (destRec.y > playerPosition.y && destRec.x > playerPosition.x - anchosX && destRec.x < playerPosition.x + anchosX) && !disparando) { //Si el personaje esta encima
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
            else if (destRec.x > playerPosition.x + anchosX*10 && !disparando) { //Si el personaje esta a la izquierda      
                MoverIzq();
            }
            else if (destRec.x < playerPosition.x - anchosX*10 && !disparando) { //Si el personaje esta a la derecha
                MoverDer();
            }
            else if (destRec.x > playerPosition.x + anchosX && !hayBola) { //Si el personaje esta suficientemente cerca a la izquierda lanza bola
                BolaIzq();
            }
            else if (destRec.x < playerPosition.x - anchosX && !hayBola) { //Si el personaje esta suficientemente cerca a la izquierda lanza bola
                BolaDer();
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
                if (indiceAnimacion == 1) {
                    if (!muertePorAgua) {
                        Frutas f = Frutas();
                        f = Frutas("resources/frutas/cereza.png", 1.0f, 2.0f, (unsigned int)500, 60, destRec, admin->scores);
                        admin->frutas.push_back(std::make_shared<Frutas>(f));
                    }
                    borrame = true;
                }
                break;
            case 2:
                indiceAnimacion = (indiceAnimacion + 1) % fAnimation[2];
                widthAnimation = animations[2].width / fAnimation[2];
                heightAnimation = animations[2].height;
                if (indiceAnimacion == 3) {
                    disparando = false;
                    animacionActiva = 0;
                    Bola b;
                    b =Bola("resources/enemyBola/bolaBasic.png", 2.0f, 40.0f, 1.0f, 1.0f, targetFrames, destRec, dir, IDBola);
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
                    b = Bola("resources/enemyBola/bolaBasic.png", 2.0f, 40.0f, 1.0f, 1.0f, targetFrames, destRec, dir, IDBola);
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

    void Salto(Rectangle player) {
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
        else if (saltoRecorrido < 5) {
            //std::cout << " Salto acabado" << std::endl;
            saltando = false;
            finSaltando = true;
            if (player.x > destRec.x) {
                direccionX = 1;
            }
            else {
                direccionX = 0;
            }
        }
        else {
            //std::cout << " Salto perdido" << std::endl;
            saltando = false;
            finSaltando = true;
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

typedef std::shared_ptr<Fantasma> sh_Fantasma;