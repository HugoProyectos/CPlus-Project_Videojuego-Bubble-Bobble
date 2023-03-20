#pragma once
#include "Enemigo.cpp"

class Robot : public Enemigo {
public:
    
    //Animation
    Texture2D walkAnimation = LoadTexture("resources/enemyRobot/robotWalk.png");
    int fWalkAnimation = 4; //Número de fotogramas de la animacion
    int widthAnimation;
    int heightAnimation;

    int animacionActiva = 0; //0->WalkAnimationBasic
    int indiceAnimacion = 0;

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
                indiceAnimacion = (indiceAnimacion + 1) % fWalkAnimation;
                break;
            default:
                break;
            }
        }
    }

    void Dibujar() {
        srcRec.x = (float)widthAnimation * (float)indiceAnimacion;
        DrawTexturePro(walkAnimation, srcRec, destRec, origin, 0.0f, WHITE);
    }

    //funciones de comportamiento
    void MoverIzq() {
        destRec.x -= velocidadLateral;
    }

    void MoverDer() {
        destRec.x += velocidadLateral;
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