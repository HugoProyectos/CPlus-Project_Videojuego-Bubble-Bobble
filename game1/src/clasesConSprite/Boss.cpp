#include "Boss.hpp"

Boss::Boss(float tamano, float saltoMax, float velSalto, float velLateral, float _targetFPS, Rectangle destino) {

    this->velLateral = velLateral;
    this->velSalto = velSalto;
    this->saltoMax = saltoMax;

    frameWidth = 64;
    frameHeight = 64;

    srcRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
    this->tamano = tamano;

    destRec = { 
        GetScreenWidth() / 2.0f - 50, GetScreenHeight() / 2.0f - 20, (float)frameWidth * tamano, 
        (float)frameHeight * tamano 
    }; 
    
    origin = { (float)frameWidth * tamano / 2, (float)frameHeight * tamano / 2 }; 

    if (enfadado) {
        animacionActiva = 3;
    }
    destRec = destino;

    widthAnimation = walkUp.width / fWalk;
    heightAnimation = walkUp.height;
    targetFrames = _targetFPS;
    enElAire = true;
    cayendo = true;
    disparando = false;
    sinVida = false;
    muerto = false;
    vida = 100;
    // vector de fuegos tamano 5

}

void Boss::Actualizar(Rectangle playerPosition) {

    // Gestion logica -----------------------------------------
    if (vida <= 0) {
        sinVida = true;
    }

    if (hit) {
        animacionActiva = 2;
    }
    else if (((clock() - tempAngry) > disparoCoolDown * CLOCKS_PER_SEC)) {
        enfadado = true;
        animacionActiva = 3;
    }
    else if (direccionY == 1 && !enfadado) {
        animacionActiva = 0;
    }
    else if (direccionY == 0 && !enfadado) {
        animacionActiva = 1;
    }
    else if (sinVida && !muerto) {
        animacionActiva = 4;
        tempAngry = clock();
    }
    else if (muerto) {
        // Asciende arriba a la izquierda
        animacionActiva = 2;
    }

    if ((clock() - tempDisparo) > disparoCoolDown * CLOCKS_PER_SEC) {
        disparar();
    }

    // --------------------------------------------------------
    // IA -----------------------------------------------------
    
    Ia(playerPosition);

    // --------------------------------------------------------
    // Animacion ----------------------------------------------
    cuentaFrames++;
    if (cuentaFrames >= (targetFrames / velocidadFrames)) {
        cuentaFrames = 0;
        switch (animacionActiva) {
        case 0:
            indiceAnimacion = (indiceAnimacion + 1) % fWalk;
            widthAnimation = walkUp.width / fWalk;
            heightAnimation = walkUp.height;
            break;
        case 1:
            indiceAnimacion = (indiceAnimacion + 1) % fWalk;
            widthAnimation = walkDown.width / fWalk;
            heightAnimation = walkDown.height;
            break;
        case 2:
            indiceAnimacion = (indiceAnimacion + 1) % fDead;
            widthAnimation = dead.width / fDead;
            heightAnimation = dead.height;
            break;
        case 3:
            indiceAnimacion = (indiceAnimacion + 1) % fAngry;
            widthAnimation = angry.width / fAngry;
            heightAnimation = angry.height;
            break;
        case 4:
            indiceAnimacion = (indiceAnimacion + 1) % fTrapped;
            widthAnimation = trapped.width / fTrapped;
            heightAnimation = trapped.height;
        default:
            break;
        }
    }
    // --------------------------------------------------------

}

void Boss::Dibujar() {
    srcRec.x = (float)widthAnimation * (float)indiceAnimacion;
    DrawTexturePro(animations[animacionActiva], srcRec, destRec, origin, 0.0f, WHITE);
}

void Boss::Ia(Rectangle playerPosition) {
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
            saltoMax = saltoMax * ((float)GetScreenHeight() / (float)lastHeight);
            origin.y = destRec.height / 2;
            lastHeight = GetScreenHeight();
        }
        if (lastWidth != GetScreenWidth()) {
            destRec.width = GetScreenWidth() / 25.0f;
            destRec.x = GetScreenWidth() * (destRec.x / lastWidth);
            origin.x = destRec.width / 2;
            lastWidth = GetScreenWidth();
        }

        if (enfadado) {
            velLateral = 1.5 * destRec.width / 16.0f;
            velSalto = destRec.height / 10.0f;
        }
        else {
            velLateral = destRec.width / 16.0f;
            velSalto = destRec.height / 10.0f;
        }

        if (direccionX == 0) {
            if (direccionY == 0) {
                MoverIzqAbajo();
            }
            else {
                MoverIzqArriba();
            }
        }

        else if (direccionX == 1) {
            if (direccionY == 0) {
                MoverDerAbajo();
            }
            else {
                MoverDerArriba();
            }
        }


        if (destRec.y > GetScreenHeight() + 50) {
            destRec.y = -10;
            enElAire = true;
            cayendo = true;
        }
        else if (destRec.y < -50) {
            destRec.y = GetScreenHeight() + 5;
        }
    }
}

void Boss::MoverIzqArriba() {
    destRec.x -= velLateral;
    destRec.y -= velSalto;
    srcRec.width = pixels;
}

void Boss::MoverIzqAbajo() {
    destRec.x -= velLateral;
    destRec.y += velSalto;
    srcRec.width = pixels;
}

void Boss::MoverDerArriba() {
    destRec.x += velLateral;
    destRec.y -= velSalto;
    srcRec.width = -pixels;
}

void Boss::MoverDerAbajo() {
    destRec.x += velLateral;
    destRec.y += velSalto;
    srcRec.width = -pixels;
}

void Boss::enfadar() {
    animacionActiva = 3;
}

void Boss::disparar() {
    disparando = true;
    // Disparar 4 bolas
    tempDisparo = clock();
}

void Boss::administrarBotellas() {
    // for 0 to 5 fuegos
    //      actualizar
    //      borrar

    // igual que con las bolas en admin pompas
}

void Boss::eliminarBotellas() {
    // for 0 to 5 fuegos
    // if borrar == true
    //      fuegos = fuegos sin i

    // igual que con las bolas en admin pompas
}