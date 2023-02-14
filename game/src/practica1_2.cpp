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

class Asteroide {
    const float tamanoMaximo = 6.0f;
public:
    Texture2D sprite;
    int frameWidth = 0;
    int frameHeight = 0;

    Rectangle srcRec, destRec;
    Vector2 origin;
    int rotacion;
    //int razonDeCrecimiento;
    float tamano;
    Vector2 speed;
    bool sentido;

    Asteroide() = default; //Debe llamarsse a Inicializador

    Asteroide(std::string rutaTextura) {
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

        tamano = 0;
        rotacion = GetRandomValue(0, 360);
        int velocidad = 10;//(float)GetRandomValue(250, 300)/50;
        float coseno = cos(GetRandomValue(0, 360) * PI / 180.0f);
        float seno = sin(GetRandomValue(0, 360) * PI / 180.0f);
        speed = {velocidad * coseno, velocidad * seno};
        

        sentido = GetRandomValue(0, 1);

        // Destination rectangle (screen rectangle where drawing part of texture)
        destRec = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f, 0, 0 }; //Dos primeros, ubicacion. Dos ultimos, dimensiones

        // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
        origin = { (float)frameWidth * tamano / 2, (float)frameHeight * tamano / 2 };
        //Vector2 origin = { 0,0 };
        int rotation = 0;
    };

    ~Asteroide() {
        UnloadTexture(sprite);
    };

    void UnloadAsteroide() {
        UnloadTexture(sprite);
    };

    void Actualizar() {
        //Tamaño
        //tamano = std::fmod(tamano + 0.1f, tamanoMaximo);
        if (tamano < tamanoMaximo/4) {
            tamano += 0.02f;
        }
        else {
            tamano += 0.1f;
        }
        

        destRec.width = frameWidth * tamano;
        destRec.height = frameHeight * tamano;
        origin = { (float)frameWidth * tamano / 2, (float)frameHeight * tamano / 2 };

        //Actualizar posicion
        destRec.y += speed.y;
        destRec.x += speed.x;
        if (destRec.y + destRec.height / 2 <= 0 || destRec.y - destRec.height / 2 >= GetScreenHeight() 
            || destRec.x + destRec.width / 2 <= 0 || destRec.x - destRec.width / 2 >= GetScreenWidth() || tamano >= tamanoMaximo) { //sale arriba
            Inicializador();
           
        }
        
       

        //Giro
        if (sentido) {
            rotacion += 10;
        }
        else {
            rotacion -= 10;
        }

        return;
    };

    void Dibujar() {
        DrawTexturePro(sprite, srcRec, destRec, origin, (float)rotacion, WHITE);
    }

};

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    //"resources/Fire Emblem Theme WITH KAZOOS-YoutubeConvert.cc.ogg"
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
    SetMusicVolume(fun, 0.2f);
    PlayMusicStream(fun);
    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

    Asteroide espadas[numEspadas];
    for (int i = 0; i < 20; i++) {
        espadas[i].Inicializador("resources/espada_new.png");
    }

    SetTargetFPS(60);
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        for (int i = 0; i < numEspadas; i++) {
            espadas[i].Actualizar();
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        for (int i = 0; i < numEspadas; i++) {
            espadas[i].Dibujar();
        }

        //DrawText("(c) Scarfy sprite by Eiden Marsal", screenWidth - 200, screenHeight - 20, 10, GRAY);
        //std::string x_extremo = "Limite x " + std::to_string(screenWidth);
        //std::string valores = "Valor x = " + std::to_string(destRec.x) + " Valor y = " + std::to_string(destRec.y);
        //DrawText(valores.c_str(), screenWidth - 600, screenHeight - 30, 20, GRAY);
        //DrawText(x_extremo.c_str(), screenWidth - 600, screenHeight - 50, 20, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    for (int i = 0; i < numEspadas; i++) {
        espadas[i].UnloadAsteroide();
    }
    UnloadMusicStream(fun);

    CloseAudioDevice();
    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

