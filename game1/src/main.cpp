#include "MainMenu.cpp"
#include "mapa.cpp"
#include "AdministradorPompas.cpp"
#include "clasesConSprite/Bub.cpp"
#include "clasesConSprite/Agua.hpp"

const int TARGET_FPS = 60;

//cambiar nombre de "not_main" a "main" para que el depurador entre aqu�.
//Se mueve con A y S, y se salta con el espacio.
int not_main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Bubble Bobble");
    SetWindowMinSize(200, 200);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    InitAudioDevice();
    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

    // Cargo el fondo
    MainMenu main_menu = MainMenu(10, 40, 70);
    Credits credits = Credits(15, 10, 20, KEY_SIX);
    Scores scores = Scores(0, 0, 20, SKYBLUE);

    SetTargetFPS(60);
    // Main game loop
    std::cout << "Inicia bucle" << std::endl;
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        main_menu.Actualizar(credits.creditos);
        credits.Actualizar();
        scores.Actualizar();
        //----------------------------------------------------------------------------------


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        main_menu.Dibujar();
        credits.Dibujar();
        scores.Dibujar();


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    main_menu.Unload();
    credits.Unload();
    scores.Unload();

    CloseAudioDevice();
    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


//cambiar nombre de "not_main" a "main" para que el depurador entre aqu�.
//Se mueve con A y S, y se salta con el espacio
int nivel_1(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;


    InitWindow(screenWidth, screenHeight, "Bubble Bobble");
    SetWindowMinSize(200, 200);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    InitAudioDevice();
    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

    // Cargo el fondo
    //Mapa mapa = Mapa("resources/mapa_nivel_1/bloque_pequeno.png", "resources/mapa_nivel_1/bloque_grande.png", "resources/mapa_nivel_1/mapa_nivel_1_v2.txt");
    Columnas columnas = Columnas("resources/mapa_nivel_1/bloque_grande.png", 40.0f, 0.0f, 1);
    Plataformas plataformas = Plataformas("resources/mapa_nivel_1/bloque_pequeno.png", "resources/mapa_nivel_2/mapa_nivel_2.txt", 40.0f, 0.0f);
    Credits credits = Credits(15, 10, 20, KEY_SIX, false);
    Scores scores = Scores(0, 0, 20, SKYBLUE);

    int numPlat = plataformas.listaPlataforma.size();

    AdministradorPompas admin = AdministradorPompas();

    Texture2D spritePompa = LoadTexture("resources/Players/Bobblun/Pompa.png");
    Rectangle destRec = { GetScreenWidth() / 2.0f + 20, GetScreenHeight() / 2.0f - 20, (float)32, 32.0f }; //Dos primeros, ubicacion. Dos ultimos, dimensiones
    Pompa p = Pompa(spritePompa, destRec, 5.0, 200.0, true, 100);

    Rectangle destRob = { GetScreenWidth() / 2, 70, 32, 32 };
    sh_Enemigo robot = std::make_shared<Robot>(Robot("resources/enemyRobot/robotBasic.png", 2.0f, 40.0f, 1.0f, 1.0f, TARGET_FPS, destRob));
    admin.enemigos.push_back(robot);


    Rectangle destBub = { 100, GetScreenHeight() - 50, 32, 32};
    Bub bub = Bub(2.0f, 30.0f, 4.0f, 2.0f, TARGET_FPS, destBub, admin);
    //bub.destRec.x = 100; bub.destRec.y = 100;

    SetTargetFPS(TARGET_FPS);

    // Cargo elementos del personaje jugable

    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        columnas.Actualizar();
        plataformas.Actualizar();
        credits.Actualizar();
        scores.Actualizar();
        bub.Actualizar();
        bub.cayendo = true;
        bub.enElAire = true;
        for (int i = 0; i < numPlat; i++) {
            bub.compruebaColision(plataformas.listaPlataforma[i]);
        }
        bub.compruebaPared(columnas);
        admin.actualizaPompas();
        admin.actualizaEnemigos(plataformas);
        //----------------------------------------------------------------------------------


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        columnas.Dibujar();
        plataformas.Dibujar();
        credits.Dibujar();
        scores.Dibujar();
        bub.Dibujar();
        admin.dibujaPompas();
        admin.dibujaEnemigos();
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    columnas.Unload();
    plataformas.Unload();
    credits.Unload();
    scores.Unload();

    CloseAudioDevice();
    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
typedef enum GameScreen { MAIN_MENU, NIVEL_1, NIVEL_AGUA } GameScreen;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Bubble Bobble");
    SetWindowMinSize(200, 200);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    InitAudioDevice();

    GameScreen currentScreen = MAIN_MENU;

    // TODO: Initialize all required variables and load all required data here!
    // 
    // Global:
    //--------------------------------------------------------------------------------------
    Credits credits = Credits(15, 10, 20, KEY_SIX);
    Scores scores = Scores(0, 0, 20, SKYBLUE);
    //-------------------------------------------------------------------------------------- 
    
    // Main Menu:
    //--------------------------------------------------------------------------------------
    MainMenu main_menu = MainMenu(10, 40, 70);
    //--------------------------------------------------------------------------------------

    // Nivel 1:
    //--------------------------------------------------------------------------------------
    Columnas columnas = Columnas("resources/mapa_nivel_agua/bloque_grande_agua.png", 40.0f, 0.0f, 1);
    Plataformas plataformas = Plataformas("resources/mapa_nivel_agua/bloque_pequeno_agua.png", "resources/mapa_nivel_agua/mapa_nivel_agua.txt", 40.0f, 0.0f);
    
    int numPlat = plataformas.listaPlataforma.size();

    AdministradorPompas admin = AdministradorPompas();

    Texture2D spritePompa = LoadTexture("resources/Players/Bobblun/Pompa.png");
    Rectangle destRec = { GetScreenWidth() / 2.0f + 20, GetScreenHeight() / 2.0f - 20, (float)32, 32.0f }; //Dos primeros, ubicacion. Dos ultimos, dimensiones
    Pompa p = Pompa(spritePompa, destRec, 5.0, 200.0, true, 100);
    
    //Rectangle destRob = { GetScreenWidth()/2, 70, 32, 32};
    //sh_Enemigo robot = std::make_shared<Robot>(Robot("resources/enemyRobot/robotBasic.png", 2.0f, 80.0f, 1.0f, 1.0f, TARGET_FPS, destRob));
    //admin.enemigos.push_back(robot);

    //destRob = { (float)GetScreenWidth() / 2, 30, 32, 32 };
    //sh_Enemigo robot2 = std::make_shared<Robot>(Robot("resources/enemyRobot/robotBasic.png", 2.0f, 80.0f, 1.0f, 1.0f, TARGET_FPS, destRob));
    //dmin.enemigos.push_back(robot2);

    Rectangle destBub = { 100, GetScreenHeight() - 50, 32, 32 };
    Bub bub = Bub(2.0f, 30.0f, 4.0f, 2.0f, TARGET_FPS, destBub, admin);

    Texture2D spriteAgua = LoadTexture("resources/agua.png");
    Rectangle destAgua = { 150, 100, 16, 16 };
    Agua agua = Agua(destAgua, true, spriteAgua, numPlat);

    //bub.destRec.x = 100; bub.destRec.y = 100;
    
    //--------------------------------------------------------------------------------------

    int framesCounter = 0;          // Useful to count frames

    SetTargetFPS(60);               // Set desired framerate (frames-per-second)
    //--------------------------------------------------------------------------------------

    bub.cayendo = true;
    bub.enElAire = true;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        switch (currentScreen)
        {
        case MAIN_MENU:
        {
            // TODO: Update MAIN_MENU screen variables here!
            main_menu.Actualizar(credits.creditos);
            credits.Actualizar();
            scores.Actualizar();

            // Wait for 2 seconds (120 frames) before jumping to TITLE screen
            if (IsKeyPressed(KEY_ONE) && credits.creditos >= 1)
            {
                currentScreen = NIVEL_1;
                credits.creditos -= 1;
                scores.hayP1 = true;
            }
            else if (IsKeyPressed(KEY_TWO) && credits.creditos >= 2) {
                currentScreen = NIVEL_1;
                credits.creditos -= 2;
                scores.hayP1 = true;
                scores.hayP2 = true;
            }
        } break;
        case NIVEL_1:
        {
            // TODO: Update NIVEL_1 screen variables here!
            columnas.Actualizar();
            plataformas.Actualizar();
            credits.Actualizar();
            scores.Actualizar();
            bub.Actualizar();
            for (int i = 0; i < numPlat; i++) {
                bub.compruebaColision(plataformas.listaPlataforma[i]);
            }
            bub.compruebaPared(columnas);
            if (bub.enElAgua) {
                bub.destRec.x = agua.stream[agua.bubTile].destRec.x;
                bub.destRec.y = agua.stream[agua.bubTile].destRec.y;
            }
            else {
                agua.colisionBub(bub);
            }
            agua.Actualizar(plataformas,columnas);
            admin.actualizaPompas();
            admin.actualizaEnemigos(plataformas);

            if (IsKeyPressed(KEY_TWO) && credits.creditos >= 1 && scores.hayP1 && !scores.hayP2)
            {
                credits.creditos -= 1;
                scores.hayP2 = true;
            }

        } break;
        default: break;
        }
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_D)) {
            char a;
            std::cin >> a;
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        switch (currentScreen)
        {
        case MAIN_MENU:
        {
            // TODO: Draw MAIN_MENU screen here!
            main_menu.Dibujar();
            credits.Dibujar();
            scores.Dibujar();

        } break;
        case NIVEL_1:
        {
            // TODO: Draw NIVEL_1 screen here!
            columnas.Dibujar();
            plataformas.Dibujar();
            scores.Dibujar();
            agua.Dibujar();
            bub.Dibujar();
            admin.dibujaPompas();
            admin.dibujaEnemigos();
            
        } break;
        default: break;
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    // TODO: Unload all loaded data (textures, fonts, audio) here!
    main_menu.Unload();
    columnas.Unload();
    plataformas.Unload();
    credits.Unload();
    scores.Unload();

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}