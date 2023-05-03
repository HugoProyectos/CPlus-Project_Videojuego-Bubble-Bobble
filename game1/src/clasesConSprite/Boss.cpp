#include "Boss.hpp"

Boss::Boss(float tamano, float distanciaSaltoMax, float velocidadSalto, float velocidadLateral, float _targetFPS, Rectangle destino, AdministradorPompas& admin) {

    Inicializador("resources/boss/walkUp.png",  tamano, distanciaSaltoMax,  velocidadSalto,  velocidadLateral);
    frameWidth = 128;
    frameHeight = 128;
    srcRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
    origin = { (float)frameWidth * tamano / 2, (float)frameHeight * tamano / 2 }; //En principio no lo necesitamos
    pixels = 128;
    this->_targetFPS = _targetFPS;
    this->targetFrames = _targetFPS;
    enfadado = false;
    this->admin = &admin;
    tipo = 7;

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
    hit = false;
    animacionActiva = 0;
    borrame = false;
    // vector de fuegos tamano 5

}

void Boss::Actualizar(Rectangle playerPosition1, Rectangle playerPosition2) {
    // Gestion logica -----------------------------------------
    if (vida <= 0) {
        sinVida = true;
    }

    if (hit) {
        animacionActiva = 2;
        hit = false;
        contadorParpadeo = 0;
    }
    else if (((clock() - tempAngry) > disparoCoolDown * CLOCKS_PER_SEC) && sinVida) {
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
    
    //Obtener el rectangulo del jugador a seguir
    Rectangle playerPosition;
    if (playerPosition2.x == -1 && playerPosition2.y == -1) {
        playerPosition = playerPosition1;
    }
    else if (playerPosition1.x == -1 && playerPosition1.y == -1) {
        playerPosition = playerPosition2;
    }
    else if (destRec.y <= playerPosition1.y && destRec.y + destRec.height >= playerPosition1.y) {
        playerPosition = playerPosition1;
    }
    else if (destRec.y <= playerPosition2.y && destRec.y + destRec.height >= playerPosition2.y) {
        playerPosition = playerPosition2;
    }
    else {
        float dist1 = sqrt(pow(playerPosition1.x - destRec.x, 2) + pow(playerPosition1.y - destRec.y, 2));
        float dist2 = sqrt(pow(playerPosition2.x - destRec.x, 2) + pow(playerPosition2.y - destRec.y, 2));
        if (dist1 <= dist2) {
            playerPosition = playerPosition1;
        }
        else {
            playerPosition = playerPosition2;
        }
    }

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
            indiceAnimacion = (indiceAnimacion + 1);
            if (indiceAnimacion >= fDead) {
                indiceAnimacion = 0;
                if (contadorParpadeo >= ITERACIONES_PARPADEO) {
                    animacionActiva = 0;
                }
                else {
                    contadorParpadeo++;
                }
            }
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
    if (lastHeight != GetScreenHeight()) {
        destRec.height = GetScreenHeight() / 14.0625f * 4;
        destRec.y = GetScreenHeight() * (destRec.y / lastHeight);
        distanciaSaltoMax = distanciaSaltoMax * ((float)GetScreenHeight() / (float)lastHeight);
        origin.y = destRec.height / 2;
        lastHeight = GetScreenHeight();
    }
    if (lastWidth != GetScreenWidth()) {
        destRec.width = GetScreenWidth() / 25.0f * 4;
        destRec.x = GetScreenWidth() * (destRec.x / lastWidth);
        origin.x = destRec.width / 2;
        lastWidth = GetScreenWidth();
    }
    ratioX = destRec.width / 128;
    ratioY = destRec.height / 128;
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
        if (enfadado) {
            velocidadLateral = 2.0f;
            velocidadSalto = 2.0f;
            animacionActiva = 3;
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
    destRec.x -= velocidadLateral * ratioX;
    destRec.y -= velocidadSalto * ratioY;
    srcRec.width = pixels;
}

void Boss::MoverIzqAbajo() {
    destRec.x -= velocidadLateral * ratioX;
    destRec.y += velocidadSalto * ratioY;
    srcRec.width = pixels;
}

void Boss::MoverDerArriba() {
    destRec.x += velocidadLateral * ratioX;
    destRec.y -= velocidadSalto * ratioY;
    srcRec.width = -pixels;
}

void Boss::MoverDerAbajo() {
    destRec.x += velocidadLateral * ratioX;
    destRec.y += velocidadSalto * ratioY;
    srcRec.width = -pixels;
}

void Boss::enfadar() {
    animacionActiva = 3;
}

void Boss::disparar() {
    disparando = true;
    Rectangle auxDest = { destRec.x, destRec.y, GetScreenWidth() / 25.0f, GetScreenHeight() / 14.0625f};
    Botellas botella1 = Botellas(2.0f, 0, 4.0f, _targetFPS, auxDest, 1, direccionX);
    botella1.lastWidth = GetScreenWidth();
    botella1.lastHeight = GetScreenHeight();
    Botellas botella2 = Botellas(2.0f, 0.5f, 3.5f, _targetFPS, auxDest, 2, direccionX);
    botella2.lastWidth = GetScreenWidth();
    botella2.lastHeight = GetScreenHeight();
    Botellas botella3 = Botellas(2.0f, 1.0f, 3.0f, _targetFPS, auxDest, 3, direccionX);
    botella3.lastWidth = GetScreenWidth();
    botella3.lastHeight = GetScreenHeight();
    Botellas botella4 = Botellas(2.0f, 2.0f, 2.0f, _targetFPS, auxDest, 4, direccionX);
    botella4.lastWidth = GetScreenWidth();
    botella4.lastHeight = GetScreenHeight();
    Botellas botella5 = Botellas(2.0f, 3.0f, 0.0f, _targetFPS, auxDest, 5, direccionX);
    botella5.lastWidth = GetScreenWidth();
    botella5.lastHeight = GetScreenHeight();
    admin->enemigos.push_back(std::make_shared<Botellas>(botella1));
    admin->enemigos.push_back(std::make_shared<Botellas>(botella2));
    admin->enemigos.push_back(std::make_shared<Botellas>(botella3));
    admin->enemigos.push_back(std::make_shared<Botellas>(botella4));
    admin->enemigos.push_back(std::make_shared<Botellas>(botella5));


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

void Boss::compruebaColision(Plataforma& s, int enemyNum) {
    if (cambioMapa == 0) {


        if (   (s.bot > (destRec.y - destRec.height/2)  ) && ( (destRec.y + destRec.height/2) > s.top) && s.top < GetScreenHeight() * 0.10  ) {
            direccionY = 0;
            //destRec.y = s.bot;
        }
        else if ( (s.top  <  (destRec.y + destRec.height/2)) && ((destRec.y + destRec.height/2 ) < s.bot) && (s.top > 50)) {
            direccionY = 1;
            //destRec.y = s.top + 128;
        }
    }
}
void Boss::compruebaSuelo() {
    if (cambioMapa == 0) {
        if (
            !(
                //Comprobamos colision esquina inferior derecha
                (((lastGround.bot) > (destRec.y + destRec.height)) &&
                    ((destRec.y + destRec.height + 1) > (lastGround.top))
                    ) && (
                        ((lastGround.right) > (destRec.x + destRec.width)) &&
                        ((destRec.x + destRec.width) > (lastGround.left))
                        )
                ) &&
            !(
                //Comprobamos colision esquina inferior izquierda
                (((lastGround.bot) > (destRec.y + destRec.height)) &&
                    ((destRec.y + destRec.height + 1) > (lastGround.top))
                    ) && (
                        ((lastGround.right) > (destRec.x - destRec.width)) &&
                        ((destRec.x - destRec.width ) > (lastGround.left))
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
}
void Boss::compruebaPared(const Columnas& s) {
    if (cambioMapa == 0) {
        //Comprobamos columna derecha
        if (s.left_der < (destRec.x + destRec.width/2)) {
            destRec.x = s.left_der - destRec.width/2;
            direccionX = 0;
        }
        //Comprobamos columna izquierda
        else if (s.right_izq > (destRec.x - destRec.width/2)) {
            destRec.x = s.right_izq + destRec.width/2;
            direccionX = 1;
        }
    }
}