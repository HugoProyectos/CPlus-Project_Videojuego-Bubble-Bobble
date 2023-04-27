#include <string>
#include <iostream>
#include "Credits.cpp"
#include "Scores.cpp"




class MainMenu{
public:

    Texture2D menu[6] = { 
        LoadTexture("resources/main_menu/menu_1.png"),
        LoadTexture("resources/main_menu/menu_2.png"),
        LoadTexture("resources/main_menu/menu_3.png"),
        LoadTexture("resources/main_menu/menu_4.png"),
        LoadTexture("resources/main_menu/menu_5.png"),
        LoadTexture("resources/main_menu/options.png")};
    Texture2D currentMenu = menu[0];

    bool mute_music = false;
    Music music = LoadMusicStream("resources/music/sonido_main_menu.mp3");
    
    unsigned int frecuencia;
    unsigned int iteraciones = 0;

    Rectangle srcRect;
    Rectangle destRect;

    Rectangle srcRectOptions;

    unsigned int indice = 0;

    unsigned int margenSuperior = 0;
    unsigned int margenInferior = 0;

    Vector2 mousePosition;

    MainMenu() = default; //Debe llamarsse a Inicializador

    MainMenu(int frecuencia, int margenSuperior, int margenInferior) {
        Inicializador(frecuencia, margenSuperior, margenInferior);
    };
    void Inicializador(int frecuencia, int margenSuperior, int margenInferior)
    {
        this->frecuencia = frecuencia;
        // Source rectangle (part of the texture to use for drawing)
        srcRect = { 0.0f, 0.0f, (float)currentMenu.width, (float)currentMenu.height };

        srcRectOptions = { 0.0f, 0.0f, (float)menu[5].width, (float)menu[5].height };

        PlayMusicStream(music);

        this->margenSuperior = margenSuperior;
        this->margenInferior = margenInferior;
    }

    ~MainMenu() {
        UnloadMusicStream(music);
        UnloadTexture(menu[0]);
        UnloadTexture(menu[1]);
        UnloadTexture(menu[2]);
        UnloadTexture(menu[3]);
        UnloadTexture(menu[4]);
        UnloadTexture(menu[5]);
    };

    void Unload() {
        UnloadMusicStream(music);
        UnloadTexture(menu[0]);
        UnloadTexture(menu[1]);
        UnloadTexture(menu[2]);
        UnloadTexture(menu[3]);
        UnloadTexture(menu[4]);
        UnloadTexture(menu[5]);
    };

    bool Actualizar(int credits) {
        if (credits == 1) {
            currentMenu = menu[3];
        }
        else if (credits > 1) {
            currentMenu = menu[4];
        }
        else {
            if (!mute_music) {
                UpdateMusicStream(music);
            }
            
            iteraciones++;
            if (iteraciones == frecuencia) {
                indice = (indice + 1) % 3;
                iteraciones = 0;
            }
            currentMenu = menu[indice];
        }
        mousePosition = GetMousePosition();
        destRect = { 0, (float)GetScreenHeight() - margenInferior / 2, (float)margenInferior / 2, (float)margenInferior / 2 };
        if (CheckCollisionPointRec(mousePosition, destRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // El clic se ha hecho dentro de la imagen
            return true;
        }
        return false;
        
    };

    void Dibujar() {
        // Destination rectangle (screen rectangle where drawing part of texture)
        int altura = GetScreenHeight();
        destRect = { 0, (float)GetScreenHeight() / 14 , (float)GetScreenWidth(), (float)altura * 12 / 14 };
        DrawTexturePro(currentMenu, srcRect, destRect, Vector2 {0,0}, 0, WHITE);

        destRect = { 0, (float)GetScreenHeight() - margenInferior/2, (float)margenInferior / 2, (float)margenInferior / 2};
        DrawTexturePro(menu[5], srcRectOptions, destRect, Vector2{0,0}, 0, WHITE);
    }

};
