#pragma once
#include "Sprite.hpp"
#include "Suelo.cpp"
#include "Pompa.hpp"
#include "mapa.cpp"
#include "../AdministradorPompas.cpp"
#include <iostream>



//Se ha decidido que tengan sprites distintos para cada animación,
//La estructura difiere del modelo original genérico de personaje por ello.
class Bub : public Sprite {
    const int STANDING = 0;
    const int MOVING = 1;
    const int JUMPING = 2;
    const int FALLING = 3;
    const int SHOOTING = 4;
    const int DYING = 5;
    const Texture2D spriteBurbuja = LoadTexture("resources/Players/Bobblun/Pompa.png");

    const int NUM_FILAS = 4; //número de filas en el animation_set

    int fStandingAnimation = 4; //Número de fotogramas de la StandingAnimation
    int fMovingAnimation = 4; //Número de fotogramas
    int fJumpingAnimation = 4; //Número de fotogramas
    int fFallingAnimation = 4; //Número de fotogramas
    int fShootingAnimation = 5; //Número de fotogramas
    int fDeathAnimation = 30; //Número de fotogramas

    const float VELOCIDAD_DISPARO = 5.0;
    const float DISTANCIA_DISPARO = 200.0;
    Rectangle inicio = { -1,-1,-1,-1 };

public:
    
    // VARIABLE DE ULTIMA PLATAFORMA SUELO
    Plataforma lastGround;

    // VARIABLES PARA LA GENERACIÓN DE POMPAS
    bool disparando = false;
    int multiplicadorVelocidadDisparo = 1;
    int multiplicadorDistanciaDisparo = 1;

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
    bool enElAire = false;
    bool cayendo = false;
    float saltoRecorrido = 0;
    float distanciaSaltoMax = 0;
    float velocidadSalto = 0;  //Añadir aceleracion, y hacer que velocidad nueva = velocidad anterior + aceleracion. Empezar con una aceleracion inicial, y que se le vayan restando valores. A partir de cierta velocidad, capar. Hay que comprobar que el tope coincida con el planeo.
    float velocidadLateral = 0;
    float deceleracion = 0.1f;
    float velocidadActual = 0;

    Bub() = default; //Debe llamarse a Inicializador

