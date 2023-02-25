#pragma once
#include "Enemigo.cpp"

class Robot : public Enemigo {
public:
    float saltoRecorrido = 0;
    float distanciaSaltoMax = 0;
    float velocidadSalto = 0;
    int orden = 0;

    // Sigue como un canelo al jugador
    void Comportamiento(Rectangle playerPosition) {
        if (srcRec.x < playerPosition.x) {
            orden = 2;
        }
        else if (srcRec.x > playerPosition.x) {
            orden = 3;
        }

        if (srcRec.y < playerPosition.y) {
            orden = 1;
        }
    }

    void Actualizar() {
        //Gestión de salto
        if (orden == 1 && !enElAire) {
            std::cout << "Salto" << std::endl;
            enElAire = true;
            destRec.y -= velocidadSalto;
            saltoRecorrido += velocidadSalto;
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

        //Gestión de desplazamiento lateral
        if (orden == 2) {
            destRec.x -= velocidadLateral;
        }
        else if (orden == 3) {
            destRec.x += velocidadLateral;
        }
    };

    void compruebaColision(const Suelo& s) {
        if ((destRec.y + destRec.height / 2) > (s.destRec.y - s.destRec.height / 2)) { //Choca abajo
            destRec.y = (s.destRec.y - s.destRec.height / 2) - destRec.height / 2;
            enElAire = false;
            cayendo = false;
            saltoRecorrido = 0;
        }
        // Comprobar colision con bubuja -> golpeado = true
        // Comprobar colision con jugador aqui?
    }
};