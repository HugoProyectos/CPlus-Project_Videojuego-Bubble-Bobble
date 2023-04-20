#include <string>
#include <iostream>
#include "Credits.cpp"
#include "Scores.cpp"




class MainMenu{
public:

    Texture2D menu[5] = { 
        LoadTexture("resources/main_menu/menu_1.png"),
        LoadTexture("resources/main_menu/menu_2.png"),
        LoadTexture("resources/main_menu/menu_3.png"),
        LoadTexture("resources/main_menu/menu_4.png"),
        LoadTexture("resources/main_menu/menu_5.png") };
    Texture2D currentMenu = menu[0];

    Music music = LoadMusicStream("resources/music/sonido_main_menu.mp3");
    
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
        UnloadTexture(menu[3]);
        UnloadTexture(menu[4]);
    };

    void Unload() {
        UnloadMusicStream(music);
        UnloadTexture(menu[0]);
        UnloadTexture(menu[1]);
        UnloadTexture(menu[2]);
        UnloadTexture(menu[3]);
        UnloadTexture(menu[4]);
    };

    void Actualizar(int credits) {
        if (credits == 1) {
            currentMenu = menu[3];
        }
        else if (credits > 1) {
            currentMenu = menu[4];
        }
        else {
            UpdateMusicStream(music);
            iteraciones++;
            if (iteraciones == frecuencia) {
                indice = (indice + 1) % 3;
                iteraciones = 0;
            }
            currentMenu = menu[indice];
        }
        
    };

    void Dibujar() {
        // Destination rectangle (screen rectangle where drawing part of texture)
        int altura = GetScreenHeight();
        destRect = { 0, (float)GetScreenHeight() / 14 , (float)GetScreenWidth(), (float)altura * 12 / 14 };
        DrawTexturePro(currentMenu, srcRect, destRect, Vector2 {0,0}, 0, WHITE);
    }

};
