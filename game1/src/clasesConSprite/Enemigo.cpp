#pragma once
#include "Sprite.hpp"
#include "Suelo.cpp"
#include "mapa.cpp"
#include <iostream>
#include <time.h>
#include <cstdlib>


class Enemigo : public Sprite {
public:
    //Gestión de transición de nivel
    //int8_t cambioMapa = 2; //2->Primera Iteración 1->Desplazándose 0->Ya no
    //Rectangle posicionPartida = { (float)GetScreenWidth()/2, (float)50, 32, 32};
    //int cuentaFramesTraslacion = 0; //3 segundos = 3 * 60 frames = 180 frames
    //const int LIMITE_FRAMES_TRASLACION = 180; //3 segundos = 3 * 60 frames = 180 frames
    //double razonX = 0;
    //double razonY = 0;
    ////////////////////
 
    //VARIABLES DEL JEFE
    bool sinVida = false;
    bool hit = false;
    int vida = 0;
    bool muertoJefe = false;
    bool muertoInterno = false;

    int tipo = -1; // -2->No atrapable por pompa 1->Robot 2-> Agua NO USAR 3->Fantasma 4-> Monsta 5-> Rosa 6->Rayo
    bool borrame = false; //Eliminarlo de la lista
    bool muerto = false; //Iniciar animacion de muerte
    bool rebotando = false; //Iniciar rebote antes de muerte
    float velYRebote;
    float velXRebote;
    float deathHeight = 0;
    bool muertePorAgua = false;
    bool enElAgua = false;
    int numTileAgua;
    bool muertePorRayo = false;
    
    bool saltando = false;
    bool finSaltando = false;
    bool enElAire = false;
    bool cayendo = false;
    bool golpeado = false;
    bool enfadado = false;
    float velocidadLateral = 0;
    float velocidadSalto = 0;
    float saltoRecorrido = 0;
    float distanciaSaltoMax = 0;
    int lastHeight = 450;
    int lastWidth = 800;
    int killCount = 0;
    int animacionActiva;
    Rectangle posicion;

    Enemigo() = default; //Debe llamarsse a Inicializador

    Enemigo(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral) {
        Inicializador(rutaTextura, tamano, saltoMax, velSalto, velLateral);
    };

    void virtual Actualizar(Rectangle playerPosition1, Rectangle playerPosition2) = 0;
    void virtual Dibujar() = 0;

    void Inicializador(std::string rutaTextura, float tamano, float saltoMax, float velSalto, float velLateral)
    {
        //sprite = LoadTexture(rutaTextura.c_str());// Texture loading
        velocidadLateral = velLateral;
        velocidadSalto = velSalto;
        distanciaSaltoMax = saltoMax;

        //frameWidth = sprite.width;
        //frameHeight = sprite.height;

        frameWidth = 16;
        frameHeight = 16;


        // Source rectangle (part of the texture to use for drawing)
        srcRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
        this->tamano = tamano;

        // Destination rectangle (screen rectangle where drawing part of texture)
        destRec = { GetScreenWidth() / 2.0f - 50, GetScreenHeight() / 2.0f - 20, (float)frameWidth * tamano, (float)frameHeight * tamano }; //Dos primeros, ubicacion. Dos ultimos, dimensiones
        //destRec = { 0,0, (float)frameWidth * tamano, (float)frameHeight * tamano }; //Dos primeros, ubicacion. Dos ultimos, dimensiones

        // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
        origin = { (float)frameWidth * tamano / 2, (float)frameHeight * tamano / 2 }; //En principio no lo necesitamos
    }

    ~Enemigo() {
        UnloadTexture(sprite);
    };

    void Unload() {
        UnloadTexture(sprite);
    };

    void virtual compruebaSuelo() = 0;

    void virtual compruebaColision(Plataforma& s, int enemyNum) = 0;

    void virtual compruebaPared(const Columnas& s) = 0;

    //void Dibujar() {
    //    DrawTexturePro(sprite, srcRec, destRec, origin, 0.0f, WHITE);
    //}
};

typedef std::shared_ptr<Enemigo> sh_Enemigo;