#pragma once
#include "Sprite.hpp"
#include "Suelo.cpp"
#include "Pompa.hpp"
#include "mapa.cpp"
#include "../AdministradorPompas.cpp"
#include "Controls.cpp" //Bub 0-3, Bob 4-7
#include <iostream>
#include<time.h>



//Se ha decidido que tengan sprites distintos para cada animaci�n,
//La estructura difiere del modelo original gen�rico de personaje por ello.
class Bub : public Sprite {
    const int STANDING = 0;
    const int MOVING = 1;
    const int JUMPING = 2;
    const int FALLING = 3;
    const int SHOOTING = 4;
    const int DYING = 5;
    const int TRASLATION = 6;
    const int SHOCK = 7;
    const Texture2D spriteBurbuja = LoadTexture("resources/Players/Bobblun/pompa.png");
    const Texture2D spriteBurbuja2 = LoadTexture("resources/Players/Bobblun/pompa2.png");
    const Sound sonidoDisparar = LoadSound("resources/music/sonido_escupir.mp3");
    const Sound sonidoSaltar = LoadSound("resources/music/sonido_salto.mp3");
    const Sound sonidoFruta = LoadSound("resources/music/sonido_comer_fruta.mp3");

    const int NUM_FILAS = 4; //n�mero de filas en el animation_set

    int fStandingAnimation = 4; //N�mero de fotogramas de la StandingAnimation
    int fMovingAnimation = 4; //N�mero de fotogramas
    int fJumpingAnimation = 4; //N�mero de fotogramas
    int fFallingAnimation = 4; //N�mero de fotogramas
    int fShootingAnimation = 5; //N�mero de fotogramas
    int fDeathAnimation = 30; //N�mero de fotogramas
    int fTranslationAnimation = 10; //N�mero de fotogramas
    int fShockAnimation = 2;

    const float VELOCIDAD_DISPARO = 5.0;
    const float DISTANCIA_DISPARO = 200.0;
    Rectangle inicio = { -1,-1,-1,-1 };
    

    const int anchoOriginal = 800;
    Texture2D sprite2;
    
public:
    //Variable de controles
    Controls controles;
    unsigned int left;
    unsigned int right;
    unsigned int jump;
    unsigned int shoot;

    bool segundaSkin = false;
    bool mute_sound = false;
    //Variable de identidad
    bool eresBub = true;
    bool eresIA = false;

    //VARIABLES DE DESPLAZAMIENTO DE BUB 
    int8_t cambioMapa = 2; //2->Primera Iteraci�n 1->Desplaz�ndose 0->Ya no
    Rectangle posicionOriginalBub = { (float)100, (float)GetScreenHeight() - 50, 32, 32 }; 
    Rectangle posicionOriginalBob = { (float)GetScreenWidth() - 100, (float)GetScreenHeight() - 50, 32, 32 };
    int cuentaFramesTraslacion = 0; //3 segundos = 3 * 60 frames = 180 frames
    const int LIMITE_FRAMES_TRASLACION = 180; //3 segundos = 3 * 60 frames = 180 frames
    double razonX = 0;
    double razonY = 0;
    bool primeraActualizacion = true; //Se pone a false al final de la primera iteración de Actualización
    
    // VARIABLE DE ULTIMA PLATAFORMA SUELO
    Plataforma lastGround;

    // VARIABLES PARA LA GENERACI�N DE POMPAS
    bool disparando = false;
    bool permitirDisparo = false;
    int multiplicadorVelocidadDisparo = 1;
    int multiplicadorDistanciaDisparo = 1;
    int vidaPompa[11] = {60 * 60, 60 * 60, 60 * 60, 60 * 60, 60*120, 60 * 60, 60 * 60, 60 * 60, 60 * 60, 60 * 60, 60 * 60 };
    int nivel = 0;

    // Referencia al administrador de pompas
    AdministradorPompas* admin;

	//Indicador de muerte y vidas
    bool muriendo = false;
    bool muerto = false;
    int numVidas = 2;

    int widthAnimation = 16;
    int heightAnimation = 16;
    int switchOrientacion = 1;
    int orientacionActual = 2;
    int dirCorrer = 0; //0 = parado, 1 = izquierda, 2 = derecha
    int dirAir = 0; //0 = parado, 1 = izquieda, 2 = derecha

    int animacionActiva = 0; //0->StandingAnimation 1->MoveAnimation 2->JumpAnimation 3->FallAnimation 5->ShootingAnimation 6->DyingAninimation
    int targetFrames;
    
    int lastHeight = 450;
    int lastWidth = 800;

    int indiceAnimacion = 0;
    int cuentaFrames = 0;
    int velocidadFrames = 8;

    //Variables de fisicas
    bool enElAire = false;
    bool cayendo = false;
    bool enElAgua = false;
    int waterlessFrames = 0;
    float saltoRecorrido = 0;
    float distanciaSaltoMax = 0;
    float velocidadSalto = 0;  //A�adir aceleracion, y hacer que velocidad nueva = velocidad anterior + aceleracion. Empezar con una aceleracion inicial, y que se le vayan restando valores. A partir de cierta velocidad, capar. Hay que comprobar que el tope coincida con el planeo.
    float velocidadLateral = 0;
    float deceleracion = 0.1f;
    float velocidadActual = 0;

    //Invulneravilidad
    int framesInvulnerabilidad = 0;
    bool invulnerable = false;
    bool dibuja = true;

    //Electrocucion
    bool electrocutado = false;
    int thunderLessFrames = 0;
    int cuentaShock = 0;
    const int LIMITE_CUENTA_SHOCK = 3;
    bool imTheThunder = false; //Posee pompas de rayo

    Bub() = default; //Debe llamarse a Inicializador

    // VARIABLE DE LA IA
    bool teclaDerecha = false;
    bool teclaIzquierda = false;
    bool teclaSalto = false;
    bool teclaDisparo = false;

    bool sueloArriba = false;
    bool sueloDerecha = false;
    bool sueloIzquierda = false;
    bool sueloArribaDer = false;
    bool sueloArribaIzq = false;
    int direccionX = 1; //0 para izquierda, 1 para derecha
    int contadorSaltos = 0;
    int saltoRecto = false;
    int saltoDer = false;
    int saltoIzq = false;
    /////////////////////////////////

    Bub(float tamano, float saltoMax, float velSalto, float velLateral, int _targetFrames, Rectangle destino, AdministradorPompas& adm, bool esBub) {
        Inicializador(tamano, saltoMax, velSalto, velLateral, _targetFrames, adm, esBub);
		destRec = destino;
        inicio = destino;
        lastHeight = GetScreenHeight();
        lastWidth = GetScreenWidth();
    };
    void Inicializador(float tamano, float saltoMax, float velSalto,float velLateral, int _targetFrames, AdministradorPompas& adm, bool esBub)
    {
        //
        admin = &adm;

        if (esBub) {
            sprite = LoadTexture("resources/Players/Bobblun/animation_set.png");
            sprite2 = LoadTexture("resources/Players/Bobblun/animation_set4.png");

        } else {
            this->eresBub = false;
            sprite = LoadTexture("resources/Players/Bobblun/animation_set2.png");
            sprite2 = LoadTexture("resources/Players/Bobblun/animation_set5.png");
        }
        //widthAnimation = standingAnimation.width / fStandingAnimation;
        //heightAnimation = standingAnimation.height;
        targetFrames = _targetFrames;

        std::cout << "Anchura y altura de un fotograma" << std::endl;
        std::cout << widthAnimation << std::endl;
        std::cout << heightAnimation << std::endl;

        //distanciaSaltoMax = saltoMax;//60.0f;
        distanciaSaltoMax = 60;//60.0f;
        velocidadSalto = velSalto;
        velocidadLateral = velLateral;

        // Source rectangle (part of the texture to use for drawing)
        srcRec = { 0.0f, 0.0f, (float)heightAnimation, (float)widthAnimation };
        this->tamano = tamano;

        // Destination rectangle (screen rectangle where drawing part of texture)
        destRec = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f, (float)widthAnimation * tamano, (float)heightAnimation * tamano }; //Dos primeros, ubicacion. Dos ultimos, dimensiones
		inicio = destRec;

        // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
        origin = { (float)widthAnimation * tamano / 2, (float)heightAnimation * tamano / 2 }; //En principio no lo necesitamos
        
    }

    ~Bub() {
        UnloadTexture(sprite);
    };

    void Unload() {
        UnloadTexture(sprite);
    };

    void resetPosicionOriginal() {
        if (eresBub) {
            posicionOriginalBub = { (float)100, (float)400, 32, 32 };
        }
        else {
            posicionOriginalBob = { (float)700, (float)400, 32, 32 };
        }
    }

    //Llamar al inicio de la partida
    /*void resizeMe() {
        if (lastHeight != GetScreenHeight()) {
            destRec.height = GetScreenHeight() / 14.0625f;
            destRec.y = GetScreenHeight() * (destRec.y / lastHeight);
            origin.y = destRec.height / 2;
            if (eresBub) {
                posicionOriginalBub.y = GetScreenHeight() * (posicionOriginalBub.y / lastHeight);
            }
            else {
                posicionOriginalBob.y = GetScreenHeight() * (posicionOriginalBob.y / lastHeight);
            }
            lastHeight = GetScreenHeight();
        }
        if (lastWidth != GetScreenWidth()) {
           
            destRec.width = GetScreenWidth() / 25.0f;
            destRec.x = GetScreenWidth() * (destRec.x / lastWidth);
            origin.x = destRec.width / 2;
            if (eresBub) {
                posicionOriginalBub.x = GetScreenWidth() * (posicionOriginalBub.x / lastWidth);
            }
            else {
                posicionOriginalBob.x = GetScreenWidth() * (posicionOriginalBob.x / lastWidth);
            }
           
            lastWidth = GetScreenWidth();
        }
    }*/

    void Actualizar() {
        if (framesInvulnerabilidad > 0) {
            framesInvulnerabilidad--;
            if (framesInvulnerabilidad % 10 == 0) { //Parpadeo
                dibuja = !dibuja;
            }
        }
        else {
            invulnerable = false;
            dibuja = true;
        }
        if (lastHeight != GetScreenHeight()) {
            if (cambioMapa > 0 && !primeraActualizacion) {
                destRec.height /= 2;
                destRec.width /= 2;
                destRec.x += destRec.width / 2.0f;
                destRec.y += destRec.height / 2.0f;
            }
            destRec.height = GetScreenHeight() / 14.0625f;
            destRec.y = GetScreenHeight() * (destRec.y / lastHeight);
            origin.y = destRec.height / 2;
            distanciaSaltoMax = distanciaSaltoMax * ((float)GetScreenHeight() / (float)lastHeight);

            if (eresBub) {
                posicionOriginalBub.y = GetScreenHeight() * (posicionOriginalBub.y / lastHeight);
            }
            else {
                posicionOriginalBob.y = GetScreenHeight() * (posicionOriginalBob.y / lastHeight);
            }
            if (cambioMapa > 0 && !primeraActualizacion) {
                destRec.height *= 2;
                destRec.width *= 2;
                if (eresBub) {
                    razonY = ((posicionOriginalBub.y - posicionOriginalBub.y*0.05) - destRec.y) / (LIMITE_FRAMES_TRASLACION - cuentaFramesTraslacion);
                }
                else {
                    razonY = ((posicionOriginalBob.y - posicionOriginalBob.y*0.05)- destRec.y) / (LIMITE_FRAMES_TRASLACION - cuentaFramesTraslacion);
                }
            }
            lastHeight = GetScreenHeight();
        }
        if (lastWidth != GetScreenWidth()) {
            if (cambioMapa > 0 && !primeraActualizacion) {
                destRec.height /= 2;
                destRec.width /= 2;
                destRec.x += destRec.width / 2.0f;
                destRec.y += destRec.height / 2.0f;
            }
           
            destRec.width = GetScreenWidth() / 25.0f;
            destRec.x = GetScreenWidth() * (destRec.x / lastWidth);
            origin.x = destRec.width / 2;
            if (eresBub) {
                posicionOriginalBub.x = GetScreenWidth() * (posicionOriginalBub.x / lastWidth);
            }
            else {
                posicionOriginalBob.x = GetScreenWidth() * (posicionOriginalBob.x / lastWidth);
            }
            if (cambioMapa > 0 && !primeraActualizacion) {
                destRec.height *= 2;
                destRec.width *= 2;
                if (eresBub) {
                    razonX = (posicionOriginalBub.x - destRec.x) / (LIMITE_FRAMES_TRASLACION - cuentaFramesTraslacion);
                }
                else {
                    razonX = (posicionOriginalBob.x - destRec.x) / (LIMITE_FRAMES_TRASLACION - cuentaFramesTraslacion);
                }
            }
            
            lastWidth = GetScreenWidth();
        }
        //Frames de "inmunidad" al agua
        velocidadLateral = destRec.width / 16.0f;
        velocidadSalto = destRec.height / 7.5f;
        deceleracion = velocidadSalto / 40.0f;
        if (waterlessFrames > 0) { waterlessFrames--; }
        //Gestion de wrap vertical
        if ( destRec.y > GetScreenHeight() * 1.05f ) {
            destRec.y = GetScreenHeight() * 0.025f;
            enElAire = true;
            cayendo = true;
            enElAgua = false;
            waterlessFrames = 5;
        }
        else if (destRec.y < (-GetScreenHeight() * 0.05f) ) {
            destRec.y = GetScreenHeight() * 1.025f;
        }


        ////////////////////////////////////////////
        // IA de BOB 
        ////////////////////////////////////////////
        if (eresIA && cambioMapa == 0) {
            Rectangle enemy = Rectangle();
            int bestIndex = 0;
            int type = 0;
            if ((admin->enemigos).size()) {
                for (int i = 1; i < (admin->enemigos).size(); i++) {
                    if (admin->enemigos[i]->muerto || admin->enemigos[i]->rebotando || admin->enemigos[i]->borrame) {
                        //No se añaden
                    }
                    else if (destRec.y <= admin->enemigos[i]->posicion.y && destRec.y + destRec.height >= admin->enemigos[i]->posicion.y) {
                        if(abs(admin->enemigos[bestIndex]->posicion.y - destRec.y) > abs(admin->enemigos[i]->posicion.y - destRec.y))
                        bestIndex = i;
                    }
                    else {
                        float dist1 = sqrt(pow(admin->enemigos[i]->posicion.x - destRec.x, 2) + pow(admin->enemigos[bestIndex]->posicion.y - destRec.y, 2));
                        float dist2 = sqrt(pow(admin->enemigos[bestIndex]->posicion.x - destRec.x, 2) + pow(admin->enemigos[bestIndex]->posicion.y - destRec.y, 2));
                        if (dist1 < dist2) {
                            bestIndex = i;
                        }
                    }

                    if (admin->enemigos[bestIndex]->muerto || admin->enemigos[bestIndex]->rebotando || admin->enemigos[bestIndex]->borrame)
                        bestIndex = i;

                }
                if (!(admin->enemigos[bestIndex]->muerto || admin->enemigos[bestIndex]->rebotando || admin->enemigos[bestIndex]->borrame)) 
                    enemy = admin->enemigos[bestIndex]->posicion;
                    type = admin->enemigos[bestIndex]->tipo;
            }

            ActualizarIA(enemy, type);
        }

        if (eresIA) {
            if (cambioMapa > 0) {
                if (cambioMapa == 2) {
                    primeraActualizacion = false;
                    enElAire = true;
                    cayendo = true;
                    velocidadActual = 0;
                    switchOrientacion = 1;
                    //Reiniciamos su orientación
                    if (orientacionActual == 3) {
                        srcRec.width *= -1;
                        orientacionActual = 2;
                        switchOrientacion = 2;
                    }

                    destRec.height *= 2;
                    destRec.width *= 2;
                    srcRec.height *= 2;
                    srcRec.width *= 2;
                    cambioMapa = 1;
                    animacionActiva = TRASLATION;
                    indiceAnimacion = 6;

                    //Calculamos la distancia a recorrer por iteracion
                    if (eresBub) {
                        razonX = (posicionOriginalBub.x - destRec.x) / LIMITE_FRAMES_TRASLACION;
                        razonY = (posicionOriginalBub.y - destRec.y) / LIMITE_FRAMES_TRASLACION;
                    }
                    else {
                        razonX = (posicionOriginalBob.x - destRec.x) / LIMITE_FRAMES_TRASLACION;
                        razonY = (posicionOriginalBob.y - destRec.y) / LIMITE_FRAMES_TRASLACION;
                    }
                }
                destRec.x += razonX;
                destRec.y += razonY;

                cuentaFrames++;
                if (cuentaFrames >= (targetFrames / velocidadFrames)) {
                    //std::cerr << "Actualiza" << std::endl;
                    cuentaFrames = 0;
                    indiceAnimacion++;//Si la animación no chuta en subsecuentes llamamientos, comenta esta linea, compila, ejecuta, descomenta y vuelve a compilar
                    if (indiceAnimacion >= (fTranslationAnimation + 6 - 2)) { //Los dos últimos frames son de salida de la pompa
                        //std::cout << "Puede volver a disparar" << std::endl;
                        indiceAnimacion = 12; //El frame en que inicia la iteración
                    }
                }
                cuentaFramesTraslacion++;
                //std::cout << cuentaFramesTraslacion << "/" << LIMITE_FRAMES_TRASLACION << std::endl;
                if (cuentaFramesTraslacion >= LIMITE_FRAMES_TRASLACION) {
                    cambioMapa = -1;
                    //std::cout << "DEBERIA CANCELAR" << std::endl;
                }
            }
            else if (!muerto) {
                float velocidadLateralActual = 0;
                if (cambioMapa == 0) {
                    if (admin->j2.electrocutalo && !electrocutado && !muriendo && !muerto) {
                        if (thunderLessFrames <= 0 && !imTheThunder) {
                            electrocutado = true;
                            animacionActiva = SHOCK;
                            indiceAnimacion = 6;
                        }
                        admin->j2.electrocutalo = false;
                    }
                    if (thunderLessFrames > 0) {
                        thunderLessFrames--;
                    }
                    if (admin->j2.debeRebotar > 0) {
                        //std::cout << "Me dicen que rebote" << std::endl;
                        if (!electrocutado) {
                            saltoRecorrido = 0;
                            dirCorrer = 0;
                            //Copy-paste del inicio del salto
                            if (!disparando) animacionActiva = JUMPING;
                            enElAire = true;
                            cayendo = false;
                            velocidadActual = velocidadSalto;
                            destRec.y -= velocidadActual;
                            saltoRecorrido += velocidadActual;
                            velocidadActual -= deceleracion;
                            if (!mute_sound) {
                                PlaySound(sonidoSaltar);
                            }
                        }
                        admin->j2.debeRebotar = 0;
                    }

                    if (enElAire) {
                        if (saltoRecorrido > 0) {
                            if (dirCorrer == 1) {  //Salta izquierda
                                if (teclaDerecha) { //if (IsKeyDown(KEY_S)) {
                                    //(OPCIONAL) A�adir que decelere en vez de cambiar repentinamente de velocidad
                                    switchOrientacion = 3;
                                    destRec.x -= velocidadLateral / 3.0f;;
                                    velocidadLateralActual = velocidadLateral / 3.0f;;
                                    dirAir = 2;
                                }
                                else {
                                    if (teclaIzquierda) { //if (IsKeyDown(KEY_A)) {
                                        switchOrientacion = 2;
                                        dirAir = 1;
                                        destRec.x -= velocidadLateral;
                                        velocidadLateralActual = velocidadLateral;
                                    }
                                    else if (dirAir == 1) {
                                        destRec.x -= velocidadLateral;
                                        velocidadLateralActual = velocidadLateral;
                                    }
                                    else {
                                        destRec.x -= velocidadLateral / 3.0f;
                                        velocidadLateralActual = velocidadLateral / 3.0f;
                                    }
                                }
                            }
                            else if (dirCorrer == 2) {  //Salta derecha
                                if (teclaIzquierda) { //if (IsKeyDown(KEY_A)) {
                                    //(OPCIONAL) A�adir que decelere en vez de cambiar repentinamente de velocidad
                                    switchOrientacion = 2;
                                    destRec.x += velocidadLateral / 3.0f;
                                    velocidadLateralActual = velocidadLateral / 3.0f;
                                    dirAir = 1;
                                }
                                else {
                                    if (teclaDerecha) { //if (IsKeyDown(KEY_S)) {
                                        switchOrientacion = 3;
                                        dirAir = 2;
                                        destRec.x += velocidadLateral;
                                        velocidadLateralActual = velocidadLateral;
                                    }
                                    else if (dirAir == 2) {
                                        destRec.x += velocidadLateral;
                                        velocidadLateralActual = velocidadLateral;
                                    }
                                    else {
                                        destRec.x += velocidadLateral / 3.0f;
                                        velocidadLateralActual = velocidadLateral / 3.0f;
                                    }
                                }
                            }
                            else {
                                if (teclaIzquierda) { //if (IsKeyDown(KEY_A)) {
                                    switchOrientacion = 2;
                                    destRec.x -= velocidadLateral / 2.0f;
                                    velocidadLateralActual = velocidadLateral / 2.0f;
                                }
                                else if (teclaDerecha) { //if (IsKeyDown(KEY_S)) {
                                    switchOrientacion = 3;
                                    destRec.x += velocidadLateral / 2.0f;
                                    velocidadLateralActual = velocidadLateral / 2.0f;
                                }
                            }
                        }
                        else {
                            if (teclaIzquierda) { //if (IsKeyDown(KEY_A)) {
                                if (!disparando && !muriendo && !electrocutado) animacionActiva = MOVING;
                                switchOrientacion = 2;
                                destRec.x -= velocidadLateral / 2.0f;
                                velocidadLateralActual = velocidadLateral / 2.0f;
                            }
                            else if (teclaDerecha) {//if (IsKeyDown(KEY_S)) {
                                if (!disparando && !muriendo && !electrocutado) animacionActiva = MOVING;
                                switchOrientacion = 3;
                                destRec.x += velocidadLateral / 2.0f;
                                velocidadLateralActual = velocidadLateral / 2.0f;
                            }
                        }
                    }

                    // Se puede disparar en el aire. Las acciones en el aire no se ven limitadas por el disparo, 
                    // pero las del suelo s�. Para mantener la idea if/else de en el aire o en el suelo, 
                    // al del suelo se le ha a�aido la restricci�n opuesta al de en el aire (!enElAire)
                    if (teclaDisparo && (!disparando || permitirDisparo) && !muriendo && !electrocutado) { //if (IsKeyDown(KEY_F)) {
                        //std::cout << "Dispara" << std::endl;
                        int sentido = 1; //Hacia la derecha
                        if (orientacionActual == 2) { //Si es hacia la izquierda
                            sentido = -1;
                        }
                        disparando = true;
                        permitirDisparo = false;
                        animacionActiva = SHOOTING;
                        indiceAnimacion = 6; //Es el 0 de la segunda parte de animaciones
                        Pompa p;
                        if (eresBub) {
                            p = Pompa(spriteBurbuja, destRec, (double)(VELOCIDAD_DISPARO * multiplicadorVelocidadDisparo * ((double)lastWidth / (double)anchoOriginal) * sentido), (double)(DISTANCIA_DISPARO * multiplicadorDistanciaDisparo * ((double)lastWidth / (double)anchoOriginal)), true, vidaPompa[nivel]);
                        }
                        else {
                            p = Pompa(spriteBurbuja2, destRec, VELOCIDAD_DISPARO * multiplicadorVelocidadDisparo * ((double)lastWidth / (double)anchoOriginal * sentido), (double)(DISTANCIA_DISPARO * multiplicadorDistanciaDisparo * ((double)lastWidth / (double)anchoOriginal)), true, vidaPompa[nivel]);
                        }
                        if (imTheThunder) { // Debería estar solo en el jefe (si se llega a hacer)
                            p.modulo = Pompa::MODULO_RAYO;
                            p.indiceAnimacion = 18;
                            p.tVida = Pompa::INFINITA;
                        }
                        //std::cout << "Dimensiones pompa; " <<p.destRec.x << "," << p.destRec.y << "/" << p.lastHeight << "," << p.lastWidth << std::endl;
                        admin->pompas.push_back(std::make_shared<Pompa>(p));
                        if (!mute_sound) {
                            PlaySound(sonidoDisparar);
                        }

                    }
                    else if (!enElAire && !electrocutado) {
                        if (teclaIzquierda && !muriendo) { //if (IsKeyDown(KEY_A)) {
                            if (!disparando) animacionActiva = MOVING;
                            switchOrientacion = 2;
                            //std::cout << "Muevo Izquierda, orientacion: " << switchOrientacion << std::endl;
                            if (!enElAgua) {
                                destRec.x -= velocidadLateral;
                                velocidadLateralActual = velocidadLateral;
                            }
                            dirCorrer = 1;
                            dirAir = 1;
                            compruebaSuelo(lastGround);
                        }
                        else if (teclaDerecha && !muriendo) { //if (IsKeyDown(KEY_S)) {
                            if (!disparando) animacionActiva = MOVING;
                            switchOrientacion = 3;
                            //std::cout << "Muevo Derecha, orientacion: " << switchOrientacion << std::endl;
                            if (!enElAgua) {
                                destRec.x += velocidadLateral;
                                velocidadLateralActual = velocidadLateral;
                            }
                            dirCorrer = 2;
                            dirAir = 2;
                            compruebaSuelo(lastGround);
                        }
                        else {
                            dirCorrer = 0;
                            if (!(enElAire && saltoRecorrido == 0)) {
                                switchOrientacion = 1;
                            }
                            if (!disparando && !muriendo) animacionActiva = STANDING;
                        }
                    }

                    //Gesti�n de salto
                    if (teclaSalto && !enElAire && !muriendo && !electrocutado) { //if (IsKeyDown(KEY_SPACE)) {

                        //std::cout << "Salto" << std::endl;
                        if (!disparando) animacionActiva = JUMPING;
                        enElAire = true;
                        if (enElAgua) { enElAgua = false; waterlessFrames = 5; }
                        velocidadActual = velocidadSalto;
                        destRec.y -= velocidadActual;
                        saltoRecorrido += velocidadActual;
                        velocidadActual -= deceleracion;
                        if (!mute_sound) {
                            PlaySound(sonidoSaltar);
                        }

                    }
                    else if (velocidadActual > 0 && enElAire && !cayendo) {
                        if (!disparando && !muriendo && !electrocutado) animacionActiva = JUMPING;
                        destRec.y -= velocidadActual;
                        saltoRecorrido += velocidadActual;
                        velocidadActual -= deceleracion;
                    }
                    else if (enElAire && cayendo && saltoRecorrido > 0) {
                        if (!disparando && !muriendo && !electrocutado) animacionActiva = FALLING;
                        destRec.y -= velocidadActual;
                        saltoRecorrido += velocidadActual;
                        velocidadActual -= deceleracion;
                    }
                    else if (enElAire && cayendo) { //Planeando
                        //std::cout << "I'm gliding" << std::endl;
                        if (!disparando && !muriendo && !electrocutado) animacionActiva = FALLING;
                        destRec.y += velocidadSalto / 2.0f;
                        saltoRecorrido -= velocidadSalto / 2.0f;
                    }
                    else if (enElAire) { //Inicio ca�da
                        if (!disparando && !muriendo && !electrocutado) animacionActiva = FALLING;
                        cayendo = true;
                        destRec.y -= velocidadActual;
                        saltoRecorrido += velocidadActual; //planeo
                        velocidadActual -= deceleracion;
                    }


                    //Comprueba choque con los enemigos
                    for (int i = 0; i < admin->enemigos.size(); i++) {
                        if (!admin->enemigos.at(i)->borrame && !admin->enemigos.at(i)->muerto && !admin->enemigos.at(i)->muertePorAgua && !muriendo && !invulnerable
                            && ((destRec.y + destRec.height / 2.0f) >= (admin->enemigos.at(i)->destRec.y + admin->enemigos.at(i)->destRec.height / 2.0f)
                                && (destRec.y - destRec.height / 2.0f) <= (admin->enemigos.at(i)->destRec.y + admin->enemigos.at(i)->destRec.height / 2.0f)
                                || (destRec.y + destRec.height / 2.0f) >= (admin->enemigos.at(i)->destRec.y - admin->enemigos.at(i)->destRec.height / 2.0f)
                                && (destRec.y - destRec.height / 2.0f) <= (admin->enemigos.at(i)->destRec.y - admin->enemigos.at(i)->destRec.height / 2.0f)
                                || (admin->enemigos.at(i)->tipo == 7
                                    && ((destRec.y + destRec.height / 2) <= (admin->enemigos.at(i)->destRec.y + admin->enemigos.at(i)->destRec.height / 2)
                                        && (destRec.y - destRec.height / 2) >= (admin->enemigos.at(i)->destRec.y - admin->enemigos.at(i)->destRec.height / 2))))
                            && ((destRec.x + destRec.width / 2 - destRec.width * 0.125) >= (admin->enemigos.at(i)->destRec.x - admin->enemigos.at(i)->destRec.width / 2.0f)
                                && (destRec.x - destRec.width / 2 + destRec.width * 0.125) <= (admin->enemigos.at(i)->destRec.x - admin->enemigos.at(i)->destRec.width / 2.0f)
                                || (destRec.x + destRec.width / 2 - destRec.width * 0.125) >= (admin->enemigos.at(i)->destRec.x + admin->enemigos.at(i)->destRec.width / 2.0f)
                                && (destRec.x - destRec.width / 2 + destRec.width * 0.125) <= (admin->enemigos.at(i)->destRec.x + admin->enemigos.at(i)->destRec.width / 2.0f)
                                || (admin->enemigos.at(i)->tipo == 7
                                    && ((destRec.x + destRec.width / 2) <= (admin->enemigos.at(i)->destRec.x + (admin->enemigos.at(i)->destRec.width / 2 - admin->enemigos.at(i)->destRec.width * 0.0625))
                                        && (destRec.x - destRec.width / 2) >= (admin->enemigos.at(i)->destRec.x - (admin->enemigos.at(i)->destRec.width / 2 + admin->enemigos.at(i)->destRec.width * 0.0625)))))) { //Colisiona con un enemigo
                            //std::cout << "I DIED" << std::endl;
                            if (admin->enemigos.at(i)->tipo == 7 && admin->enemigos.at(i)->muertoInterno) {
                                admin->enemigos.at(i)->muertoJefe = true;
                                admin->enemigos.at(i)->muerto = true;
                                if (eresBub) {
                                    admin->scores.SumarPuntuacionP1((unsigned int)10000);
                                }
                                else {
                                    admin->scores.SumarPuntuacionP2((unsigned int)10000);
                                }
                            }
                            else {
                                muriendo = true;
                                electrocutado = false;
                                animacionActiva = DYING;
                            }
                        }
                    }

                    // Comprueba que se come la fruta
                    for (int i = 0; i < admin->frutas.size(); i++) {
                        if (!admin->frutas.at(i)->borrame && !admin->frutas.at(i)->muerto_bub && !admin->frutas.at(i)->muerto_bob && !muriendo
                            && ((destRec.y + destRec.height / 2.0f) >= (admin->frutas.at(i)->destRec.y + admin->frutas.at(i)->destRec.height / 2.0f)
                                && (destRec.y - destRec.height / 2.0f) <= (admin->frutas.at(i)->destRec.y + admin->frutas.at(i)->destRec.height / 2.0f)
                                || (destRec.y + destRec.height / 2.0f) >= (admin->frutas.at(i)->destRec.y - admin->frutas.at(i)->destRec.height / 2.0f)
                                && (destRec.y - destRec.height / 2.0f) <= (admin->frutas.at(i)->destRec.y - admin->frutas.at(i)->destRec.height / 2.0f))
                            && ((destRec.x + destRec.width / 2 - 2) >= (admin->frutas.at(i)->destRec.x - admin->frutas.at(i)->destRec.width / 2.0f)
                                && (destRec.x - destRec.width / 2 + 2) <= (admin->frutas.at(i)->destRec.x - admin->frutas.at(i)->destRec.width / 2.0f)
                                || (destRec.x + destRec.width / 2 - 2) >= (admin->frutas.at(i)->destRec.x + admin->frutas.at(i)->destRec.width / 2.0f)
                                && (destRec.x - destRec.width / 2 + 2) <= (admin->frutas.at(i)->destRec.x + admin->frutas.at(i)->destRec.width / 2.0f)))
                        { //Colisiona con fruta
                            if (eresBub) {
                                admin->scores.SumarPuntuacionP1((unsigned int)admin->frutas.at(i)->puntuacion);
                                admin->frutas.at(i)->muerto_bub = true;
                            }
                            else {
                                admin->scores.SumarPuntuacionP2((unsigned int)admin->frutas.at(i)->puntuacion);
                                admin->frutas.at(i)->muerto_bob = true;
                            }
                            if (!mute_sound) {
                                PlaySound(sonidoFruta);
                            }
                        }
                    }

                    //Le dice al administrador los datos que necesita saber
                    admin->j2.velLateral = velocidadLateralActual;
                    admin->j2.jugadorCayendo = cayendo;
                    admin->j2.posicionJugador = destRec;
                    admin->j2.sentidoJugador = orientacionActual;
                    admin->j2.muriendo = muriendo;
                    admin->j2.thunderLessFrames = thunderLessFrames;
                    admin->j2.electrocutado = electrocutado;
                    admin->j2.heIsTheThunder = imTheThunder;
                }

                //Actualizar puntero de animacion
                cuentaFrames++;
                if (cuentaFrames >= (targetFrames / velocidadFrames)) {
                    //std::cerr << "Actualiza" << std::endl;
                    cuentaFrames = 0;
                    switch (animacionActiva) {
                    case 0:
                        indiceAnimacion = (indiceAnimacion + 1) % fStandingAnimation;
                        break;
                    case 1:
                        indiceAnimacion = (indiceAnimacion + 1) % fMovingAnimation;
                        break;
                    case 2:
                        indiceAnimacion = (indiceAnimacion + 1) % fJumpingAnimation;
                        break;
                    case 3:
                        indiceAnimacion = (indiceAnimacion + 1) % fFallingAnimation;
                        break;
                    case 4:
                        indiceAnimacion++;
                        if (imTheThunder && indiceAnimacion == 9) {
                            permitirDisparo = true;
                        }
                        if (indiceAnimacion >= (fShootingAnimation + 6)) {
                            //std::cout << "Puede volver a disparar" << std::endl;
                            disparando = false;
                            animacionActiva = STANDING;
                            indiceAnimacion = 0;
                        }
                        break;
                    case 5:
                        indiceAnimacion++;
                        if (indiceAnimacion >= (fDeathAnimation + 6)) {
                            indiceAnimacion = 6;
                            muerto = true;
                        }
                        break;
                    case 6:
                        indiceAnimacion++;//Animacion de traslacion
                        //std::cout << "Im in" << std::endl;
                        if (indiceAnimacion >= (fTranslationAnimation + 6)) {
                            //std::cout << "Im truly in" << std::endl;
                            //Reestablecemos las dimensiones de la animación
                            destRec.height /= 2;
                            destRec.width /= 2;
                            destRec.x += destRec.width / 2.0f;
                            destRec.y += destRec.height / 2.0f;
                            srcRec.height = heightAnimation;
                            srcRec.width = widthAnimation;
                            //velocidadFrames *= 2;
                            //Preparamos el personaje para que vuelva a funcionar
                            animacionActiva = STANDING;
                            indiceAnimacion = 0;
                            enElAire = true;
                            cayendo = true;
                            disparando = false;
                            cambioMapa = 0;
                            cuentaFramesTraslacion = 0;
                        }
                        break;
                    case 7:
                        indiceAnimacion++;
                        if (indiceAnimacion >= (8)) {
                            if (cuentaShock >= LIMITE_CUENTA_SHOCK) {
                                indiceAnimacion = 0;
                                cuentaShock = 0;
                                animacionActiva = STANDING;
                                electrocutado = false;
                                thunderLessFrames = 60 * 1.5;
                            }
                            else {
                                indiceAnimacion = 6;
                                cuentaShock++;
                                thunderLessFrames = 60 * 1.5;
                            }

                        }
                    default:
                        break;
                    }
                }
            }
            else {
                if (numVidas > 0) {
                    numVidas--;
                    muerto = false;
                    muriendo = false;
                    enElAire = true;
                    cayendo = true;
                    disparando = false;
                    animacionActiva = STANDING;
                    invulnerable = true;
                    electrocutado = false;
                    thunderLessFrames = 0;
                    framesInvulnerabilidad = 60 * 3; //3 segundos
                    if (!eresBub) {
                        destRec.x = posicionOriginalBob.x;
                        destRec.y = posicionOriginalBob.y;
                    }
                    else {
                        destRec.x = posicionOriginalBub.x;
                        destRec.y = posicionOriginalBub.y;
                    }

                    saltoRecto = false;
                }
            }
        }


        ////////////////////////////////////////////
        // FIN IA de BOB 
        ////////////////////////////////////////////
        // 
        else {
            //Gesti�n de desplazamiento lateral
            if (cambioMapa > 0 && !muerto) {
                if (cambioMapa == 2) {
                    primeraActualizacion = false;
                    enElAire = true;
                    cayendo = true;
                    velocidadActual = 0;
                    switchOrientacion = 1;
                    //Reiniciamos su orientación
                    if (orientacionActual == 3) {
                        srcRec.width *= -1;
                        orientacionActual = 2;
                        switchOrientacion = 2;
                    }

                    destRec.height *= 2;
                    destRec.width *= 2;
                    srcRec.height *= 2;
                    srcRec.width *= 2;
                    cambioMapa = 1;
                    animacionActiva = TRASLATION;
                    indiceAnimacion = 6;
                    //velocidadFrames /= 2;

                    //Calculamos la distancia a recorrer por iteracion
                    if (eresBub) {
                        razonX = (posicionOriginalBub.x - destRec.x) / LIMITE_FRAMES_TRASLACION;
                        razonY = (posicionOriginalBub.y - destRec.y) / LIMITE_FRAMES_TRASLACION;
                    }
                    else {
                        razonX = (posicionOriginalBob.x - destRec.x) / LIMITE_FRAMES_TRASLACION;
                        razonY = (posicionOriginalBob.y - destRec.y) / LIMITE_FRAMES_TRASLACION;
                    }
                }
                destRec.x += razonX;
                destRec.y += razonY;

                cuentaFrames++;
                if (cuentaFrames >= (targetFrames / velocidadFrames)) {
                    //std::cerr << "Actualiza" << std::endl;
                    cuentaFrames = 0;
                    indiceAnimacion++;//Si la animación no chuta en subsecuentes llamamientos, comenta esta linea, compila, ejecuta, descomenta y vuelve a compilar
                    if (indiceAnimacion >= (fTranslationAnimation + 6 - 2)) { //Los dos últimos frames son de salida de la pompa
                        //std::cout << "Puede volver a disparar" << std::endl;
                        indiceAnimacion = 12; //El frame en que inicia la iteración
                    }
                }
                cuentaFramesTraslacion++;
                //std::cout << cuentaFramesTraslacion << "/" << LIMITE_FRAMES_TRASLACION << std::endl;
                if (cuentaFramesTraslacion >= LIMITE_FRAMES_TRASLACION) {
                    cambioMapa = -1;
                    //std::cout << "DEBERIA CANCELAR" << std::endl;
                }
                /*
                if (destRec.x <= posicionOriginalBub.x + 5 && destRec.x >= posicionOriginalBub.x - 5 && destRec.y <= posicionOriginalBub.y + 5 && destRec.y >= posicionOriginalBub.y - 5) {
                    cambioMapa = -1;
                }*/
            }
            else if (!muerto) {
                float velocidadLateralActual = 0;
                if (cambioMapa == 0) {
                    if (admin->j1.electrocutalo && !electrocutado && !muriendo && !muerto) {
                        if (thunderLessFrames <= 0 && !imTheThunder) {
                            electrocutado = true;
                            animacionActiva = SHOCK;
                            indiceAnimacion = 6;
                        }
                        admin->j1.electrocutalo = false;
                    }
                    else if (admin->j2.electrocutalo && !electrocutado && !muriendo && !muerto) {
                        if (thunderLessFrames <= 0 && !imTheThunder) {
                            electrocutado = true;
                            animacionActiva = SHOCK;
                            indiceAnimacion = 6;
                        }
                        admin->j2.electrocutalo = false;
                    }
                    if (thunderLessFrames > 0) {
                        thunderLessFrames--;
                    }
                    if (admin->j1.debeRebotar > 0 && eresBub) { //Si rebota sobre una pompa, es como iniciar un nuevo salto
                        if (!electrocutado) {
                            //std::cout << "Me dicen que rebote" << std::endl;
                            saltoRecorrido = 0;

                            dirCorrer = 0;
                            //Copy-paste del inicio del salto
                            if (!disparando) animacionActiva = JUMPING;
                            enElAire = true;
                            cayendo = false;
                            velocidadActual = velocidadSalto;
                            destRec.y -= velocidadActual;
                            saltoRecorrido += velocidadActual;
                            velocidadActual -= deceleracion;
                            if (!mute_sound) {
                                PlaySound(sonidoSaltar);
                            }
                        }
                        admin->j1.debeRebotar = 0;

                    }
                    else if (admin->j2.debeRebotar > 0 && !eresBub) {
                        //std::cout << "Me dicen que rebote" << std::endl;
                        if (!electrocutado) {
                            saltoRecorrido = 0;
                            dirCorrer = 0;
                            //Copy-paste del inicio del salto
                            if (!disparando) animacionActiva = JUMPING;
                            enElAire = true;
                            cayendo = false;
                            velocidadActual = velocidadSalto;
                            destRec.y -= velocidadActual;
                            saltoRecorrido += velocidadActual;
                            velocidadActual -= deceleracion;
                            if (!mute_sound) {
                                PlaySound(sonidoSaltar);
                            }
                        }
                        admin->j2.debeRebotar = 0;
                    }

                    if (enElAire) {
                        if (saltoRecorrido > 0) {
                            if (dirCorrer == 1) {  //Salta izquierda
                                if (IsKeyDown(right)) { //if (IsKeyDown(KEY_S)) {
                                    //(OPCIONAL) A�adir que decelere en vez de cambiar repentinamente de velocidad
                                    switchOrientacion = 3;
                                    destRec.x -= velocidadLateral / 3.0f;;
                                    velocidadLateralActual = velocidadLateral / 3.0f;;
                                    dirAir = 2;
                                }
                                else {
                                    if (IsKeyDown(left)) { //if (IsKeyDown(KEY_A)) {
                                        switchOrientacion = 2;
                                        dirAir = 1;
                                        destRec.x -= velocidadLateral;
                                        velocidadLateralActual = velocidadLateral;
                                    }
                                    else if (dirAir == 1) {
                                        destRec.x -= velocidadLateral;
                                        velocidadLateralActual = velocidadLateral;
                                    }
                                    else {
                                        destRec.x -= velocidadLateral / 3.0f;
                                        velocidadLateralActual = velocidadLateral / 3.0f;
                                    }
                                }
                            }
                            else if (dirCorrer == 2) {  //Salta derecha
                                if (IsKeyDown(left)) { //if (IsKeyDown(KEY_A)) {
                                    //(OPCIONAL) A�adir que decelere en vez de cambiar repentinamente de velocidad
                                    switchOrientacion = 2;
                                    destRec.x += velocidadLateral / 3.0f;
                                    velocidadLateralActual = velocidadLateral / 3.0f;
                                    dirAir = 1;
                                }
                                else {
                                    if (IsKeyDown(right)) { //if (IsKeyDown(KEY_S)) {
                                        switchOrientacion = 3;
                                        dirAir = 2;
                                        destRec.x += velocidadLateral;
                                        velocidadLateralActual = velocidadLateral;
                                    }
                                    else if (dirAir == 2) {
                                        destRec.x += velocidadLateral;
                                        velocidadLateralActual = velocidadLateral;
                                    }
                                    else {
                                        destRec.x += velocidadLateral / 3.0f;
                                        velocidadLateralActual = velocidadLateral / 3.0f;
                                    }
                                }
                            }
                            else {
                                if (IsKeyDown(left)) { //if (IsKeyDown(KEY_A)) {
                                    switchOrientacion = 2;
                                    destRec.x -= velocidadLateral / 2.0f;
                                    velocidadLateralActual = velocidadLateral / 2.0f;
                                }
                                else if (IsKeyDown(right)) { //if (IsKeyDown(KEY_S)) {
                                    switchOrientacion = 3;
                                    destRec.x += velocidadLateral / 2.0f;
                                    velocidadLateralActual = velocidadLateral / 2.0f;
                                }
                            }
                        }
                        else {
                            if (IsKeyDown(left)) { //if (IsKeyDown(KEY_A)) {
                                if (!disparando && !muriendo && !electrocutado) animacionActiva = MOVING;
                                switchOrientacion = 2;
                                destRec.x -= velocidadLateral / 2.0f;
                                velocidadLateralActual = velocidadLateral / 2.0f;
                            }
                            else if (IsKeyDown(right)) {//if (IsKeyDown(KEY_S)) {
                                if (!disparando && !muriendo && !electrocutado) animacionActiva = MOVING;
                                switchOrientacion = 3;
                                destRec.x += velocidadLateral / 2.0f;
                                velocidadLateralActual = velocidadLateral / 2.0f;
                            }
                        }
                    }

                    // Se puede disparar en el aire. Las acciones en el aire no se ven limitadas por el disparo, 
                    // pero las del suelo s�. Para mantener la idea if/else de en el aire o en el suelo, 
                    // al del suelo se le ha a�aido la restricci�n opuesta al de en el aire (!enElAire)
                    if (IsKeyPressed(shoot) && (!disparando || permitirDisparo) && !muriendo && !electrocutado) { //if (IsKeyDown(KEY_F)) {
                        //std::cout << "Dispara" << std::endl;
                        int sentido = 1; //Hacia la derecha
                        if (orientacionActual == 2) { //Si es hacia la izquierda
                            sentido = -1;
                        }
                        disparando = true;
                        permitirDisparo = false;
                        animacionActiva = SHOOTING;
                        indiceAnimacion = 6; //Es el 0 de la segunda parte de animaciones
                        Pompa p;
                        if (eresBub) {
                            p = Pompa(spriteBurbuja, destRec, (double)(VELOCIDAD_DISPARO * multiplicadorVelocidadDisparo * ((double)lastWidth / (double)anchoOriginal) * sentido), (double)(DISTANCIA_DISPARO * multiplicadorDistanciaDisparo * ((double)lastWidth / (double)anchoOriginal)), true, vidaPompa[nivel]);
                            //std::cout << "Dimensiones pompa; " <<p.destRec.x << "," << p.destRec.y << "/" << p.lastHeight << "," << p.lastWidth << std::endl;

                        }
                        else {
                            p = Pompa(spriteBurbuja2, destRec, VELOCIDAD_DISPARO * multiplicadorVelocidadDisparo * ((double)lastWidth / (double)anchoOriginal * sentido), (double)(DISTANCIA_DISPARO * multiplicadorDistanciaDisparo * ((double)lastWidth / (double)anchoOriginal)), true, vidaPompa[nivel]);
                        }
                        if (imTheThunder) { // Debería estar solo en el jefe (si se llega a hacer)
                            p.modulo = Pompa::MODULO_RAYO;
                            p.indiceAnimacion = 18;
                            p.tVida = Pompa::INFINITA;
                        }
                        //std::cout << "Dimensiones pompa; " <<p.destRec.x << "," << p.destRec.y << "/" << p.lastHeight << "," << p.lastWidth << std::endl;
                        admin->pompas.push_back(std::make_shared<Pompa>(p));
                        if (!mute_sound) {
                            PlaySound(sonidoDisparar);
                        }

                    }
                    else if (!enElAire && !electrocutado) {
                        if (IsKeyDown(left) && !muriendo) { //if (IsKeyDown(KEY_A)) {
                            if (!disparando) animacionActiva = MOVING;
                            switchOrientacion = 2;
                            //std::cout << "Muevo Izquierda, orientacion: " << switchOrientacion << std::endl;
                            if (!enElAgua) {
                                destRec.x -= velocidadLateral;
                                velocidadLateralActual = velocidadLateral;
                            }
                            dirCorrer = 1;
                            dirAir = 1;
                            compruebaSuelo(lastGround);
                        }
                        else if (IsKeyDown(right) && !muriendo) { //if (IsKeyDown(KEY_S)) {
                            if (!disparando) animacionActiva = MOVING;
                            switchOrientacion = 3;
                            //std::cout << "Muevo Derecha, orientacion: " << switchOrientacion << std::endl;
                            if (!enElAgua) {
                                destRec.x += velocidadLateral;
                                velocidadLateralActual = velocidadLateral;
                            }
                            dirCorrer = 2;
                            dirAir = 2;
                            compruebaSuelo(lastGround);
                        }
                        else {
                            dirCorrer = 0;
                            if (!(enElAire && saltoRecorrido == 0)) {
                                switchOrientacion = 1;
                            }
                            if (!disparando && !muriendo) animacionActiva = STANDING;
                        }
                    }

                    //Gesti�n de salto
                    if (IsKeyPressed(jump) && !enElAire && !muriendo && !electrocutado) { //if (IsKeyDown(KEY_SPACE)) {

                        //std::cout << "Salto" << std::endl;
                        if (!disparando) animacionActiva = JUMPING;
                        enElAire = true;
                        if (enElAgua) { enElAgua = false; waterlessFrames = 5; }
                        velocidadActual = velocidadSalto;
                        destRec.y -= velocidadActual;
                        saltoRecorrido += velocidadActual;
                        velocidadActual -= deceleracion;
                        if (!mute_sound) {
                            PlaySound(sonidoSaltar);
                        }

                    }
                    else if (velocidadActual > 0 && enElAire && !cayendo) {
                        if (!disparando && !muriendo && !electrocutado) animacionActiva = JUMPING;
                        destRec.y -= velocidadActual;
                        saltoRecorrido += velocidadActual;
                        velocidadActual -= deceleracion;
                    }
                    else if (enElAire && cayendo && saltoRecorrido > 0) {
                        if (!disparando && !muriendo && !electrocutado) animacionActiva = FALLING;
                        destRec.y -= velocidadActual;
                        saltoRecorrido += velocidadActual;
                        velocidadActual -= deceleracion;
                    }
                    else if (enElAire && cayendo) { //Planeando
                        //std::cout << "I'm gliding" << std::endl;
                        if (!disparando && !muriendo && !electrocutado) animacionActiva = FALLING;
                        destRec.y += velocidadSalto / 2.0f;
                        saltoRecorrido -= velocidadSalto / 2.0f;
                    }
                    else if (enElAire) { //Inicio ca�da
                        if (!disparando && !muriendo && !electrocutado) animacionActiva = FALLING;
                        cayendo = true;
                        destRec.y -= velocidadActual;
                        saltoRecorrido += velocidadActual; //planeo
                        velocidadActual -= deceleracion;
                    }


                    //Comprueba choque con los enemigos
                    for (int i = 0; i < admin->enemigos.size(); i++) {
                        if (!admin->enemigos.at(i)->borrame && !admin->enemigos.at(i)->muerto && !admin->enemigos.at(i)->muertePorAgua && !muriendo && !invulnerable
                            && ((destRec.y + destRec.height / 2.0f) >= (admin->enemigos.at(i)->destRec.y + admin->enemigos.at(i)->destRec.height / 2.0f)
                                && (destRec.y - destRec.height / 2.0f) <= (admin->enemigos.at(i)->destRec.y + admin->enemigos.at(i)->destRec.height / 2.0f)
                                || (destRec.y + destRec.height / 2.0f) >= (admin->enemigos.at(i)->destRec.y - admin->enemigos.at(i)->destRec.height / 2.0f)
                                && (destRec.y - destRec.height / 2.0f) <= (admin->enemigos.at(i)->destRec.y - admin->enemigos.at(i)->destRec.height / 2.0f)
                                || (admin->enemigos.at(i)->tipo == 7
                                    && ((destRec.y + destRec.height / 2) <= (admin->enemigos.at(i)->destRec.y + admin->enemigos.at(i)->destRec.height / 2)
                                        && (destRec.y - destRec.height / 2) >= (admin->enemigos.at(i)->destRec.y - admin->enemigos.at(i)->destRec.height / 2))))
                            && ((destRec.x + destRec.width / 2 - destRec.width * 0.125) >= (admin->enemigos.at(i)->destRec.x - admin->enemigos.at(i)->destRec.width / 2.0f)
                                && (destRec.x - destRec.width / 2 + destRec.width * 0.125) <= (admin->enemigos.at(i)->destRec.x - admin->enemigos.at(i)->destRec.width / 2.0f)
                                || (destRec.x + destRec.width / 2 - destRec.width * 0.125) >= (admin->enemigos.at(i)->destRec.x + admin->enemigos.at(i)->destRec.width / 2.0f)
                                && (destRec.x - destRec.width / 2 + destRec.width * 0.125) <= (admin->enemigos.at(i)->destRec.x + admin->enemigos.at(i)->destRec.width / 2.0f)
                                || (admin->enemigos.at(i)->tipo == 7
                                    && ((destRec.x + destRec.width / 2) <= (admin->enemigos.at(i)->destRec.x + (admin->enemigos.at(i)->destRec.width / 2 - admin->enemigos.at(i)->destRec.width * 0.0625))
                                        && (destRec.x - destRec.width / 2) >= (admin->enemigos.at(i)->destRec.x - (admin->enemigos.at(i)->destRec.width / 2 + admin->enemigos.at(i)->destRec.width * 0.0625)))))) { //Colisiona con un enemigo
                            //std::cout << "I DIED" << std::endl;
                            if (admin->enemigos.at(i)->tipo == 7 && admin->enemigos.at(i)->muertoInterno) {
                                admin->enemigos.at(i)->muertoJefe = true;
                                admin->enemigos.at(i)->muerto = true; 
                                if (eresBub) {
                                    admin->scores.SumarPuntuacionP1((unsigned int)10000);
                                }
                                else {
                                    admin->scores.SumarPuntuacionP2((unsigned int)10000);
                                }
                            }
                            else {
                                muriendo = true;
                                electrocutado = false;
                                animacionActiva = DYING;
                            }
                        }
                    }

                    // Comprueba que se come la fruta
                    for (int i = 0; i < admin->frutas.size(); i++) {
                        if (!admin->frutas.at(i)->borrame && !admin->frutas.at(i)->muerto_bub && !admin->frutas.at(i)->muerto_bob && !muriendo
                            && ((destRec.y + destRec.height / 2.0f) >= (admin->frutas.at(i)->destRec.y + admin->frutas.at(i)->destRec.height / 2.0f)
                                && (destRec.y - destRec.height / 2.0f) <= (admin->frutas.at(i)->destRec.y + admin->frutas.at(i)->destRec.height / 2.0f)
                                || (destRec.y + destRec.height / 2.0f) >= (admin->frutas.at(i)->destRec.y - admin->frutas.at(i)->destRec.height / 2.0f)
                                && (destRec.y - destRec.height / 2.0f) <= (admin->frutas.at(i)->destRec.y - admin->frutas.at(i)->destRec.height / 2.0f))
                            && ((destRec.x + destRec.width / 2 - 2) >= (admin->frutas.at(i)->destRec.x - admin->frutas.at(i)->destRec.width / 2.0f)
                                && (destRec.x - destRec.width / 2 + 2) <= (admin->frutas.at(i)->destRec.x - admin->frutas.at(i)->destRec.width / 2.0f)
                                || (destRec.x + destRec.width / 2 - 2) >= (admin->frutas.at(i)->destRec.x + admin->frutas.at(i)->destRec.width / 2.0f)
                                && (destRec.x - destRec.width / 2 + 2) <= (admin->frutas.at(i)->destRec.x + admin->frutas.at(i)->destRec.width / 2.0f)))
                        { //Colisiona con fruta
                            if (eresBub) {
                                admin->scores.SumarPuntuacionP1((unsigned int)admin->frutas.at(i)->puntuacion);
                                admin->frutas.at(i)->muerto_bub = true;
                            }
                            else {
                                admin->scores.SumarPuntuacionP2((unsigned int)admin->frutas.at(i)->puntuacion);
                                admin->frutas.at(i)->muerto_bob = true;
                            }
                            if (!mute_sound) {
                                PlaySound(sonidoFruta);
                            }
                        }
                    }

                    //Le dice al administrador los datos que necesita saber
                    if (eresBub) {
                        admin->j1.velLateral = velocidadLateralActual;
                        admin->j1.jugadorCayendo = cayendo;
                        admin->j1.posicionJugador = destRec;
                        admin->j1.sentidoJugador = orientacionActual;
                        admin->j1.muriendo = muriendo;
                        admin->j1.thunderLessFrames = thunderLessFrames;
                        admin->j1.electrocutado = electrocutado;
                        admin->j1.heIsTheThunder = imTheThunder;
                    }
                    else {
                        admin->j2.velLateral = velocidadLateralActual;
                        admin->j2.jugadorCayendo = cayendo;
                        admin->j2.posicionJugador = destRec;
                        admin->j2.sentidoJugador = orientacionActual;
                        admin->j2.muriendo = muriendo;
                        admin->j2.thunderLessFrames = thunderLessFrames;
                        admin->j2.electrocutado = electrocutado;
                        admin->j2.heIsTheThunder = imTheThunder;
                    }
                }

                //Actualizar puntero de animacion
                cuentaFrames++;
                if (cuentaFrames >= (targetFrames / velocidadFrames)) {
                    //std::cerr << "Actualiza" << std::endl;
                    cuentaFrames = 0;
                    switch (animacionActiva) {
                    case 0:
                        indiceAnimacion = (indiceAnimacion + 1) % fStandingAnimation;
                        break;
                    case 1:
                        indiceAnimacion = (indiceAnimacion + 1) % fMovingAnimation;
                        break;
                    case 2:
                        indiceAnimacion = (indiceAnimacion + 1) % fJumpingAnimation;
                        break;
                    case 3:
                        indiceAnimacion = (indiceAnimacion + 1) % fFallingAnimation;
                        break;
                    case 4:
                        indiceAnimacion++;
                        if (imTheThunder && indiceAnimacion == 9) {
                            permitirDisparo = true;
                        }
                        if (indiceAnimacion >= (fShootingAnimation + 6)) {
                            //std::cout << "Puede volver a disparar" << std::endl;
                            disparando = false;
                            animacionActiva = STANDING;
                            indiceAnimacion = 0;
                        }
                        break;
                    case 5:
                        indiceAnimacion++;
                        if (indiceAnimacion >= (fDeathAnimation + 6)) {
                            indiceAnimacion = 6;
                            muerto = true;
                        }
                        break;
                    case 6:
                        indiceAnimacion++;//Animacion de traslacion
                        //std::cout << "Im in" << std::endl;
                        if (indiceAnimacion >= (fTranslationAnimation + 6)) {
                            //std::cout << "Im truly in" << std::endl;
                            //Reestablecemos las dimensiones de la animación
                            destRec.height /= 2;
                            destRec.width /= 2;
                            destRec.x += destRec.width / 2.0f;
                            destRec.y += destRec.height / 2.0f;
                            srcRec.height = heightAnimation;
                            srcRec.width = widthAnimation;
                            //velocidadFrames *= 2;
                            //Preparamos el personaje para que vuelva a funcionar
                            animacionActiva = STANDING;
                            indiceAnimacion = 0;
                            if (eresBub && orientacionActual == 2) {
                                switchOrientacion = 3;
                                orientacionActual = 2;
                                dirCorrer = 2;
                                dirAir = 2;
                            }
                            enElAire = true;
                            cayendo = true;
                            disparando = false;
                            cambioMapa = 0;
                            cuentaFramesTraslacion = 0;
                        }
                        break;
                    case 7:
                        indiceAnimacion++;
                        if (indiceAnimacion >= (8)) {
                            if (cuentaShock >= LIMITE_CUENTA_SHOCK) {
                                indiceAnimacion = 0;
                                cuentaShock = 0;
                                animacionActiva = STANDING;
                                electrocutado = false;
                                thunderLessFrames = 60 * 1.5;
                            }
                            else {
                                indiceAnimacion = 6;
                                cuentaShock++;
                                thunderLessFrames = 60 * 1.5;
                            }

                        }
                    default:
                        break;
                    }
                }
            }
            else {
                if (numVidas > 0) {
                    numVidas--;
                    muerto = false;
                    muriendo = false;
                    enElAire = true;
                    cayendo = true;
                    disparando = false;
                    animacionActiva = STANDING;
                    invulnerable = true;
                    electrocutado = false;
                    thunderLessFrames = 0;
                    framesInvulnerabilidad = 60 * 3; //3 segundos
                    if (eresBub) {
                        destRec.x = posicionOriginalBub.x;
                        destRec.y = posicionOriginalBub.y;

                    }
                    else {
                        destRec.x = posicionOriginalBob.x;
                        destRec.y = posicionOriginalBob.y;
                    }
                }
            }
        }
    };

    void ActualizarIA(Rectangle enemy, int tipo) 
    {
        teclaDerecha = false;
        teclaIzquierda = false;
        teclaSalto = false;
        teclaDisparo = false;
        if (enemy.x == 0 && enemy.y == 0) {
            //No hay enemigos
        }
        else if (tipo < 0) {
            if (enemy.x < destRec.x) {
                teclaDerecha = true;
                srcRec.width = -abs(srcRec.width);
                orientacionActual = 3;
                direccionX = 1;
            }
            else {
                teclaIzquierda = true;
                srcRec.width = abs(srcRec.width);
                orientacionActual = 2;
                direccionX = 0;
            }

            //comprobar hueco en el suelo
            if (direccionX == 0 && !sueloIzquierda) {
                teclaSalto = true;
            }
            else if (direccionX == 1 && !sueloDerecha) {
                teclaSalto = true;
            }

        }
        //Si el enemigo esta a misma altura
        else if (destRec.y - destRec.height * 3 <= enemy.y && destRec.y + destRec.height * 3 >= enemy.y) {
            //Comprobar rango de disparo
            int distance = abs(enemy.x - destRec.x);
            if (distance <= destRec.width * 7) {
                //Si esta a la derecha
                if (enemy.x > destRec.x && direccionX == 0) {
                    teclaDerecha = true;
                    srcRec.width = -abs(srcRec.width);
                    orientacionActual = 3;
                    direccionX = 1;
                }
                else if (enemy.x < destRec.x && direccionX == 1) {
                    teclaIzquierda = true;
                    srcRec.width = abs(srcRec.width);
                    orientacionActual = 2;
                    direccionX = 0;
                }
                else {
                    if (destRec.y - destRec.height <= enemy.y && destRec.y + destRec.height >= enemy.y) { teclaDisparo = true; }
                }
            }
            //comprobar direccion
            else if (enemy.x > destRec.x) {
                teclaDerecha = true;
                srcRec.width = -abs(srcRec.width);
                orientacionActual = 3;
                direccionX = 1;
            }
            else if (enemy.x < destRec.x) {
                teclaIzquierda = true;
                srcRec.width = abs(srcRec.width);
                orientacionActual = 2;
                direccionX = 0;
            }
            //comprobar hueco en el suelo
            if (direccionX == 0 && !sueloIzquierda) {
                teclaSalto = true;
            }
            else if (direccionX == 1 && !sueloDerecha) {
                teclaSalto = true;
            }
        }
        //Si el enemigo esta abajo
        else if(enemy.y > destRec.y){
            //Moverse en horizontal con direccionX
            //Mover izquierda
            if (direccionX == 0) {
                teclaIzquierda = true;
                //srcRec.width = abs(srcRec.width);
                //orientacionActual = 2;
            }
            else if (direccionX == 1) {
                teclaDerecha = true;
                //srcRec.width = -abs(srcRec.width);
                //orientacionActual = 3;
            }

            //Saltos que "impiden" situacion irresoluble entre IAs
            if (contadorSaltos > 300) {
                teclaSalto = true;
                contadorSaltos = 0;
            }
            else if (!enElAire) {
                contadorSaltos++;
            }
        }
        //Si el enemigo esta muy arriba
        else if (enemy.y < destRec.y - distanciaSaltoMax*3) {
            if (sueloArriba && !enElAire) {

                teclaSalto = true;
                saltoRecto = true;
            }
            else if (direccionX == 0 && !saltoRecto) {

                teclaIzquierda = true;
                //srcRec.width = abs(srcRec.width);
                //orientacionActual = 2;
            }
            else if (direccionX == 1 && !saltoRecto) {

                teclaDerecha = true;
                //srcRec.width = -abs(srcRec.width);
                //orientacionActual = 3;
            }
            //comprobar hueco en el suelo
            if (direccionX == 0 && !sueloIzquierda) {
                teclaSalto = true;
            }
            else if (direccionX == 1 && !sueloDerecha) {
                teclaSalto = true;
            }
        }
        //El enemigo esta arriba
        else if (enemy.y < destRec.y) {
            if (sueloArriba && !enElAire && (5*destRec.width < abs(destRec.x-enemy.x))) {
                teclaSalto = true;
                saltoRecto = true;
            }
            else if (direccionX == 0 && !saltoRecto) {
                teclaIzquierda = true;
            }
            else if (direccionX == 1 && !saltoRecto) {
                teclaDerecha = true;

            }

            //comprobar hueco en el suelo
            if (direccionX == 0 && !sueloIzquierda) {
                teclaSalto = true;
            }
            else if (direccionX == 1 && !sueloDerecha) {
                teclaSalto = true;
            }

        }
        else {
            if (direccionX == 0) {
                teclaIzquierda = true;
                //srcRec.width = abs(srcRec.width);
                //orientacionActual = 2;
            }
            else if (direccionX == 1) {
                teclaDerecha == true;
                //srcRec.width = -abs(srcRec.width);
                //orientacionActual = 3;
            }
        }

        sueloArriba = false;
        sueloDerecha = false;
        sueloIzquierda = false;
        sueloArribaDer = false;
        sueloArribaIzq = false;
        return;
    }

    void Dibujar() {
        //std::cerr << "Indice Animacion: " << indiceAnimacion << std::endl;
        if (srcRec.width > widthAnimation) { //Modo traslacion
            //srcRec.x = (float)widthAnimation * (float)indiceAnimacion; //Cambia el fotograma
            srcRec.x = (float)widthAnimation * 6 + (float)widthAnimation*2 * ((float)indiceAnimacion-6);// + (float)indiceAnimacion - 6 * srcRec.width; //Cambia el fotograma
            //std::cout << "Dibujo sobre traslacion sobre x = " << srcRec.x << " Con indice:" << indiceAnimacion << std::endl;
        }
        else {
            srcRec.x = (float)widthAnimation * (float)indiceAnimacion; //Cambia el fotograma
        }
        if ((switchOrientacion == 2 && orientacionActual == 3) || (switchOrientacion == 3 && orientacionActual == 2)) {
            //std::cout << "Cambio de orientacion" << std::endl;
            srcRec.width *= -1; //Cambia la orientacion
            orientacionActual = switchOrientacion;
        }
        if (animacionActiva == SHOCK) {
            srcRec.y = (float)heightAnimation * (float)(DYING % NUM_FILAS);
        }
        else {
            srcRec.y = (float)heightAnimation * (float)(animacionActiva % NUM_FILAS);
        }
        if (dibuja) {
            if (!segundaSkin) {
                DrawTexturePro(sprite, srcRec, destRec, origin, 0.0f, WHITE);
            }
            else {
                DrawTexturePro(sprite2, srcRec, destRec, origin, 0.0f, WHITE);
            }
        }
    }

    void compruebaColision(Plataforma& s) {
        if (cambioMapa == 0) {
            //Si esta en el agua no comprobamos colision
            if (enElAgua) {
                enElAire = false;
                saltoRecto = false;
                cayendo = true;
                return;
            }
            //Comprobamos si colisiona con la superficie
            if (
                (
                    //Comprobamos colision esquina inferior derecha
                    (((s.bot) > (destRec.y + destRec.height * 0.49)) &&
                        ((destRec.y + destRec.height * 0.49) > (s.top))
                        ) && (
                            ((s.right) > (destRec.x + destRec.width / 2.0f)) &&
                            ((destRec.x + destRec.width / 2.0f) > (s.left))
                            )
                    ) ||
                (
                    //Comprobamos colision centro inferior derecha
                    (((s.bot) > (destRec.y + destRec.height / 4.0f)) &&
                        ((destRec.y + destRec.height / 4.0f) > (s.top))
                        ) && (
                            ((s.right) > (destRec.x + destRec.width / 2.0f)) &&
                            ((destRec.x + destRec.width / 2.0f) > (s.left))
                            )
                    ) ||
                (
                    //Comprobamos colision centro superior derecha
                    (((s.bot) > (destRec.y - destRec.height / 4.0f)) &&
                        ((destRec.y - destRec.height / 4.0f) > (s.top))
                        ) && (
                            ((s.right) > (destRec.x + destRec.width / 2.0f)) &&
                            ((destRec.x + destRec.width / 2.0f) > (s.left))
                            )
                    ) ||
                (
                    //Comprobamos colision esquina superior derecha
                    (((s.bot) > (destRec.y - destRec.height / 2.1f)) &&
                        ((destRec.y - destRec.height / 2.1f) > (s.top))
                        ) && (
                            ((s.right) > (destRec.x + destRec.width / 2.0f)) &&
                            ((destRec.x + destRec.width / 2.0f) > (s.left))
                            )
                    ) ||
                (
                    //Comprobamos colision esquina superior izquierda
                    (((s.bot) > (destRec.y - destRec.height / 2.1f)) &&
                        ((destRec.y - destRec.height / 2.1f) > (s.top))
                        ) && (
                            ((s.right) > (destRec.x - destRec.width / 2.0f)) &&
                            ((destRec.x - destRec.width / 2.0f) > (s.left))
                            )
                    ) ||
                (
                    //Comprobamos colision centro superior izquierda
                    (((s.bot) > (destRec.y - destRec.height / 4.0f)) &&
                        ((destRec.y - destRec.height / 4.0f) > (s.top))
                        ) && (
                            ((s.right) > (destRec.x - destRec.width / 2.0f)) &&
                            ((destRec.x - destRec.width / 2.0f) > (s.left))
                            )
                    ) ||
                (
                    //Comprobamos colision centro inferior izquierda
                    (((s.bot) > (destRec.y + destRec.height / 4.0f)) &&
                        ((destRec.y + destRec.height / 4.0f) > (s.top))
                        ) && (
                            ((s.right) > (destRec.x - destRec.width / 2.0f)) &&
                            ((destRec.x - destRec.width / 2.0f) > (s.left))
                            )
                    ) ||
                (
                    //Comprobamos colision esquina inferior izquierda
                    (((s.bot) > (destRec.y + destRec.height * 0.49)) &&
                        ((destRec.y + destRec.height * 0.49) > (s.top))
                        ) && (
                            ((s.right) > (destRec.x - destRec.width / 2.0f)) &&
                            ((destRec.x - destRec.width / 2.0f) > (s.left))
                            )
                    ) ||
                (
                    //Comprobamos colision centro inferior
                    (((s.bot) > (destRec.y + destRec.height * 0.49)) &&
                        ((destRec.y + destRec.height * 0.49) > (s.top))
                        ) && (
                            ((s.right) > (destRec.x)) &&
                            ((destRec.x) > (s.left))
                            )
                    )
                ) {
                switch (s.aproach[0 + (int)!eresBub]) {
                case 1:
                    destRec.x = s.left - destRec.width / 2.0f;
                    if (!eresBub) {
                        direccionX = 0;
                        srcRec.width = abs(srcRec.width);
                        orientacionActual = 2;
                    }
                    break;
                case 2:
                    destRec.x = s.right + destRec.width / 2.0f;
                    if (!eresBub) {
                        direccionX = 1;
                        srcRec.width = -abs(srcRec.width);
                        orientacionActual = 3;
                    }
                    break;
                case 3:
                    destRec.y = s.top - destRec.height / 2.0f;
                    enElAire = false;
                    saltoRecto = false;
                    cayendo = false;
                    saltoRecorrido = 0;
                    lastGround = s;
                    break;
                }
            }
            //Comprobamos si se esta acercando a la superficie desde alguna direcci�n
            else {
                //Izquierda
                if (
                    //Comprobamos colision esquina superior derecha
                    (
                        (((s.bot) > (destRec.y - destRec.height / 2.1f)) &&
                            ((destRec.y - destRec.height / 2.1f) > (s.top))
                            ) && (
                                ((s.right) > (destRec.x + destRec.width / 2 + destRec.width / 6)) &&
                                ((destRec.x + destRec.width / 2 + destRec.width / 6) > (s.left))
                                )
                        )
                    ||
                    //Comprobamos colision esquina inferior derecha
                    (
                        (((s.bot) > (destRec.y + destRec.height * 0.49)) &&
                            ((destRec.y + destRec.height * 0.49) > (s.top))
                            ) && (
                                ((s.right) > (destRec.x + destRec.width / 2 + destRec.width / 6)) &&
                                ((destRec.x + destRec.width / 2 + destRec.width / 6) > (s.left))
                                )
                        )
                    ) {
                    //std::cout << "Aproach left" << std::endl;
                    s.aproach[0 + (int)!eresBub] = 1;
                }
                //Derecha
                else if (
                    //Comprobamos colision esquina superior derecha
                    (
                        (((s.bot) > (destRec.y - destRec.height / 2.1f)) &&
                            ((destRec.y - destRec.height / 2.1f) > (s.top))
                            ) && (
                                ((s.right) > (destRec.x - destRec.width / 2 - destRec.width / 6)) &&
                                ((destRec.x - destRec.width / 2 - destRec.width / 6) > (s.left))
                                )
                        )
                    ||
                    //Comprobamos colision esquina inferior derecha
                    (
                        (((s.bot) > (destRec.y + destRec.height * 0.49)) &&
                            ((destRec.y + destRec.height * 0.49) > (s.top))
                            ) && (
                                ((s.right) > (destRec.x - destRec.width / 2 - destRec.width / 6)) &&
                                ((destRec.x - destRec.width / 2 - destRec.width / 6) > (s.left))
                                )
                        )
                    ) {
                    //std::cout << "Aproach right" << std::endl;
                    s.aproach[0 + (int)!eresBub] = 2;
                }
                //Arriba
                else if (
                    //Comprobamos colision esquina inferior derecha
                    (
                        (((s.bot) > (destRec.y + destRec.height / 2 + destRec.height / 6)) &&
                            ((destRec.y + destRec.height / 2 + destRec.height / 6) > (s.top))
                            ) && (
                                ((s.right) > (destRec.x + destRec.width / 2.0f)) &&
                                ((destRec.x + destRec.width / 2.0f) > (s.left))
                                )
                        )
                    ||
                    //Comprobamos colision esquina inferior izquierda
                    (
                        (((s.bot) > (destRec.y + destRec.height / 2 + destRec.height / 6)) &&
                            ((destRec.y + destRec.height / 2 + destRec.height / 6) > (s.top))
                            ) && (
                                ((s.right) > (destRec.x - destRec.width / 2.0f)) &&
                                ((destRec.x - destRec.width / 2.0f) > (s.left))
                                )
                        )
                    ||
                    //Comprobamos colision centro inferior
                    (
                        (((s.bot) > (destRec.y + destRec.height / 2 + destRec.height / 6)) &&
                            ((destRec.y + destRec.height / 2 + destRec.height / 6) > (s.top))
                            ) && (
                                ((s.right) > (destRec.x)) &&
                                ((destRec.x) > (s.left))
                                )
                        )
                    )
                    {
                    //std::cout << "Aproach top" << std::endl;
                    s.aproach[0 + (int)!eresBub] = 3;
                }
                //Abajo
                else {
                    //Si no se cumplen anteriores asumimos que se acerca por debajo
                    s.aproach[0 + (int)!eresBub] = 4;
                }
            }


            destRec.y = destRec.y - distanciaSaltoMax;
            if ((((s.bot) > (destRec.y)) && ((destRec.y+60) > (s.top)) &&
                ((s.right) > (destRec.x)) && ((destRec.x) > (s.left)))) {
                sueloArriba = true;

            }
            destRec.y = destRec.y + distanciaSaltoMax;


            destRec.y = destRec.y - distanciaSaltoMax;
            destRec.x = destRec.x + destRec.width * 15 / 4;
            if ((((s.bot) > (destRec.y)) && ((destRec.y + 60) > (s.top)) &&
                ((s.right) > (destRec.x)) && ((destRec.x) > (s.left)))) {
                sueloArribaDer = true;
            }
            destRec.y = destRec.y + distanciaSaltoMax;
            destRec.x = destRec.x - destRec.width * 15 / 4;


            destRec.y = destRec.y - distanciaSaltoMax;
            destRec.x = destRec.x - destRec.width *15 / 4;
            if ((((s.bot) > (destRec.y)) && ((destRec.y + 60) > (s.top)) &&
                ((s.right) > (destRec.x)) && ((destRec.x) > (s.left)))) {
                sueloArribaIzq = true;

            }
            destRec.y = destRec.y + distanciaSaltoMax;
            destRec.x = destRec.x + destRec.width * 15 / 4;



            destRec.y = destRec.y + destRec.height * 3 / 4;
            destRec.x = destRec.x - destRec.width * 3 / 4;
            //Comporbaciones adicionales
            if ((((s.bot) > (destRec.y)) && ((destRec.y) > (s.top)) &&
                ((s.right) > (destRec.x)) && ((destRec.x) > (s.left)))) {
                sueloIzquierda = true;
            }
            destRec.y = destRec.y - destRec.height * 3 / 4;
            destRec.x = destRec.x + destRec.width * 3 / 4;

            destRec.y = destRec.y + destRec.height * 3 / 4;
            destRec.x = destRec.x + destRec.width * 3 / 4;
            //Comporbaciones adicionales
            if ((((s.bot) > (destRec.y)) && ((destRec.y) > (s.top)) &&
                ((s.right) > (destRec.x)) && ((destRec.x) > (s.left)))) {
                sueloDerecha = true;
            }
            destRec.y = destRec.y - destRec.height * 3 / 4;
            destRec.x = destRec.x - destRec.width * 3 / 4;
        }
    }

    void compruebaSuelo(Plataforma s) {
        if (cambioMapa == 0) {
            if (
                !(
                    //Comprobamos colision esquina inferior derecha
                    (((s.bot) > (destRec.y + destRec.height / 2.0f)) &&
                        ((destRec.y + destRec.height / 2 + 1) > (s.top))
                        ) && (
                            ((s.right) > (destRec.x + destRec.width / 2.0f)) &&
                            ((destRec.x + destRec.width / 2.0f) > (s.left))
                            )
                    ) &&
                !(
                    //Comprobamos colision esquina inferior izquierda
                    (((s.bot) > (destRec.y + destRec.height / 2.0f)) &&
                        ((destRec.y + destRec.height / 2 + 1) > (s.top))
                        ) && (
                            ((s.right) > (destRec.x - destRec.width / 2.0f)) &&
                            ((destRec.x - destRec.width / 2.0f) > (s.left))
                            )
                    ) &&
                !(
                    //Comprobamos colision esquina centro inferior
                    (((s.bot) > (destRec.y + destRec.height / 2.0f)) &&
                        ((destRec.y + destRec.height / 2 + 1) > (s.top))
                        ) && (
                            ((s.right) > (destRec.x)) &&
                            ((destRec.x) > (s.left))
                            )
                    )
                ) {
                enElAire = true;
                cayendo = true;
            }
        }
    }

    void compruebaPared(const Columnas& s) {
        if (cambioMapa == 0) {
            
            //Comprobamos columna derecha
            if (s.left_der < (destRec.x + destRec.width / 2.0f)) {
                destRec.x = s.left_der - destRec.width / 2.0f;
                direccionX = 0;
                srcRec.width = abs(srcRec.width);
                orientacionActual = 2;
            }
            //Comprobamos columna izquierda
            else if (s.right_izq > (destRec.x - destRec.width / 2.0f)) {
                destRec.x = s.right_izq + destRec.width / 2.0f;
                direccionX = 1;
                srcRec.width = -abs(srcRec.width);
                orientacionActual = 3;
            }
        }
    }
};

//Controls Bub::controles = NULL;