#pragma once
#include "Sprite.hpp"
#include "Suelo.cpp"
#include "mapa.cpp"
#include "../Scores.cpp"
#include <iostream>

class Frutas : public Sprite {


public:
	// Animacion y sprites -------------------------------------------------------
	std::string path = "resources/frutas/";
    Texture2D walkAnimation = LoadTexture("resources/frutas/cereza.png");
    Texture2D deadAnimation = LoadTexture("resources/frutas/500.png");
	Texture2D animations[2] = { walkAnimation, deadAnimation };
	int fWalkAnimation = 1; 
	int fDeadAnimation = 1;
	int fAnimation[2] = { fWalkAnimation , fDeadAnimation };
	int widthAnimation; 
	int heightAnimation;
	int animacionActiva = 0; 
	int indiceAnimacion = 0; 
	int targetFrames;
	int cuentaFrames = 0;
	int velocidadFrames = 2;
	int pixels = 16;
	//----------------------------------------------------------------------------
	int velCaida;
	unsigned int puntuacion;
    bool muerto;
	bool borrame;
    bool enElAire;
    bool eliminame;
	Plataforma lastGround;
    int alturaMax;
    clock_t temp;
    Scores* score;

	Frutas() = default;

	Frutas(std::string rutaTextura, int velCaida, float tamano, unsigned int puntuacion, int targetFrames, Rectangle destino, Scores& score) {
        this->score = &score;
		Inicializador(rutaTextura, velCaida, tamano, puntuacion, targetFrames, destino);
	}

	void Inicializador(std::string fruta, int velCaida ,float tamano, unsigned int puntuacion, int targetFrames, Rectangle destino) {
		/*
        sprite = LoadTexture((path.assign(fruta).assign(".png")).c_str());
		walkAnimation = LoadTexture((path.assign(fruta).assign(".png")).c_str());
		deadAnimation = LoadTexture(( path.assign(( std::to_string(puntuacion) )).assign(".png")).c_str() );
        */
        sprite = LoadTexture(fruta.c_str());
        

		frameWidth = 16;
		frameHeight = 16;
        this->targetFrames = targetFrames;
		srcRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
		this->tamano = tamano;
		destRec = { GetScreenWidth() / 2.0f - 50, GetScreenHeight() / 2.0f - 20, (float)frameWidth * tamano, (float)frameHeight * tamano }; 
		origin = { (float)frameWidth * tamano / 2, (float)frameHeight * tamano / 2 }; 
		this->velCaida = velCaida;
        muerto = false;
        borrame = false;
        enElAire = true;
        destRec = destino;
        alturaMax = destRec.y - 5;
        eliminame = false;
        this->puntuacion = puntuacion;
	}

	~Frutas() {
		UnloadTexture(sprite);
	};

	void Unload() {
		UnloadTexture(sprite);
	};

	void Dibujar() {
		srcRec.x = (float)widthAnimation * (float)indiceAnimacion;
		DrawTexturePro(animations[animacionActiva], srcRec, destRec, origin, 0.0f, WHITE);
	}

    void Actualizar() {

        if (eliminame) {
            if ((clock() - temp) > 2 * CLOCKS_PER_SEC) {
                borrame = true;
            }
        }
        
        

        if (muerto) {
            animacionActiva = 1;
            Asciende();
        }

        if (enElAire && !muerto) {
            CaerLento();
        }

        cuentaFrames++;
        if (cuentaFrames >= (targetFrames / velocidadFrames)) {
            cuentaFrames = 0;
            switch (animacionActiva) {
            case 0:
                indiceAnimacion = (indiceAnimacion + 1) % fWalkAnimation;
                widthAnimation = walkAnimation.width / fWalkAnimation;
                heightAnimation = walkAnimation.height;
                break;
            case 1:
                indiceAnimacion = (indiceAnimacion + 1) % fAnimation[1];
                widthAnimation = deadAnimation.width / fAnimation[1];
                heightAnimation = deadAnimation.height;
                break;
            default:
                break;
            }
        }
    }

	void CaerLento() {
        velCaida = 2;
		destRec.y += velCaida / 2;
	}

    void Asciende() {
        if( (destRec.y - velCaida / 2) > alturaMax ){
            destRec.y -= velCaida / 2;
        }
        else if (!eliminame) {
            eliminame = true;
            temp = clock();
        }
    }

    void compruebaColision(Plataforma& s, int enemyNum) {
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
            switch (s.aproach[enemyNum + 2]) {
            case 1:
               // destRec.x = s.left - destRec.width / 2;
                break;
            case 2:
                //destRec.x = s.right + destRec.width / 2;
                break;
            case 3:
                destRec.y = s.top - destRec.height / 2;
                enElAire = false;
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
                s.aproach[enemyNum + 2] = 1;
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
                s.aproach[enemyNum + 2] = 2;
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
                s.aproach[enemyNum + 2] = 3;
            }
            //Abajo
            else {
                //Si no se cumplen anteriores asumimos que se acerca por debajo
                s.aproach[enemyNum + 2] = 4;
            }
        }
    }

    void compruebaSuelo()  {
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
            ) {
            // No colisiona con plataforma
            enElAire = true;
        }
        else if (muerto) {
            enElAire = false;
            animacionActiva = 1;
        }
        else {
            enElAire = false;
        }
    }


};

typedef std::shared_ptr<Frutas> sh_Frutas;