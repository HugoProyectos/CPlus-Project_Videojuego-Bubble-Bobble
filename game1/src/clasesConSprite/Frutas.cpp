#pragma once
#include "Sprite.hpp"
#include "Suelo.cpp"
#include "mapa.cpp"
#include "../Scores.cpp"
#include <iostream>

class Frutas : public Sprite {


public:
	// Animacion y sprites -------------------------------------------------------
    int i = 0; 
	std::string path = "resources/frutas/";
    Texture2D walkAnimation[7] = {
        LoadTexture("resources/frutas/platano.png"),
        LoadTexture("resources/frutas/naranja.png"),
        LoadTexture("resources/frutas/melocoton.png"),
        LoadTexture("resources/frutas/sandia.png"),
        LoadTexture("resources/frutas/uva.png"),
        LoadTexture("resources/frutas/pina.png"),
        LoadTexture("resources/frutas/diamante.png"),
    };
    
    Texture2D deadAnimation[7] = {
        LoadTexture("resources/frutas/500_bub.png"),
        LoadTexture("resources/frutas/1000_bub.png"),
        LoadTexture("resources/frutas/2000_bub.png"),
        LoadTexture("resources/frutas/3000_bub.png"),
        LoadTexture("resources/frutas/4000_bub.png"),
        LoadTexture("resources/frutas/5000_bub.png"),
        LoadTexture("resources/frutas/6000_bub.png"),

    };
    Texture2D deadAnimation2[7] = {
        LoadTexture("resources/frutas/500_bob.png"),
        LoadTexture("resources/frutas/1000_bob.png"),
        LoadTexture("resources/frutas/2000_bob.png"),
        LoadTexture("resources/frutas/3000_bob.png"),
        LoadTexture("resources/frutas/4000_bob.png"),
        LoadTexture("resources/frutas/5000_bob.png"),
        LoadTexture("resources/frutas/6000_bob.png"),

    };
	Texture2D animations[3*7] = { walkAnimation[0], deadAnimation[0], deadAnimation2[0],
                                  walkAnimation[1], deadAnimation[1], deadAnimation2[1],
                                  walkAnimation[2], deadAnimation[2], deadAnimation2[2],
                                  walkAnimation[3], deadAnimation[3], deadAnimation2[3],
                                  walkAnimation[4], deadAnimation[4], deadAnimation2[4], 
                                  walkAnimation[5], deadAnimation[5], deadAnimation2[5],
                                  walkAnimation[6], deadAnimation[6], deadAnimation2[6],
    
    };
	int fWalkAnimation = 1; 
	int fDeadAnimation = 1;
    int fDeadAnimation2 = 1;
	int fAnimation[3] = { fWalkAnimation , fDeadAnimation , fDeadAnimation2 };
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
    bool muerto_bob;
    bool muerto_bub;
	bool borrame;
    bool enElAire;
    bool eliminame;
	Plataforma lastGround;
    int alturaMax;
    clock_t temp;
    Scores* score;
    int asciende = 0;


	Frutas() = default;

	Frutas(std::string rutaTextura, int velCaida, float tamano, unsigned int puntuacion, int targetFrames, Rectangle destino, Scores& score) {
        this->score = &score;
		Inicializador(rutaTextura, velCaida, tamano, puntuacion, targetFrames, destino);
	}

    void Inicializador(std::string fruta, int velCaida, float tamano, unsigned int puntuacion, int targetFrames, Rectangle destino) {
        /*
        sprite = LoadTexture((path.assign(fruta).assign(".png")).c_str());
        walkAnimation = LoadTexture((path.assign(fruta).assign(".png")).c_str());
        deadAnimation = LoadTexture(( path.assign(( std::to_string(puntuacion) )).assign(".png")).c_str() );
        */
        sprite = LoadTexture(fruta.c_str());
        this->puntuacion = puntuacion;
        if (puntuacion == 500) {
            i = 0;
        }
        else{
            i = puntuacion / 1000;
        }

		frameWidth = 16;
		frameHeight = 16;
        this->targetFrames = targetFrames;
		srcRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
		this->tamano = tamano;
		destRec = { GetScreenWidth() / 2.0f - 50, GetScreenHeight() / 2.0f - 20, (float)frameWidth * tamano, (float)frameHeight * tamano }; 
		origin = { (float)frameWidth * tamano / 2, (float)frameHeight * tamano / 2 }; 
		this->velCaida = velCaida;
        muerto_bob = false;
        muerto_bub = false;
        borrame = false;
        enElAire = false;
        destRec = destino;
        alturaMax = destRec.y - 5;
        eliminame = false;
        animacionActiva = (i * 3);
        
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
        
        

        if (muerto_bub) {
            animacionActiva = (i*3) + 1;
            velCaida = 2;
            Asciende();
        }
        if (muerto_bob) {
            animacionActiva = (i * 3) + 2;
            velCaida = 2;
            Asciende();
        }

        if (enElAire && !muerto_bob && !muerto_bub) {
            CaerLento();
        }

        cuentaFrames++;
        if (cuentaFrames >= (targetFrames / velocidadFrames)) {
            cuentaFrames = 0;
            if (animacionActiva == (i*3)){
                indiceAnimacion = (indiceAnimacion + 1) % fWalkAnimation;
                widthAnimation = walkAnimation[0+ i].width / fWalkAnimation;
                heightAnimation = walkAnimation[0+i].height;
            }
            else if (animacionActiva == (i * 3) + 1){
                indiceAnimacion = (indiceAnimacion + 1) % fAnimation[1];
                widthAnimation = deadAnimation[1].width / fAnimation[1];
                heightAnimation = deadAnimation[1].height;
            }
            else if (animacionActiva == (i * 3) + 2){
                indiceAnimacion = (indiceAnimacion + 1) % fAnimation[2];
                widthAnimation = deadAnimation2[2].width / fAnimation[2];
                heightAnimation = deadAnimation2[2].height;
            }
        }
    }

	void CaerLento() {
        velCaida = 2;
		destRec.y += velCaida / 2;
	}

    void Asciende() {
        if( asciende < 50){
            asciende += 1;
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
            enElAire = false;
        }
        else {
            enElAire = false;
        }
    }


};

typedef std::shared_ptr<Frutas> sh_Frutas;