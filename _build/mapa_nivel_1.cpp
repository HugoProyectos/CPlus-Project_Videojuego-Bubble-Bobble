#include "raylib.h"
#include "clasesConSprite/Personaje.cpp"
#include <string>

class Marcadores {
public:
    unsigned int puntuacion1 = 0;
    unsigned int puntuacion_maxima = 300000;
    unsigned int puntuacion2 = 0;

    int x = 0;
    int y = 0;
    int tamano_fuente_base;
    int tamano_fuente;
    Color color;

    Marcadores() = default;

    Marcadores(int x, int y, int tamano_fuente_base, Color color) {
        Inicializador(x, y, tamano_fuente_base, color);
    }

    void Inicializador(int x, int y, int tamano_fuente_base, Color color)
    {
        this->x = x;
        this->y = y;
        this->color = color;
        this->tamano_fuente_base = tamano_fuente_base;
        this->tamano_fuente = GetScreenHeight() / tamano_fuente_base;
    }

    ~Marcadores() {

    }

    void Unload() {
    };

    void Actualizar() {

    };

    void Dibujar() {
        // Dibuja el texto "CREDITOS" y el número de créditos
        tamano_fuente = GetScreenHeight() / tamano_fuente_base;

        std::string texto1 = "1UP ";
        std::string texto2 = "HIGH SCORE ";
        std::string texto3 = "2UP ";

        int tamano_texto1 = MeasureText((texto1 + std::to_string(puntuacion1)).c_str(), tamano_fuente);
        int tamano_texto2 = MeasureText((texto2 + std::to_string(puntuacion_maxima)).c_str(), tamano_fuente);
        int tamano_texto3 = MeasureText((texto3 + std::to_string(puntuacion2)).c_str(), tamano_fuente);

        DrawText(texto1.c_str(), 10, 10, tamano_fuente, GREEN);
        DrawText(std::to_string(puntuacion1).c_str(), 10 + MeasureText(texto1.c_str(), tamano_fuente), 10, tamano_fuente, RAYWHITE);

        DrawText(texto2.c_str(), GetScreenWidth() / 2 - tamano_texto2 / 2, 10, tamano_fuente, RED);
        DrawText(std::to_string(puntuacion_maxima).c_str(), GetScreenWidth() / 2 - tamano_texto2 / 2 + MeasureText(texto2.c_str(), tamano_fuente), 10, tamano_fuente, RAYWHITE);

        DrawText(texto3.c_str(), GetScreenWidth() - 10 - tamano_texto3, 10, tamano_fuente, SKYBLUE);
        DrawText(std::to_string(puntuacion2).c_str(), GetScreenWidth() - 10 - tamano_texto3 + MeasureText(texto3.c_str(), tamano_fuente), 10, tamano_fuente, RAYWHITE);
    }
};

class Creditos {
public:
    unsigned int creditos = 0;
    bool teclaPresionada = false;

    int x = 0;
    int y = 0;
    int tamano_fuente_base;
    int tamano_fuente;
    int tecla = KEY_SIX;

    Creditos() = default;

    Creditos(int x, int y, int tamano_fuente_base, int tecla) {
        Inicializador(x, y, tamano_fuente_base, tecla);
    }

    void Inicializador(int x, int y, int tamano_fuente_base, int tecla)
    {
        this->x = x;
        this->y = y;
        this->tamano_fuente_base = tamano_fuente_base;
        this->tecla = tecla;
        this->tamano_fuente = GetScreenHeight() / tamano_fuente_base;
    }

    ~Creditos() {

    }

    void Unload() {
    };

    void Actualizar() {
        if (IsKeyDown(tecla) && !teclaPresionada)
        {
            // Realiza la acción que desees cuando se presione la tecla
            // Por ejemplo, puedes imprimir un mensaje en la consola
            teclaPresionada = true;
            creditos++;
        }

        // Comprueba si se ha liberado la tecla
        if (!IsKeyDown(tecla) && teclaPresionada)
        {
            teclaPresionada = false;  // Actualiza la variable para indicar que la tecla ha sido liberada
        }
    };

    void Dibujar() {
        // Dibuja el texto "CREDITOS" y el número de créditos
        tamano_fuente = GetScreenHeight() / tamano_fuente_base;
        std::string texto = "CREDITOS: " + std::to_string(creditos);
        int tamano_texto = MeasureText(texto.c_str(), tamano_fuente);
        DrawText(texto.c_str(), GetScreenWidth() - x - tamano_texto, GetScreenHeight() - y - tamano_fuente, tamano_fuente, RAYWHITE);
    }


};

class MainMenu {
public:

    Texture2D menu[3] = {
        LoadTexture("resources/main_menu/menu_1.png"),
        LoadTexture("resources/main_menu/menu_2.png"),
        LoadTexture("resources/main_menu/menu_3.png") };
    Texture2D currentMenu = menu[0];

    Music music = LoadMusicStream("resources/music/Bubble Bobble Arcade In-Game Music.mp3");

    unsigned int frecuencia;
    unsigned int iteraciones = 0;

    Rectangle srcRect;
    Rectangle destRect;

    unsigned int indice = 0;

    unsigned int margenSuperior = 0;
    unsigned int margenInferior = 0;

    MainMenu() = default; //Debe llamarsse a Inicializador

    MainMenu(int frecuencia, int margenSuperior, int margenInferior) {
        Inicializador(frecuencia, margenSuperior, margenInferior);
    };
    void Inicializador(int frecuencia, int margenSuperior, int margenInferior)
    {
        this->frecuencia = frecuencia;
        // Source rectangle (part of the texture to use for drawing)
        srcRect = { 0.0f, 0.0f, (float)currentMenu.width, (float)currentMenu.height };

        PlayMusicStream(music);

        this->margenSuperior = margenSuperior;
        this->margenInferior = margenInferior;
    }

    ~MainMenu() {
        UnloadMusicStream(music);
        UnloadTexture(menu[0]);
        UnloadTexture(menu[1]);
        UnloadTexture(menu[2]);
    };

    void Unload() {
        UnloadMusicStream(music);
        UnloadTexture(menu[0]);
        UnloadTexture(menu[1]);
        UnloadTexture(menu[2]);
    };

    void Actualizar() {
        UpdateMusicStream(music);
        iteraciones++;
        if (iteraciones == frecuencia) {
            indice = (indice + 1) % 3;
            iteraciones = 0;
        }
        currentMenu = menu[indice];
    };

    void Dibujar() {
        // Destination rectangle (screen rectangle where drawing part of texture)
        int altura = GetScreenHeight();
        destRect = { 0, (float)GetScreenHeight() / 14 , (float)GetScreenWidth(), (float)altura * 12 / 14 };
        DrawTexturePro(currentMenu, srcRect, destRect, Vector2{ 0,0 }, 0, WHITE);
    }

};

//cambiar nombre de "not_main" a "main" para que el depurador entre aquí.
//Se mueve con A y S, y se salta con el espacio
int mapa_nivel_1(void)
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
    Creditos creditos = Creditos(15, 10, 20, KEY_SIX);
    Marcadores marcadores = Marcadores(0, 0, 20, SKYBLUE);

    SetTargetFPS(60);
    // Main game loop
    std::cout << "Inicia bucle" << std::endl;
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        main_menu.Actualizar();
        creditos.Actualizar();
        marcadores.Actualizar();
        //----------------------------------------------------------------------------------


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        main_menu.Dibujar();
        creditos.Dibujar();
        marcadores.Dibujar();


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    main_menu.Unload();
    creditos.Unload();
    marcadores.Unload();

    CloseAudioDevice();
    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}