#pragma once
#include "Enemigo.cpp"

class Robot : public Enemigo {
public:
    
    //Sprite pixels
    int pixels = 16;

    //Animation
    Texture2D walkAnimation = LoadTexture("resources/enemyRobot/robotWalk.png");
    Texture2D deadAnimation = LoadTexture("resources/enemyRobot/robotMuerte.png");
    Texture2D animations[2] = { walkAnimation, deadAnimation };

    int fWalkAnimation = 4; //VECTOR(cada animacion tiene un numero de frames)       Número de fotogramas de la animacion 
    int fDeadAnimation = 2;
    int fAnimation[2] = { fWalkAnimation , fDeadAnimation };
    
    int widthAnimation; //Se actualiza para cada animación activa
    int heightAnimation;

    int animacionActiva = 0; //Indica la animación activa: 0->WalkAnimation, 1->DeadAnimation
    int indiceAnimacion = 0; //Indica el número de frame actual de la animación activa

    //Frames
    int targetFrames;
    int cuentaFrames = 0;
    int velocidadFrames = 2;

    Robot(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral, float _targetFPS) {
        Inicializador(rutaTextura, tamano, saltoMax, velSalto, velLateral);
        widthAnimation = walkAnimation.width / fWalkAnimation;
        heightAnimation = walkAnimation.height;
        targetFrames = _targetFPS;
    };

    // Controlador de comportamiento
    void Actualizar(Rectangle playerPosition) {
        if (enElAire || (destRec.y > playerPosition.y && destRec.x == playerPosition.x)) { //Si el personaje esta encima
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

    void Dibujar() {
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
    //Herencia de void compruebaColisionSuelo(const Suelo& s)
};