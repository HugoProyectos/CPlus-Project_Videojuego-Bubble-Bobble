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
    Texture2D fruit = LoadTexture("resources/frutas/frutas.png");
    
    Texture2D bubPoints = LoadTexture("resources/puntos/puntosBub.png");
    
    Texture2D bobPoints = LoadTexture("resources/frutas/puntosBub.png");

    Texture2D animaciones[3] = { fruit, bubPoints, bobPoints };

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
    int lastHeight;
    int lastWidth;
    int ratioX;
    int ratioY;
    int ascenso;


	Frutas() = default;

	Frutas(std::string rutaTextura, int velCaida, float tamano, unsigned int puntuacion, int targetFrames, Rectangle destino, Scores& score, int lastHeight, int lastWidth
            , int ratioX, int ratioY, Vector2 origin) {
        this->score = &score;
		Inicializador(rutaTextura, velCaida, tamano, puntuacion, targetFrames, destino, lastHeight, lastWidth, ratioX, ratioY, origin);
	}

    void Inicializador(std::string fruta, int velCaida, float tamano, unsigned int puntuacion, int targetFrames, Rectangle destino, int lastHeight, int lastWidth, int ratioX, 
        int ratioY, Vector2 origin) {
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
		srcRec = { i*16.0f, 0.0f, 16.0f, 16.0f };
		this->tamano = tamano;
		destRec = { GetScreenWidth() / 2.0f - 50, GetScreenHeight() / 2.0f - 20, (float)frameWidth * tamano, (float)frameHeight * tamano }; 
		this->origin = origin;
		this->velCaida = velCaida;
        muerto_bob = false;
        muerto_bub = false;
        borrame = false;
        enElAire = false;
        destRec = destino;
        alturaMax = destRec.y - 5;
        eliminame = false;
        animacionActiva = 0;
        this->lastHeight = lastHeight;
        this->lastWidth = lastWidth;
        this->ratioX = ratioX;
        this->ratioY = ratioY;
        asciende = 0;
        ascenso = 50 + rand()%(50-101);
        
	}

	~Frutas() {
		UnloadTexture(sprite);
	};

	void Unload() {
		UnloadTexture(sprite);
	};

	void Dibujar() {
		DrawTexturePro(animaciones[animacionActiva], srcRec, destRec, origin, 0.0f, WHITE);
	}

    void Actualizar() {
        // Reescalado --------------------------------------------------------------------
        if (lastHeight != GetScreenHeight()) {
            destRec.height = GetScreenHeight() / 14.0625f;
            destRec.y = GetScreenHeight() * (destRec.y / lastHeight);
            origin.y = destRec.height / 2;

            ratioY = destRec.height / 32;
            lastHeight = GetScreenHeight();
        }
        if (lastWidth != GetScreenWidth()) {
            destRec.width = GetScreenWidth() / 25.0f;
            destRec.x = GetScreenWidth() * (destRec.x / lastWidth);
            origin.x = destRec.width / 2;

            ratioX = destRec.width / 32;
            lastWidth = GetScreenWidth();
        }

        // --------------------------------------------------------------------------------
        

        if (eliminame) {
            if ((clock() - temp) > 2 * CLOCKS_PER_SEC) {
                borrame = true;
            }
        }
        
        
        if (muerto_bub || muerto_bob) {
            switch (i) {
                case 0:
                    srcRec = {306,0,15,8};
                    break;
                case 1:
                    srcRec = {487,0,15,9 };
                    break;
                case 2:
                    srcRec = {522,0,16,9 };
                    break;
                case 3:
                    srcRec = {540,0,16,9 };
                    break;
                case 4:
                    srcRec = {558,0,16,9 };
                    break;
                case 5:
                    srcRec = {576,0,16,9 };
                    break;
                default:
                    srcRec = { 594,0,16,9 };
            }
        }
        if (muerto_bub) {
            animacionActiva = 1;
            velCaida = 2;
            Asciende();
        }
        if (muerto_bob) {
            animacionActiva = 2;
            velCaida = 2;
            Asciende();
        }


        if (enElAire && !muerto_bob && !muerto_bub) {
            CaerLento();
        }

        cuentaFrames++;
        /*if (cuentaFrames >= (targetFrames / velocidadFrames)) {
            cuentaFrames = 0;
            if (animacionActiva == (i*3)){
                indiceAnimacion = (indiceAnimacion + 1) % fWalkAnimation;
                widthAnimation = walkAnimation[0+ i].width / fWalkAnimation;
                heightAnimation = walkAnimation[0+i].height;
            }
        }*/
    }

	void CaerLento() {
        velCaida = 2;
		destRec.y += (velCaida / 2);
	}

    void Asciende() {
        if( asciende < ascenso){
            //std::cout << "hola" << std::endl;
            asciende += 1;
            destRec.y -= (velCaida / 2);
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
                (s.bot > ( destRec.y + 16 ))
                &&
                (s.top < (destRec.y +16 ))
            )){
                //std::cout << "sdsd" << std::endl;
                //destRec.y = (s.top - destRec.height / 2.0f) + ratioY;
                enElAire = false;
                lastGround = s;
                }
    }
};

typedef std::shared_ptr<Frutas> sh_Frutas;