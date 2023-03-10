#pragma once
#include "Enemigo.cpp"

class Fantasma : public Enemigo {
public:
    
    Fantasma(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral) {
        Inicializador(rutaTextura, tamano, saltoMax, velSalto, velLateral);
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
    }

    //Funciones de comportamiento
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

    //Invocación de bola de fuego
    
    //Comporbacion de colisiones
    //Herencia de void compruebaColisionSuelo(const Suelo& s)
};