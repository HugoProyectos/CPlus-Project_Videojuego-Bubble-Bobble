#pragma once
#include "Enemigo.cpp"

class Morado : public Enemigo {
public:
    
    //Sprite pixels
    int pixels = 16; //El numero de pixeles del sprite

    //Animation
    Texture2D walkAnimation = LoadTexture("resources/enemyRobot/robotWalk.png");
    Texture2D deadAnimation = LoadTexture("resources/enemyRobot/robotMuerte.png");
    Texture2D animations[2] = { walkAnimation, deadAnimation };

    int fWalkAnimation = 4; //Número de fotogramas de la animacion camniar
    int fDeadAnimation = 2; //Número de fotogramas de la animacion muerte
    int fAnimation[2] = { fWalkAnimation , fDeadAnimation };
    
    int widthAnimation; // Se actualiza para cada animación activa
    int heightAnimation;

    int animacionActiva = 0; //Indica la animación activa: 0->WalkAnimation, 1->DeadAnimation
    int indiceAnimacion = 0; //Indica el número de frame actual de la animación activa

    //Frames
    int targetFrames;
    int cuentaFrames = 0;
    int velocidadFrames = 2;

    Morado(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral, float _targetFPS) {
        Inicializador(rutaTextura, tamano, saltoMax, velSalto, velLateral);
        widthAnimation = walkAnimation.width / fWalkAnimation;
        heightAnimation = walkAnimation.height;
        targetFrames = _targetFPS;
    };

    // Controlador de comportamiento
    void Actualizar(Rectangle playerPosition) {
        
        MoverDerAbajo();

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
    void MoverIzqArriba() {
        destRec.x -= velocidadLateral;
        destRec.y -= velocidadLateral;
        srcRec.width = pixels;
    }
    void MoverIzqAbajo() {
        destRec.x -= velocidadLateral;
        destRec.y += velocidadLateral;
        srcRec.width = pixels;
    }

    void MoverDerArriba() {
        destRec.x += velocidadLateral;
        destRec.y -= velocidadLateral;
        srcRec.width = -pixels;
    }
    void MoverDerAbajo() {
        destRec.x += velocidadLateral;
        destRec.y += velocidadLateral;
        srcRec.width = -pixels;
    }


    //Comporbacion de colisiones
    //Herencia de void compruebaColisionSuelo(const Suelo& s)
    void compruebaColision(const Suelo& s) {
        //Choca abajo
        if ((destRec.y + destRec.height / 2) < (s.destRec.y + s.destRec.height / 2) && ((destRec.y + destRec.height / 2) > (s.destRec.y - s.destRec.height / 2))) {
            destRec.y = (s.destRec.y - s.destRec.height / 2) - destRec.height / 2;
        }

        //Choca arriba

        
        //Choca derecha

        //Choca izquierda


    }
};