#pragma once
#include "Sprite.hpp"
#include "Suelo.cpp"
#include "Pompa.hpp"
#include "mapa.cpp"
#include "../AdministradorPompas.cpp"
#include <iostream>



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
    const Texture2D spriteBurbuja = LoadTexture("resources/Players/Bobblun/Pompa.png");
    const Texture2D spriteBurbuja2 = LoadTexture("resources/Players/Bobblun/Pompa2.png");

    const int NUM_FILAS = 4; //n�mero de filas en el animation_set

    int fStandingAnimation = 4; //N�mero de fotogramas de la StandingAnimation
    int fMovingAnimation = 4; //N�mero de fotogramas
    int fJumpingAnimation = 4; //N�mero de fotogramas
    int fFallingAnimation = 4; //N�mero de fotogramas
    int fShootingAnimation = 5; //N�mero de fotogramas
    int fDeathAnimation = 30; //N�mero de fotogramas
    int fTranslationAnimation = 10; //N�mero de fotogramas

    const float VELOCIDAD_DISPARO = 5.0;
    const float DISTANCIA_DISPARO = 200.0;
    Rectangle inicio = { -1,-1,-1,-1 };

   

public:
    //Variable de identidad
    bool eresBub = true;

    //VARIABLES DE DESPLAZAMIENTO DE BUB 
    int8_t cambioMapa = 2; //2->Primera Iteraci�n 1->Desplaz�ndose 0->Ya no
    Vector2 posicionOriginalBub = { (float)100, (float)GetScreenHeight() - 50 }; 
    Vector2 posicionOriginalBob = { (float)GetScreenWidth() - 100, (float)GetScreenHeight() - 50 };
    int cuentaFramesTraslacion = 0; //3 segundos = 3 * 60 frames = 180 frames
    const int LIMITE_FRAMES_TRASLACION = 180; //3 segundos = 3 * 60 frames = 180 frames
    double razonX = 0;
    double razonY = 0;
    
    // VARIABLE DE ULTIMA PLATAFORMA SUELO
    Plataforma lastGround;

    // VARIABLES PARA LA GENERACI�N DE POMPAS
    bool disparando = false;
    int multiplicadorVelocidadDisparo = 1;
    int multiplicadorDistanciaDisparo = 1;
    int vidaPompa = 1000;

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

    Bub() = default; //Debe llamarse a Inicializador

    Bub(float tamano, float saltoMax, float velSalto, float velLateral, int _targetFrames, Rectangle destino, AdministradorPompas& adm, bool esBub) {
        Inicializador(tamano, saltoMax, velSalto, velLateral, _targetFrames, adm, esBub);
		destRec = destino;
        inicio = destino;
    };
    void Inicializador(float tamano, float saltoMax, float velSalto,float velLateral, int _targetFrames, AdministradorPompas& adm, bool esBub)
    {
        //
        admin = &adm;

        if (esBub) {
            sprite = LoadTexture("resources/Players/Bobblun/animation_set.png");
        } else {
            this->eresBub = false;
            sprite = LoadTexture("resources/Players/Bobblun/animation_set2.png");
        }
        //widthAnimation = standingAnimation.width / fStandingAnimation;
        //heightAnimation = standingAnimation.height;
        targetFrames = _targetFrames;

        std::cout << "Anchura y altura de un fotograma" << std::endl;
        std::cout << widthAnimation << std::endl;
        std::cout << heightAnimation << std::endl;

        distanciaSaltoMax = saltoMax;//60.0f;
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

    void Actualizar() {
        //Frames de "inmunidad" al agua
        if (waterlessFrames > 0) { waterlessFrames--; }
        //Gestion de wrap vertical
        if (destRec.y > 500) {
            destRec.y = -10;
            enElAire = true;
            cayendo = true;
            enElAgua = false;
            waterlessFrames = 3;
        }
        else if (destRec.y < -50) {
            destRec.y = 450;
        }

        //Gesti�n de desplazamiento lateral
        if (cambioMapa > 0) {
            if (cambioMapa == 2) {
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
                } else {
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
                indiceAnimacion++;
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
		else if(!muerto){
            float velocidadLateralActual = 0;
            if (cambioMapa == 0) {
                if (admin->j1.debeRebotar > 0 && eresBub) { //Si rebota sobre una pompa, es como iniciar un nuevo salto
                    //std::cout << "Me dicen que rebote" << std::endl;
                    saltoRecorrido = 0;
                    admin->j1.debeRebotar = 0;
                    dirCorrer = 0;
                    //Copy-paste del inicio del salto
                    if (!disparando) animacionActiva = JUMPING;
                    enElAire = true;
                    cayendo = false;
                    velocidadActual = velocidadSalto;
                    destRec.y -= velocidadActual;
                    saltoRecorrido += velocidadActual;
                    velocidadActual -= deceleracion;
                } else if (admin->j2.debeRebotar > 0 && !eresBub) {
                    //std::cout << "Me dicen que rebote" << std::endl;
                    saltoRecorrido = 0;
                    admin->j2.debeRebotar = 0;
                    dirCorrer = 0;
                    //Copy-paste del inicio del salto
                    if (!disparando) animacionActiva = JUMPING;
                    enElAire = true;
                    cayendo = false;
                    velocidadActual = velocidadSalto;
                    destRec.y -= velocidadActual;
                    saltoRecorrido += velocidadActual;
                    velocidadActual -= deceleracion;
                }

                if (enElAire) {
                    if (saltoRecorrido > 0) {
                        if (dirCorrer == 1) {  //Salta izquierda
                            if (IsKeyDown(KEY_S)) {
                                //(OPCIONAL) A�adir que decelere en vez de cambiar repentinamente de velocidad
                                switchOrientacion = 3;
                                destRec.x -= velocidadLateral / 3;
                                velocidadLateralActual = velocidadLateral / 3;
                                dirAir = 2;
                            }
                            else {
                                if (IsKeyDown(KEY_A)) {
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
                                    destRec.x -= velocidadLateral / 3;
                                    velocidadLateralActual = velocidadLateral / 3;
                                }
                            }
                        }
                        else if (dirCorrer == 2) {  //Salta derecha
                            if (IsKeyDown(KEY_A)) {
                                //(OPCIONAL) A�adir que decelere en vez de cambiar repentinamente de velocidad
                                switchOrientacion = 2;
                                destRec.x += velocidadLateral / 3;
                                velocidadLateralActual = velocidadLateral / 3;
                                dirAir = 1;
                            }
                            else {
                                if (IsKeyDown(KEY_S)) {
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
                                    destRec.x += velocidadLateral / 3;
                                    velocidadLateralActual = velocidadLateral / 3;
                                }
                            }
                        }
                        else {
                            if (IsKeyDown(KEY_A)) {
                                switchOrientacion = 2;
                                destRec.x -= velocidadLateral / 2;
                                velocidadLateralActual = velocidadLateral / 2;
                            }
                            else if (IsKeyDown(KEY_S)) {
                                switchOrientacion = 3;
                                destRec.x += velocidadLateral / 2;
                                velocidadLateralActual = velocidadLateral / 2;
                            }
                        }
                    }
                    else {
                        if (IsKeyDown(KEY_A)) {
                            if (!disparando && !muriendo) animacionActiva = MOVING;
                            switchOrientacion = 2;
                            destRec.x -= velocidadLateral / 2;
                            velocidadLateralActual = velocidadLateral / 2;
                        }
                        else if (IsKeyDown(KEY_S)) {
                            if (!disparando && !muriendo) animacionActiva = MOVING;
                            switchOrientacion = 3;
                            destRec.x += velocidadLateral / 2;
                            velocidadLateralActual = velocidadLateral / 2;
                        }
                    }
                }

                // Se puede disparar en el aire. Las acciones en el aire no se ven limitadas por el disparo, 
                // pero las del suelo s�. Para mantener la idea if/else de en el aire o en el suelo, 
                // al del suelo se le ha a�aido la restricci�n opuesta al de en el aire (!enElAire)
                if (IsKeyPressed(KEY_F) && !disparando && !muriendo) {
                    //std::cout << "Dispara" << std::endl;
                    int sentido = 1; //Hacia la derecha
                    if (orientacionActual == 2) { //Si es hacia la izquierda
                        sentido = -1;
                    }
                    disparando = true;
                    animacionActiva = SHOOTING;
                    indiceAnimacion = 6; //Es el 0 de la segunda parte de animaciones
                    Pompa p;
                    if (eresBub) {
                        p = Pompa(spriteBurbuja, destRec, VELOCIDAD_DISPARO * multiplicadorVelocidadDisparo * sentido, DISTANCIA_DISPARO * multiplicadorDistanciaDisparo, true, vidaPompa);
                    } else {
                        p = Pompa(spriteBurbuja2, destRec, VELOCIDAD_DISPARO * multiplicadorVelocidadDisparo * sentido, DISTANCIA_DISPARO * multiplicadorDistanciaDisparo, true, vidaPompa);
                    }
                    admin->pompas.push_back(p);
                }
                else if (!enElAire) {
                    if (IsKeyDown(KEY_A) && !muriendo) {
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
                    else if (IsKeyDown(KEY_S) && !muriendo) {
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
                if (IsKeyPressed(KEY_SPACE) && !enElAire && !muriendo) {

                    //std::cout << "Salto" << std::endl;
                    if (!disparando) animacionActiva = JUMPING;
                    enElAire = true;
                    if (enElAgua) { enElAgua = false; waterlessFrames = 3; }
                    velocidadActual = velocidadSalto;
                    destRec.y -= velocidadActual;
                    saltoRecorrido += velocidadActual;
                    velocidadActual -= deceleracion;
                }
                else if (velocidadActual > 0 && enElAire && !cayendo) {
                    if (!disparando && !muriendo) animacionActiva = JUMPING;
                    destRec.y -= velocidadActual;
                    saltoRecorrido += velocidadActual;
                    velocidadActual -= deceleracion;
                }
                else if (enElAire && cayendo && saltoRecorrido > 0) {
                    if (!disparando && !muriendo) animacionActiva = FALLING;
                    destRec.y -= velocidadActual;
                    saltoRecorrido += velocidadActual;
                    velocidadActual -= deceleracion;
                }
                else if (enElAire && cayendo) { //Planeando
                    //std::cout << "I'm gliding" << std::endl;
                    if (!disparando && !muriendo) animacionActiva = FALLING;
                    destRec.y += velocidadSalto / 2;
                    saltoRecorrido -= velocidadSalto / 2;
                }
                else if (enElAire) { //Inicio ca�da
                    if (!disparando && !muriendo) animacionActiva = FALLING;
                    cayendo = true;
                    destRec.y -= velocidadActual;
                    saltoRecorrido += velocidadActual; //planeo
                    velocidadActual -= deceleracion;
                }

                //Comprueba choque con los enemigos
                for (int i = 0; i < admin->enemigos.size(); i++) {
                    if (!admin->enemigos.at(i)->borrame && !admin->enemigos.at(i)->muerto && !muriendo
                        && ((destRec.y + destRec.height / 2) >= (admin->enemigos.at(i)->destRec.y + admin->enemigos.at(i)->destRec.height / 2)
                            && (destRec.y - destRec.height / 2) <= (admin->enemigos.at(i)->destRec.y + admin->enemigos.at(i)->destRec.height / 2)
                            || (destRec.y + destRec.height / 2) >= (admin->enemigos.at(i)->destRec.y - admin->enemigos.at(i)->destRec.height / 2)
                            && (destRec.y - destRec.height / 2) <= (admin->enemigos.at(i)->destRec.y - admin->enemigos.at(i)->destRec.height / 2))
                        && ((destRec.x + destRec.width / 2 - 2) >= (admin->enemigos.at(i)->destRec.x - admin->enemigos.at(i)->destRec.width / 2)
                            && (destRec.x - destRec.width / 2 + 2) <= (admin->enemigos.at(i)->destRec.x - admin->enemigos.at(i)->destRec.width / 2)
                            || (destRec.x + destRec.width / 2 - 2) >= (admin->enemigos.at(i)->destRec.x + admin->enemigos.at(i)->destRec.width / 2)
                            && (destRec.x - destRec.width / 2 + 2) <= (admin->enemigos.at(i)->destRec.x + admin->enemigos.at(i)->destRec.width / 2))) { //Colisiona con un enemigo
                        //std::cout << "I DIED" << std::endl;
                        muriendo = true;
                        animacionActiva = DYING;
                    }
                }

                //Le dice al administrador los datos que necesita saber
                if (eresBub) {
                    admin->j1.velLateral = velocidadLateralActual;
                    admin->j1.jugadorCayendo = cayendo;
                    admin->j1.posicionJugador = destRec;
                    admin->j1.sentidoJugador = orientacionActual;
                    admin->j1.muriendo = muriendo;
                } else {
                    admin->j2.velLateral = velocidadLateralActual;
                    admin->j2.jugadorCayendo = cayendo;
                    admin->j2.posicionJugador = destRec;
                    admin->j2.sentidoJugador = orientacionActual;
                    admin->j2.muriendo = muriendo;
                }
            }

			//Actualizar puntero de animacion
			cuentaFrames++;
			if (cuentaFrames >= (targetFrames / velocidadFrames) ) {
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
					if (indiceAnimacion >= (fShootingAnimation+6)) {
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
                    indiceAnimacion++;
                    //std::cout << "Im in" << std::endl;
                    if (indiceAnimacion >= (fTranslationAnimation + 6)) { //Animacion de traslacion
                        //std::cout << "Im truly in" << std::endl;
                        //Reestablecemos las dimensiones de la animación
                        destRec.height /= 2;
                        destRec.width /= 2;
                        destRec.x += destRec.width / 2;
                        destRec.y += destRec.height / 2;
                        srcRec.height = heightAnimation;
                        srcRec.width = widthAnimation;
                        //velocidadFrames *= 2;
                        //Preparamos el personaje para que vuelva a funcionar
                        animacionActiva = STANDING;
                        indiceAnimacion = 0;
                        enElAire = true;
                        cayendo = true;
                        cambioMapa = 0;
                        cuentaFramesTraslacion = 0;
                    }
                    break;
				default:
					break;
				}
			}
		} else {
			if (numVidas > 0) {
                numVidas--;
                muerto = false;
                muriendo = false;
				enElAire = true;
				cayendo = true;
                disparando = false;
                animacionActiva = STANDING;
                destRec = inicio;
            }
		}
    };

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
        srcRec.y = (float)heightAnimation * (float)(animacionActiva % NUM_FILAS);
        DrawTexturePro(sprite, srcRec, destRec, origin, 0.0f, WHITE);
    }

    void compruebaColision(Plataforma& s) {
        if (cambioMapa == 0) {
            //Si esta en el agua no comprobamos colision
            if (enElAgua) {
                enElAire = false;
                cayendo = false;
                return;
            }
            //Comprobamos si colisiona con la superficie
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
                //std::cout << "Choque" << std::endl;
                switch (s.aproach[0]) {
                case 1:
                    destRec.x = s.left - destRec.width / 2;
                    break;
                case 2:
                    destRec.x = s.right + destRec.width / 2;
                    break;
                case 3:
                    destRec.y = s.top - destRec.height / 2;
                    enElAire = false;
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
                    //std::cout << "Aproach left" << std::endl;
                    s.aproach[0] = 1;
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
                    //std::cout << "Aproach right" << std::endl;
                    s.aproach[0] = 2;
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
                    //std::cout << "Aproach top" << std::endl;
                    s.aproach[0] = 3;
                }
                //Abajo
                else {
                    //Si no se cumplen anteriores asumimos que se acerca por debajo
                    s.aproach[0] = 4;
                }
            }
        }
    }

    void compruebaSuelo(Plataforma s) {
        if (cambioMapa == 0) {
            if (
                !(
                    //Comprobamos colision esquina inferior derecha
                    (((s.bot) > (destRec.y + destRec.height / 2)) &&
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
                enElAire = true;
                cayendo = true;
            }
        }
    }

    void compruebaPared(const Columnas& s) {
        if (cambioMapa == 0) {
            //Comprobamos columna derecha
            if (s.left_der < (destRec.x + destRec.width / 2)) {
                destRec.x = s.left_der - destRec.width / 2;
            }
            //Comprobamos columna izquierda
            else if (s.right_izq > (destRec.x - destRec.width / 2)) {
                destRec.x = s.right_izq + destRec.width / 2;
            }
        }
    }
};