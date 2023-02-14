/*******************************************************************************************
*
*   raylib game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <string>
#include <cmath>
#include <iostream>

const float velocidadSalto = 1.0f;

class Personaje {
    const float tamanoMaximo = 6.0f;
public:
    Texture2D sprite;
    int frameWidth = 0;
    int frameHeight = 0;

    Rectangle srcRec, destRec;
    Vector2 origin;
    float tamano;

    bool enElAire = false;
    bool cayendo = false;
    float saltoRecorrido = 0;
    float distanciaSaltoMax;

    Personaje() = default; //Debe llamarsse a Inicializador

    Personaje(std::string rutaTextura, float tamano) {
        Inicializador(rutaTextura, tamano);
    };
    void Inicializador(std::string rutaTextura, float tamano)
    {
        sprite = LoadTexture(rutaTextura.c_str());// Texture loading

        distanciaSaltoMax = 20.0f;

        frameWidth = sprite.width;
        frameHeight = sprite.height;

        // Source rectangle (part of the texture to use for drawing)
        srcRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight};
        this->tamano = tamano;

        Inicializador();
    }
    void Inicializador() {

        
        
        // Destination rectangle (screen rectangle where drawing part of texture)
        destRec = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f, (float)frameWidth * tamano, (float)frameHeight*tamano}; //Dos primeros, ubicacion. Dos ultimos, dimensiones

        // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
        origin = { (float)frameWidth * tamano / 2, (float)frameHeight * tamano / 2 }; //En principio no lo necesitamos
    };

    ~Personaje() {
        UnloadTexture(sprite);
    };

    void UnloadAsteroide() {
        UnloadTexture(sprite);
    };

    void Actualizar() {
        if (IsKeyPressed(KEY_X) && !enElAire) {
            enElAire = true;
            destRec.y -= velocidadSalto;
            saltoRecorrido += velocidadSalto;
        } else if (!IsKeyReleased(KEY_X) && saltoRecorrido < distanciaSaltoMax && enElAire) {
            destRec.y -= velocidadSalto;
            saltoRecorrido += velocidadSalto;
        } else if (enElAire && cayendo) {
            destRec.y += velocidadSalto;
        } else if (enElAire) {
            cayendo = true;
            destRec.y += velocidadSalto;
        }
    };

    void Dibujar() {
        DrawTexturePro(sprite, srcRec, destRec, origin, 0.0f, WHITE);
    }
};

class Suelo {
    const float tamanoMaximo = 6.0f;
public:
    Texture2D sprite;
    int frameWidth = 0;
    int frameHeight = 0;

    Rectangle srcRec, destRec;
    Vector2 origin;
    float tamano;

    Suelo() = default; //Debe llamarsse a Inicializador

    Suelo(std::string rutaTextura) {
        Inicializador(rutaTextura);
    };
    void Inicializador(std::string rutaTextura)
    {
        sprite = LoadTexture(rutaTextura.c_str());// Texture loading

        frameWidth = sprite.width;
        frameHeight = sprite.height;

        // Source rectangle (part of the texture to use for drawing)
        srcRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };

        Inicializador();
    }
    void Inicializador() {
        tamano = 1;

        // Destination rectangle (screen rectangle where drawing part of texture)
        // Destination rectangle (screen rectangle where drawing part of texture)
        destRec = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f, (float)frameWidth, (float)frameHeight}; //Dos primeros, ubicacion. Dos ultimos, dimensiones
       
        // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
        origin = { (float)frameWidth * tamano / 2, (float)frameHeight * tamano / 2 };
        //Vector2 origin = { 0,0 };
    };

    ~Suelo() {
        UnloadTexture(sprite);
    };

    void UnloadAsteroide() {
        UnloadTexture(sprite);
    };

    void Actualizar() {
       
    };

    void Dibujar() {
        DrawTexturePro(sprite, srcRec, destRec, origin, 0.0f, WHITE);
    }
};

void compruebaColisionEntorno(Personaje& p, Suelo& s) { 
   if ((p.destRec.y + p.destRec.height / 2) > (s.destRec.y - s.destRec.height / 2)) { //Choca abajo
        p.destRec.y = (s.destRec.y - s.destRec.height / 2) - p.destRec.height / 2;
        p.enElAire = false;
        p.cayendo = false;
    }
}


//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int numEspadas = 50;

    InitWindow(screenWidth, screenHeight, "Practica 1.2 - espada giratoria");
    SetWindowMinSize(200, 200);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    InitAudioDevice();

    Sound select = LoadSound("resources/select2cut.wav");
    SetSoundVolume(select, 0.5f);
    Music fun = LoadMusicStream("resources/feFun.ogg");
    //SetMusicVolume(fun, 0.2f);
    //PlayMusicStream(fun);
    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

    Personaje megaman = Personaje("resources/megaman_standing.png", 0.25f);
    Suelo suelo = Suelo("resources/Suelo.png");

    SetTargetFPS(60);
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        megaman.Actualizar();
        compruebaColisionEntorno(megaman, suelo);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        suelo.Dibujar();
        megaman.Dibujar();

        //DrawText("(c) Scarfy sprite by Eiden Marsal", screenWidth - 200, screenHeight - 20, 10, GRAY);
        std::string x_extremo = "Suelo sup y " + std::to_string(suelo.destRec.y - suelo.destRec.height/2);
        std::string valores = "Megaman inf y= " + std::to_string(megaman.destRec.y + megaman.destRec.height / 2) + " Valor H = " + std::to_string(megaman.frameHeight);
        DrawText(valores.c_str(), screenWidth - 600, screenHeight - 30, 20, GRAY);
        DrawText(x_extremo.c_str(), screenWidth - 600, screenHeight - 50, 20, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    megaman.UnloadAsteroide();
    suelo.UnloadAsteroide();
    UnloadMusicStream(fun);

    CloseAudioDevice();
    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

