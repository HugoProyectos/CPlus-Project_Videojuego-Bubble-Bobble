#pragma once
#include "Enemigo.cpp"
#include "Bola.cpp"
#include "AdministradorPompas.cpp"
#include <ctime>

class Fantasma : public Enemigo {
public:
    //Sprite pixels
    int pixels = 16; //El numero de pixeles del sprite

    //Animation
    Texture2D walkAnimation = LoadTexture("resources/enemyFantasma/fantasmaWalk.png");
    Texture2D deadAnimation = LoadTexture("resources/enemyFantasma/fantasmaMuerte.png");
    Texture2D ballAnimation = LoadTexture("resources/enemyFantasma/fantasmaBola.png");
    Texture2D animations[4] = { walkAnimation, deadAnimation, ballAnimation };

    int fWalkAnimation = 2; //Número de fotogramas de la animacion camniar
    int fDeadAnimation = 2; //Número de fotogramas de la animacion muerte
    int fBallAnimation = 4;
    int fAnimation[4] = { fWalkAnimation , fDeadAnimation, fBallAnimation };

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
    //Colisiones
    Plataforma lastGround;
    AdministradorPompas* admin;

    clock_t temp;


    //Muerto -> Ahora esta en Enemigo
    //bool muerto = false;

    Fantasma(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral, float _targetFPS, Rectangle destino, AdministradorPompas& admin) {
        Inicializador(rutaTextura, tamano, saltoMax, velSalto, velLateral);
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

    // Controlador de comportamiento
    void Actualizar(Rectangle playerPosition) override {
        
        if ((clock() - temp) > 5 * CLOCKS_PER_SEC) {
            hayBola = false;
        }
        
        if (muerto) {
            animacionActiva = 1;
            //Caer();
        }
        else if (!saltando && enElAire) {
            CaerLento();
        }
        else if (saltando || (destRec.y > playerPosition.y && destRec.x > playerPosition.x - 10 && destRec.x < playerPosition.x + 10) && !disparando) { //Si el personaje esta encima
            Salto();
        }
        else if (destRec.x > playerPosition.x + 100  && !disparando) { //Si el personaje esta a la izquierda      
            MoverIzq();
        }
        else if (destRec.x < playerPosition.x - 100  && !disparando) { //Si el personaje esta a la derecha
            MoverDer();
        }
        else if (destRec.x > playerPosition.x + 5 && !hayBola) { //Si el personaje esta suficientemente cerca a la izquierda lanza bola
            BolaIzq();
        }
        else if (destRec.x < playerPosition.x - 5 && !hayBola ) { //Si el personaje esta suficientemente cerca a la izquierda lanza bola
            BolaDer();
        }

        //Actualizar puntero de animacion
        cuentaFrames++;
        if (cuentaFrames >= (targetFrames / velocidadFrames)) {
            cuentaFrames = 0;
            if (muerto) {
                animacionActiva = 1;
                //Caer();
            }
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
                    sh_Enemigo bola = std::make_shared<Bola>(Bola("resources/enemyFantasma/bolaBasic.png", 2.0f, 40.0f, 1.0f, 1.0f, targetFrames, destRec, dir));
                    admin->enemigos.push_back(bola);
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
    }

    void MoverDer() {
        destRec.x += velocidadLateral;
        srcRec.width = -pixels;
    }

    void Caer() {
        destRec.y += velocidadSalto;
    }

    void CaerLento() {
        destRec.y += velocidadSalto / 2;
    }

    void BolaIzq() {
        dir = false;
        disparando = true;
        animacionActiva = 2;
    }

    void BolaDer() {
        dir = true;
        disparando = true;
        animacionActiva = 2;
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
            switch (s.aproach[enemyNum+ 2]) {
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
                s.aproach[enemyNum+ 2] = 1;
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
                s.aproach[enemyNum+ 2] = 2;
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
                s.aproach[enemyNum+ 2] = 3;
            }
            //Abajo
            else {
                //Si no se cumplen anteriores asumimos que se acerca por debajo
                s.aproach[enemyNum+ 2] = 4;
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
            animacionActiva = 1;
        }
        else {
            enElAire = false;
            cayendo = false;
        }
    }

    void compruebaPared(const Columnas& s) override {
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

typedef std::shared_ptr<Fantasma> sh_Fantasma;