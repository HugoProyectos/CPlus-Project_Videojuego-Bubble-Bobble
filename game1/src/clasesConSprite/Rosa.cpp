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
    int velocidadFrames = 2;

    //Colisiones
    Plataforma lastGround;

    //Variables lógicas
    int direccionX = 0; //0->izquierda, 1->derecha
    int direccionY = 0; //0->abajo, 1->arriba
    int colision = 0; //0->No hay colision, 1->colisiona con paredes(debe variar el movimiento vertical), 2->colision techo(varia movimeitno horizontal)
    int colisionAux = 0;

    float velMax = 0;
    float velMin = 0;
    //Muerto -> Ahora esta en Enemigo
    //bool muerto = false;

    Rosa(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral, float _targetFPS, Rectangle destino) {
        Inicializador(rutaTextura, tamano, saltoMax, velSalto, velLateral);
        velMax = 2 * velLateral;
        velMin = velLateral/2;
        destRec = destino;
        tipo = 5;
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
//        MoverHorizontal(playerPosition);

        else if (colision == 0) {
            std::cout << "LLL" << std::endl;
            SeguirJugador(playerPosition);
        }
        else if (colision == 2) {
            std::cout << "HHH" << std::endl;
            MoverHorizontal(playerPosition);
        }
        else if (colision == 1) {
            std::cout << "VVV" << std::endl;
            MoverVertical(playerPosition);
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
        colision = 0;
    }

    void Dibujar() override {
        srcRec.x = (float)widthAnimation * (float)indiceAnimacion;
        DrawTexturePro(animations[animacionActiva], srcRec, destRec, origin, 0.0f, WHITE);
    }

    //funciones de comportamiento
    void SeguirJugador(Rectangle playerPosition) {
        //El movimiento vertical depende del jugador
        if (destRec.y > playerPosition.y - 10) {
            //Estamos por debajo del player, restamos posicion
            direccionY = 0;
            destRec.y -= velocidadLateral;
        }
        else if(destRec.y < playerPosition.y + 10) {
            //Estamos por encima del player, sumamos posicion
            direccionY = 1;
            destRec.y += velocidadLateral;
        }

        //El movimiento horizontal depende del jugador
        if (destRec.x > playerPosition.x - 10) {
            //Estamos por izq del player, restamos posicion
            direccionX = 0;
            destRec.x -= velocidadSalto;
        }
        else if (destRec.x < playerPosition.x + 10) {
            //Estamos por derecha del player, sumamos posicion
            direccionX = 1;
            destRec.x += velocidadLateral;
        }


        //Width
        srcRec.width = pixels;
    }

    void MoverHorizontal(Rectangle playerPosition) {
        //El movimiento vertical depende del jugador
        if (destRec.y > playerPosition.y - 5) {
            //Estamos por debajo del player, restamos posicion
            destRec.y -= velocidadLateral;
        }
        else if (destRec.y < playerPosition.y + 5) {
            //Estamos por encima del player, sumamos posicion
            destRec.y += velocidadLateral;
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
        if (destRec.x > playerPosition.x - 5) {
            //Estamos por izq del player, restamos posicion
            destRec.x -= velocidadSalto;
        }
        else if (destRec.x < playerPosition.x + 5) {
            //Estamos por derecha del player, sumamos posicion
            destRec.x += velocidadLateral;
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
                //Derecha
                destRec.x = s.left - destRec.width / 2;
                direccionX = 0; //Colisiona derecha, ahora se mueve izquierda
                colision = 1;
                //Se puede añadir un movimiento random en eje Y
                break;
            case 2:
                //Izquierda
                destRec.x = s.right + destRec.width / 2;
                direccionX = 1; //Colisiona izquierda, hora se mueve derecha
                colision = 1;


                //Se puede añadir un movimiento random en eje Y
                break;
            case 3:
                //Abajo
                destRec.y = s.top - destRec.height / 2;
                lastGround = s;
                direccionY = 1; //Colisiona abajo, ahora se mueve arriba
                colision = 2;


                //Se puede añadir un movimiento random en eje X
                break;
            case 4:
                //Arriba
                destRec.y = s.bot + destRec.height / 2;
                direccionY = 0; //Colisiona arriba, ahora se mueve abajo
                colision = 2;


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
                    (((s.bot) > (destRec.y + destRec.height / 2)) &&
                        ((destRec.y + destRec.height / 2) > (s.top))
                        ) && (
                            ((s.right) > (destRec.x + destRec.width / 2 + 5)) &&
                            ((destRec.x + destRec.width / 2 + 5) > (s.left))
                            )
                    )
                ) {
                s.aproach[enemyNum + 1] = 1;
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
                    (((s.bot) > (destRec.y + destRec.height / 2)) &&
                        ((destRec.y + destRec.height / 2) > (s.top))
                        ) && (
                            ((s.right) > (destRec.x - destRec.width / 2 - 5)) &&
                            ((destRec.x - destRec.width / 2 - 5) > (s.left))
                            )
                    )
                ) {
                s.aproach[enemyNum + 1] = 2;
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
                s.aproach[enemyNum + 1] = 3;
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
                s.aproach[enemyNum + 1] = 4;
                colision = 2;
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

    void compruebaPared(const Columnas& s) override {
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
};

typedef std::shared_ptr<Rosa> sh_Rosa;