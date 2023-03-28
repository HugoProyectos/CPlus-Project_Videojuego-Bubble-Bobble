/*#include "raylib.h"
#include "AdministradorPompas.cpp"
#include "clasesConSprite/Bub.cpp"

const int TARGET_FPS = 60;

//cambiar nombre de "not_main" a "main" para que el depurador entre aquí.
//Se mueve con A y S, y se salta con el espacio

int demo(void)

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
    AdministradorPompas admin = AdministradorPompas();

    Rectangle destRec = { GetScreenWidth() / 2.0f + 20, GetScreenHeight() / 2.0f - 20, (float)32, 32.0f }; //Dos primeros, ubicacion. Dos ultimos, dimensiones
    Pompa p = Pompa(destRec,5.0,200.0,true,100);

    Bub bub = Bub(2.0f, 30.0f, 4.0f, 2.0f,TARGET_FPS, admin);
    Suelo suelo = Suelo("resources/Suelo.png");

    SetTargetFPS(TARGET_FPS);
    // Main game loop
    std::cout << "Inicia bucle" << std::endl;
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
       bub.Actualizar();
       bub.compruebaColision(suelo);
       admin.actualizaPompas();
       //p.Actualizar();
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);//RAYWHITE);

        suelo.Dibujar();
        bub.Dibujar();
        admin.dibujaPompas();
        //p.Dibujar();

        //DrawText("(c) Scarfy sprite by Eiden Marsal", screenWidth - 200, screenHeight - 20, 10, GRAY);
        std::string x_extremo = "Suelo X izquierda " + std::to_string(suelo.destRec.x - suelo.destRec.width / 2);
        std::string x_extremo_2 = "Suelo X derecha " + std::to_string(suelo.destRec.x + suelo.destRec.width / 2);
        std::string x_extremo_3 = "Bub X derecha " + std::to_string(bub.destRec.x + bub.destRec.width / 2);
        std::string x_extremo_4 = "Bub X izquierda " + std::to_string(bub.destRec.x - bub.destRec.width / 2);
        
        std::string x_bub = "Bub x " + std::to_string(bub.destRec.x);
        std::string disparando = "disparando= " + std::to_string(bub.disparando);
        std::string valores = "Salto recorrido= " + std::to_string(bub.saltoRecorrido);
        
        //if (admin.pompas.size() > 0) {
        std::string pompa = "Posicion X de la pompa= " + std::to_string(p.destRec.x);//std::to_string(admin.pompas.at(0).destRec.x);
            DrawText(pompa.c_str(), screenWidth - 600, screenHeight - 90, 20, GRAY);
            std::string vida = "Vida de la pompa = " + std::to_string(p.tVida);
            DrawText(vida.c_str(), screenWidth - 600, screenHeight - 110, 20, GRAY);
            std::string disparada = "disparada: " + std::to_string(p.disparada);
            DrawText(disparada.c_str(), screenWidth - 600, screenHeight - 30, 20, GRAY);
            std::string animacion = "ANIMACION: " + std::to_string(p.animacionActiva);
            DrawText(animacion.c_str(), screenWidth - 600, screenHeight - 50, 20, GRAY);

            //}
            
        DrawText(x_bub.c_str(), screenWidth - 600, screenHeight - 110, 20, GRAY);
        DrawText(disparando.c_str(), screenWidth - 600, screenHeight - 70, 20, GRAY);
        DrawText(valores.c_str(), screenWidth - 600, screenHeight - 30, 20, GRAY);
        DrawText(x_extremo.c_str(), screenWidth - 600, screenHeight - 50, 20, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    bub.Unload();
    suelo.UnloadAsteroide();
    UnloadMusicStream(fun);

    CloseAudioDevice();
    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}*/