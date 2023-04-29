#pragma once
#include "Sprite.hpp"
#include "Suelo.cpp"
#include "mapa.cpp"
#include <iostream>

class Boss :public Sprite {
public:
    // Render --------------------------------------------------

    int8_t cambioMapa = 2; 
    Rectangle posicionPartida = 
    { 
        (float)GetScreenWidth() / 2, 
        (float)50, 32, 32 
    };
    int cuentaFramesTraslacion = 0;
    const int LIMITE_FRAMES_TRASLACION = 180; 
    double razonX = 0;
    double razonY = 0;

    float tamano;
    float saltoMax;
    float velSalto;
    float velLateral;
    float _targetFPS;
    int lastHeight = 450;
    int lastWidth = 800;
    Rectangle destino;
    
    // --------------------------------------------------------
	// Animacion ----------------------------------------------

    int pixels = 64; //El numero de pixeles del sprite

    Texture2D walkUp =      LoadTexture("resources/boss/walkUp.png");
    Texture2D walkDown =    LoadTexture("resources/boss/walkDown.png");
    Texture2D dead =        LoadTexture("resources/boss/dead.png");
    Texture2D angry =       LoadTexture("resources/boss/angry.png");
    Texture2D trapped =     LoadTexture("resources/boss/trapped.png");
    
    Texture2D animations[5] = 
    { 
        walkUp, walkDown, dead, angry, trapped,
    };

    int fWalk    = 2; 
    int fDead    = 4; 
    int fAngry   = 2; 
    int fTrapped = 2;
    int fAnimation[4] = 
    { 
        fWalk , fDead, fAngry, fTrapped,  
    };

    int widthAnimation; 
    int heightAnimation;

    int animacionActiva = 0; 
    int indiceAnimacion = 0; 

    int targetFrames;
    int cuentaFrames = 0;
    int velocidadFrames = 4;

    // --------------------------------------------------------
    // Colisiones ---------------------------------------------

    Plataforma lastGround;
    bool enElAire = true;
    bool cayendo = true;

    // --------------------------------------------------------
	// Logica -------------------------------------------------

    int direccionX = 0; 
    int direccionY = 0; 
    bool enfadado;
    clock_t tempDisparo;
    clock_t tempAngry;
    const int disparoCoolDown = 10;
    bool disparando;
    bool sinVida;
    bool muerto;
    bool hit;
    int vida;

	// --------------------------------------------------------

	Boss() = default;
    Boss(float tamano, float saltoMax, float velSalto, float velLateral, float _targetFPS, Rectangle destino);
    void enfadar();
    void Actualizar(Rectangle playerPosition);
    void Ia(Rectangle playerPosition);
    void Dibujar();
    void MoverIzqArriba();
    void MoverIzqAbajo();
    void MoverDerArriba();
    void MoverDerAbajo();
    void compruebaColision(Plataforma& s, int enemyNum);
    void compruebaSuelo();
    void compruebaPared(const Columnas& s);
    void disparar();
    void administrarFuegos();
    void eliminarFuegos();
};