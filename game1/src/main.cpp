#include "MainMenu.cpp"
#include "mapa.cpp"

//cambiar nombre de "not_main" a "main" para que el depurador entre aquí.
//Se mueve con A y S, y se salta con el espacio
int main_menu(void)
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


//cambiar nombre de "not_main" a "main" para que el depurador entre aquí.
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
    Columnas columnas = Columnas("resources/mapa_nivel_1/bloque_grande.png", 20.0f, 0.0f);
    Plataformas plataformas = Plataformas("resources/mapa_nivel_1/bloque_pequeno.png", "resources/mapa_nivel_1/mapa_nivel_1_v2.txt", 20.0f, 0.0f);
    Scores scores = Scores(0, 0, 20, SKYBLUE);

    SetTargetFPS(60);
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        columnas.Actualizar();
        plataformas.Actualizar();
        scores.Actualizar();
        //----------------------------------------------------------------------------------


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        columnas.Dibujar();
        plataformas.Dibujar();
        scores.Dibujar();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    columnas.Unload();
    plataformas.Unload();
    scores.Unload();

    CloseAudioDevice();
    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

int main(void) {
    int estado = 1; // Estado inicial: menú principal

    while (true)
    {
        // Lógica de entrada de usuario
        if (estado == 1)
        {
            // Lógica del menú principal
            if (IsKeyPressed(KEY_ONE))
            {
                estado = 2; // Cambiar al estado de nivel 1
            }
            else
            {
                main_menu(); // Ejecutar el bucle del menú principal
            }
        }
        else if (estado == 2)
        {
            // Lógica del nivel 1
            if (IsKeyPressed(KEY_TWO))
            {
                estado = 1; // Cambiar al estado del menú principal
            }
            else
            {
                nivel_1(); // Ejecutar el bucle del nivel 1
            }
        }
    }
}