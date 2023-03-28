#pragma once
#include "Enemigo.cpp"


class Robot : public Enemigo {
public:
    
    //Sprite pixels
    int pixels = 16; //El numero de pixeles del sprite

    //Animation
    Texture2D walkAnimation = LoadTexture("resources/enemyRobot/robotWalk.png");
    Texture2D deadAnimation = LoadTexture("resources/enemyRobot/robotMuerte.png");
    Texture2D animations[2] = { walkAnimation, deadAnimation };

    int fWalkAnimation = 4; //Número de fotogramas de la animacion camniar
    int fDeadAnimation = 4; //Número de fotogramas de la animacion muerte
    int fAnimation[2] = { fWalkAnimation , fDeadAnimation };
    
    int widthAnimation; // Se actualiza para cada animación activa
    int heightAnimation;

    int animacionActiva = 0; //Indica la animación activa: 0->WalkAnimation, 1->DeadAnimation
    int indiceAnimacion = 0; //Indica el número de frame actual de la animación activa

    //Frames
    int targetFrames;
    int cuentaFrames = 0;
    int velocidadFrames = 2;

    //Colisiones
    Plataforma lastGround;

    //Muerto -> Ahora esta en Enemigo
    //bool muerto = false;

    Robot(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral, float _targetFPS, Rectangle destino) {
        Inicializador(rutaTextura, tamano, saltoMax, velSalto, velLateral);
        destRec = destino;
        tipo = 1;
        widthAnimation = walkAnimation.width / fWalkAnimation;
        heightAnimation = walkAnimation.height;
        targetFrames = _targetFPS;
    };

    // Controlador de comportamiento
    void Actualizar(Rectangle playerPosition) override {
        if (muerto) {
            animacionActiva = 1;
            Caer();
        }
        else if (enElAire || (destRec.y > playerPosition.y && destRec.x == playerPosition.x)) { //Si el personaje esta encima
            Salto();
        }
        else if (destRec.x > playerPosition.x) { //Si el personaje esta a la izquierda
            MoverIzq();
        }
        else if (destRec.x < playerPosition.x) { //Si el personaje esta a la derecha
            MoverDer();
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

    void Salto() {
        //Gestión de salto
        if (!enElAire) {
            //std::cout << "Salto" << std::endl;
            enElAire = true;
        }
        else if (saltoRecorrido < distanciaSaltoMax && enElAire && !cayendo) {
            destRec.y -= velocidadSalto;
            saltoRecorrido += velocidadSalto;
        }
        else if (saltoRecorrido < 0) {
            //Caida lenta
            destRec.y += velocidadSalto/2;
            saltoRecorrido -= velocidadSalto/2;
        }
        else if (enElAire && cayendo && saltoRecorrido > 0) {
            destRec.y += velocidadSalto;
            saltoRecorrido -= velocidadSalto;
        }
        else if (enElAire) {
            saltoRecorrido = 0.0f;
            cayendo = true;
            destRec.y += velocidadSalto / 2; //planeo
        }
    }


    //Comporbacion de colisiones
    void compruebaSuelo(Plataforma s) {
        if (
            !(
                //Comprobamos colision esquina inferior derecha
                (( (s.bot) > (destRec.y + destRec.height / 2)) &&
                    ((destRec.y + destRec.height / 2 + 1) > (s.top))
                    ) && (
                        ((s.right) > (destRec.x + destRec.width / 2)) &&
                        ((destRec.x + destRec.width / 2) > (s.left))
                        )
                ) &&
            !(
                //Comprobamos colision esquina inferior izquierda
                (((s.bot) > (destRec.y + destRec.height / 2)) &&
                    ((destRec.y + destRec.height / 2 + 1) > (s.top))
                    ) && (
                        ((s.right) > (destRec.x - destRec.width / 2)) &&
                        ((destRec.x - destRec.width / 2) > (s.left))
                        )
                )
            ) {
            // No colisiona con plataforma
            enElAire = true;
            cayendo = true;
        }
        else if(muerto){
            enElAire = false;
            cayendo = false;
            borrame = true;
        }
        else {
            enElAire = false;
            cayendo = false;
        }
    }
};

typedef std::shared_ptr<Robot> sh_Robot;