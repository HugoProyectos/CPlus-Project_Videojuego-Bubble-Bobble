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

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int f(void) //Aleatorio y margen de velocidad
{
    //"resources/Fire Emblem Theme WITH KAZOOS-YoutubeConvert.cc.ogg"
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Practica 1.1 - T-roy rebota");
   
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    InitAudioDevice();

    Sound select = LoadSound("resources/select2cut.wav");
    SetSoundVolume(select,0.5f);
    Music fun = LoadMusicStream("resources/feFun.ogg");
    SetMusicVolume(fun,0.2f);
    //PlayMusicStream(fun);
    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

    Texture2D scarfy = LoadTexture("resources/T-roy.png");        // Texture loading

    int frameWidth = scarfy.width;
    int frameHeight = scarfy.height;

   

    // Source rectangle (part of the texture to use for drawing)
    Rectangle sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };

    // Destination rectangle (screen rectangle where drawing part of texture)
    Rectangle destRec = { screenWidth / 2.0f, screenHeight / 2.0f, frameWidth / 2.0f, frameHeight / 2.0f }; //Dos primeros, ubicacion. Dos ultimos, dimensiones

    SetWindowMinSize(destRec.width, destRec.height);
    // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
    Vector2 origin = { (float)frameWidth / 4, (float)frameHeight / 4 };
    //Vector2 origin = { 0,0 };
    int rotation = 0;

    SetTargetFPS(60);
    bool choque;
    bool sentido = true;
    //--------------------------------------------------------------------------------------
    Vector2 speed = { (float)GetRandomValue(3, 20), (float)GetRandomValue(3, 20) };
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        /*if (sentido) {
            rotation++;
        } else {
            rotation--;
        }*/
        UpdateMusicStream(fun);
        choque = false;
        //Actualizar posicion
        destRec.y += speed.y;
        destRec.x += speed.x;
        if (destRec.y - destRec.height / 2 <= 0 ) { //Choca arriba
            destRec.y = destRec.height / 2;
            speed.y *= -1;
            choque = true;
            sentido = !sentido;
        }
        if (destRec.y + destRec.height / 2 >= GetScreenHeight()) { //Choca abajo
            destRec.y = GetScreenHeight() - destRec.height/2;
            speed.y *= -1;
            choque = true;
            sentido = !sentido;
        }
        if (destRec.x - destRec.width / 2 <= 0) { //Choca izquierda
            destRec.x = destRec.width / 2;
            speed.x *= -1;
            choque = true;
            sentido = !sentido;
        }
        if (destRec.x + destRec.width / 2 >= GetScreenWidth()) { //Choca derecha
            destRec.x = GetScreenWidth() - destRec.width / 2;
            speed.x *= -1;
            choque = true;
            sentido = !sentido;
        }
        if (choque) {
            PlaySound(select);
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // NOTE: Using DrawTexturePro() we can easily rotate and scale the part of the texture we draw
        // sourceRec defines the part of the texture we use for drawing
        // destRec defines the rectangle where our texture part will fit (scaling it to fit)
        // origin defines the point of the texture used as reference for rotation and scaling
        // rotation defines the texture rotation (using origin as rotation point)
        DrawTexturePro(scarfy, sourceRec, destRec, origin, (float)rotation, WHITE);


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
    UnloadTexture(scarfy);        // Texture unloading
    UnloadMusicStream(fun);

    CloseAudioDevice();
    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}