    Bub(float tamano, float saltoMax, float velSalto, float velLateral, int _targetFrames, Rectangle destino, AdministradorPompas& adm) {
        Inicializador(tamano, saltoMax, velSalto, velLateral, _targetFrames, adm);
		destRec = destino;
        inicio = destino;
    };
    void Inicializador(float tamano, float saltoMax, float velSalto,float velLateral, int _targetFrames, AdministradorPompas& adm)
    {
        //
        admin = &adm;

        sprite = LoadTexture("resources/Players/Bobblun/animation_set.png");
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
        //Gestión de desplazamiento lateral
        
		if(!muerto){
            if (admin->j1DebeRebotar > 0) { //Si rebota sobre una pompa, es como iniciar un nuevo salto
                std::cout << "Me dicen que rebote" << std::endl;
                saltoRecorrido = 0; 
                admin->j1DebeRebotar = 0;
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
							//(OPCIONAL) Añadir que decelere en vez de cambiar repentinamente de velocidad
							switchOrientacion = 3;
							destRec.x -= velocidadLateral/3;
							dirAir = 2;
						}
						else {
							if (IsKeyDown(KEY_A)) {
								switchOrientacion = 2;
								dirAir = 1;
								destRec.x -= velocidadLateral;
							}
							else if (dirAir == 1) {
								destRec.x -= velocidadLateral;
							}
							else {
								destRec.x -= velocidadLateral/3;
							}
						}
					}
					else if (dirCorrer == 2) {  //Salta derecha
						if (IsKeyDown(KEY_A)) {
							//(OPCIONAL) Añadir que decelere en vez de cambiar repentinamente de velocidad
							switchOrientacion = 2;
							destRec.x += velocidadLateral/3;
							dirAir = 1;
						}
						else {
							if (IsKeyDown(KEY_S)) {
								switchOrientacion = 3;
								dirAir = 2;
								destRec.x += velocidadLateral;
							}
							else if (dirAir == 2) {
								destRec.x += velocidadLateral;
							}
							else {
								destRec.x += velocidadLateral/3;
							}
						}
					}
					else {
						if (IsKeyDown(KEY_A)) {
							switchOrientacion = 2;
							destRec.x -= velocidadLateral / 2;
						}
						else if (IsKeyDown(KEY_S)) {
							switchOrientacion = 3;
							destRec.x += velocidadLateral / 2;
						}
					}
				}else {
					if (IsKeyDown(KEY_A)) {
						if (!disparando && !muriendo) animacionActiva = MOVING;
						switchOrientacion = 2;
						destRec.x -= velocidadLateral/2;
					}else if(IsKeyDown(KEY_S)) {
						if (!disparando && !muriendo) animacionActiva = MOVING;
						switchOrientacion = 3;
						destRec.x += velocidadLateral/2;
					}
				}
			}

			// Se puede disparar en el aire. Las acciones en el aire no se ven limitadas por el disparo, 
			// pero las del suelo sí. Para mantener la idea if/else de en el aire o en el suelo, 
			// al del suelo se le ha añaido la restricción opuesta al de en el aire (!enElAire)
			if (IsKeyPressed(KEY_F) && !disparando && !muriendo) { 
				//std::cout << "Dispara" << std::endl;
				int sentido = 1; //Hacia la derecha
				if (orientacionActual == 2) { //Si es hacia la izquierda
					sentido = -1;
				}
				disparando = true;
				animacionActiva = SHOOTING;
				indiceAnimacion = 6; //Es el 0 de la segunda parte de animaciones
				Pompa p = Pompa(spriteBurbuja, destRec, VELOCIDAD_DISPARO * multiplicadorVelocidadDisparo * sentido, DISTANCIA_DISPARO * multiplicadorDistanciaDisparo, true, 500);
				admin->pompas.push_back(p);
			} else if(!enElAire) {
				if (IsKeyDown(KEY_A) && !muriendo) {
					if (!disparando) animacionActiva = MOVING;
					switchOrientacion = 2;
					//std::cout << "Muevo Izquierda, orientacion: " << switchOrientacion << std::endl;
					destRec.x -= velocidadLateral;
					dirCorrer = 1;
					dirAir = 1;
					compruebaSuelo(lastGround);
				}
				else if (IsKeyDown(KEY_S) && !muriendo) {
					if (!disparando) animacionActiva = MOVING;
					switchOrientacion = 3;
					//std::cout << "Muevo Derecha, orientacion: " << switchOrientacion << std::endl;
					destRec.x += velocidadLateral;
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

			//Gestión de salto
			if (IsKeyPressed(KEY_SPACE) && !enElAire && !muriendo) {

				//std::cout << "Salto" << std::endl;
				if (!disparando) animacionActiva = JUMPING;
				enElAire = true;
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
			} else if (enElAire && cayendo) { //Planeando
				//std::cout << "I'm gliding" << std::endl;
				if (!disparando && !muriendo) animacionActiva = FALLING;
				destRec.y += velocidadSalto / 2;
				saltoRecorrido -= velocidadSalto / 2;
			}
			else if (enElAire) { //Inicio caída
				if(!disparando && !muriendo) animacionActiva = FALLING;
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
                    && ((destRec.x + destRec.width / 2) >= (admin->enemigos.at(i)->destRec.x - admin->enemigos.at(i)->destRec.width / 2)
                        && (destRec.x - destRec.width / 2) <= (admin->enemigos.at(i)->destRec.x - admin->enemigos.at(i)->destRec.width / 2)
                        || (destRec.x + destRec.width / 2) >= (admin->enemigos.at(i)->destRec.x + admin->enemigos.at(i)->destRec.width / 2)
                        && (destRec.x - destRec.width / 2) <= (admin->enemigos.at(i)->destRec.x + admin->enemigos.at(i)->destRec.width / 2))) { //Colisiona con un enemigo
                    //std::cout << "I DIED" << std::endl;
                    muriendo = true;
                    animacionActiva = DYING;
                }
            }

            //Le dice al administrador los datos que necesita saber
            admin->jugadorCayendo = cayendo;
            admin->posicionJugador = destRec;
            admin->sentidoJugador = orientacionActual;
            admin->muriendo = muriendo;
			
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
        srcRec.x = (float)widthAnimation * (float)indiceAnimacion; //Cambia el fotograma
        if ((switchOrientacion == 2 && orientacionActual == 3) || (switchOrientacion == 3 && orientacionActual == 2)) {
            //std::cout << "Cambio de orientacion" << std::endl;
            srcRec.width *= -1; //Cambia la orientacion
            orientacionActual = switchOrientacion;
        }
        //std::cerr << "Indice Animacion: " << indiceAnimacion << std::endl;
        srcRec.y = (float)widthAnimation * (float)(animacionActiva%NUM_FILAS);
        DrawTexturePro(sprite, srcRec, destRec, origin, 0.0f, WHITE);
    }

    void compruebaColision(Plataforma& s) {
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
                        ((s.right ) > (destRec.x - destRec.width / 2)) &&
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

    void compruebaSuelo(Plataforma s) {
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

    void compruebaPared(const Columnas& s) {
        //Comprobamos columna derecha
        if (s.left_der < (destRec.x + destRec.width/2) ) {
            destRec.x = s.left_der - destRec.width / 2;
        }
        //Comprobamos columna izquierda
        else if (s.right_izq > (destRec.x - destRec.width / 2) ) {
            destRec.x = s.right_izq + destRec.width / 2;
        }
    }
};