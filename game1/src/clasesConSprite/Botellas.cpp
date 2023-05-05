# pragma once
#include "Botellas.hpp"

Botellas::Botellas(float tamano, float velSalto, float velLateral, float _targetFPS, Rectangle destino, int ID, int direccion) {
	Botellas::Inicializador("resources/enemyBotellas/walk.png", tamano, 2, velSalto, velLateral);
	this->ID = ID;
	destRec = destino;
    tipo = -3;
    widthAnimation = walk.width / fWalk;
    heightAnimation = walk.height;
    targetFrames = _targetFPS;
    enElAire = true;
    cayendo = false;
    borrame = false;
    direccionX = direccion;
}

void Botellas::Actualizar(Rectangle playerPosition1, Rectangle playerPosition2)  {
    // Gestion logica -----------------------------------------
    if (lastHeight != GetScreenHeight()) {
        destRec.height = GetScreenHeight() / 14.0625f;
        destRec.y = GetScreenHeight() * (destRec.y / lastHeight);
        origin.y = destRec.height / 2;

        lastHeight = GetScreenHeight();
    }
    if (lastWidth != GetScreenWidth()) {
        destRec.width = GetScreenWidth() / 25.0f;
        destRec.x = GetScreenWidth() * (destRec.x / lastWidth);
        origin.x = destRec.width / 2;

        lastWidth = GetScreenWidth();
    }
    ratioX = destRec.width / 32;
    ratioY = destRec.width / 32;
    if (direccionX == 0) {
        MoverIzq();
    }
    else if (direccionX == 1) {
        MoverDer();
    }

    // --------------------------------------------------------
    // Animacion ----------------------------------------------

    cuentaFrames++;
    if (cuentaFrames >= (targetFrames / velocidadFrames)) {
        cuentaFrames = 0;
        switch (animacionActiva) {
        case 0:
            indiceAnimacion = (indiceAnimacion + 1) % fWalk;
            widthAnimation = walk.width / fWalk;
            heightAnimation = walk.height;
            break;
        default:
            break;
        }
    }

    // --------------------------------------------------------
}

void Botellas::Dibujar() {
    srcRec.x = (float)widthAnimation * (float)indiceAnimacion;
    DrawTexturePro(animations[animacionActiva], srcRec, destRec, origin, 0.0f, WHITE);
}

void Botellas::MoverIzq() {
    destRec.x -= velocidadLateral * ratioX;
    destRec.y += velocidadSalto * ratioY;
    srcRec.width = pixels;
}

void Botellas::MoverDer() {
    destRec.x += velocidadLateral * ratioX;
    destRec.y += velocidadSalto * ratioY;
    srcRec.width = -pixels;
}

void Botellas::compruebaColision(Plataforma& s, int enemyNum) {
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
            destRec.x = s.left - destRec.width / 2;
            borrame = true;
            break;
        case 2:
            destRec.x = s.right + destRec.width / 2;
            borrame = true;
            break;
        case 3:
            destRec.y = s.top - destRec.height / 2;
            enElAire = false;
            cayendo = false;
            saltoRecorrido = 0;
            lastGround = s;
            borrame = true;
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
        }
        //Abajo
        else {
            //Si no se cumplen anteriores asumimos que se acerca por debajo
            s.aproach[enemyNum + 1] = 4;
        }
    }
}

void Botellas::compruebaSuelo() {
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
        ) 
    {
        
    }
    else  {
        enElAire = false;
        cayendo = false;
        borrame = true;
    }
}

void Botellas::compruebaPared(const Columnas& s) {
    if (s.left_der < (destRec.x + destRec.width / 2)) {
        velocidadLateral = 0;
        borrame = true;
    }
    else if (s.right_izq > (destRec.x - destRec.width / 2)) {
        velocidadLateral = 0;
        borrame = true;
    }
}
