#pragma once
#include "raylib.h"
#include <string>
#include <iostream>





class GameOver {
public:

    Texture2D main_screen = LoadTexture("resources/gameover/pantalla_gameover.png");

    Texture2D gameover_screen[2] = {
        LoadTexture("resources/gameover/pantalla_gameover_p1.png"),
        LoadTexture("resources/gameover/pantalla_gameover_p2.png") };

    Texture2D clear_screen[2] = {
        LoadTexture("resources/gameover/pantalla_clear_p1.png"),
        LoadTexture("resources/gameover/pantalla_clear_p2.png") };

    Music music = LoadMusicStream("resources/music/sonido_pantalla_final.mp3");
    Sound sound = LoadSound("resources/music/sonido_gameover.mp3");

    // Rectangulos del fondo
    Rectangle srcRect1;
    Rectangle destRect1;

    // Rectangulos de la parte del resultado
    Rectangle srcRect2;
    Rectangle destRect2;

    float ratioMargenSup = 0;
    float ratioMargenInf = 0;

    unsigned int tiempo_duracion;
    unsigned int iteraciones = 0;

    bool mostrar_gameover = false;

    bool hayP2 = false;
    unsigned int ronda = 0;
    bool clear = false;

    std::string texto_gameover = "GAME OVER";

    GameOver() = default; //Debe llamarsse a Inicializador

    GameOver(unsigned int tiempo_duracion, int margenSuperior, int margenInferior) {
        Inicializador(tiempo_duracion, margenSuperior, margenInferior);
    };
    void Inicializador(unsigned int tiempo_duracion, int margenSuperior, int margenInferior)
    {
        // Source rectangle (part of the texture to use for drawing)
        srcRect1 = { 0.0f, 0.0f, (float)main_screen.width, (float)main_screen.height};
        srcRect2 = { 0.0f, 0.0f, (float)gameover_screen[0].width, (float)gameover_screen[0].height};

        this->ratioMargenSup = margenSuperior != 0 ? GetScreenHeight() / margenSuperior : 0;
        this->ratioMargenInf = margenInferior != 0 ? GetScreenHeight() / margenInferior : 0;

        this->tiempo_duracion = tiempo_duracion;

        PlayMusicStream(music);
    }

    ~GameOver() {
        UnloadTexture(main_screen);
        UnloadTexture(gameover_screen[0]);
        UnloadTexture(gameover_screen[1]);
        UnloadTexture(clear_screen[0]);
        UnloadTexture(clear_screen[1]);
    };

    void Unload() {
        UnloadTexture(main_screen);
        UnloadTexture(gameover_screen[0]);
        UnloadTexture(gameover_screen[1]);
        UnloadTexture(clear_screen[0]);
        UnloadTexture(clear_screen[1]);
    };

    bool Actualizar() {
        iteraciones++;
        if (iteraciones < tiempo_duracion) {
            UpdateMusicStream(music);
        }
        if (iteraciones == tiempo_duracion) {
            PlaySound(sound);
            mostrar_gameover = true;
        }
        if (iteraciones >= (tiempo_duracion * 2)) {
            iteraciones = 0;
            
            return true;
        }
        return false;

    };

    void Dibujar() {
        float tamanoMargenSup = ratioMargenSup != 0 ? GetScreenHeight() / ratioMargenSup : 0;
        float tamanoMargenInf = ratioMargenInf != 0 ? GetScreenHeight() / ratioMargenInf : 0;
        float tamanoPantalla = (float)GetScreenHeight() - tamanoMargenSup - tamanoMargenInf;
        if (!mostrar_gameover) {
            destRect1 = { 0, (float)tamanoMargenSup , (float)GetScreenWidth(), (float)tamanoPantalla };
            DrawTexturePro(main_screen, srcRect1, destRect1, Vector2{ 0,0 }, 0, WHITE);

            destRect2 = { float(GetScreenWidth() * 0.024), (float)tamanoMargenSup + float(tamanoPantalla * 0.09) + float(tamanoPantalla * 0.077 * (ronda - 1)), float(GetScreenWidth() * 0.443), (float)(tamanoPantalla * 0.196) };
            if (!clear) {
                DrawTexturePro(gameover_screen[0], srcRect2, destRect2, Vector2{ 0,0 }, 0, WHITE);
            }
            else {
                DrawTexturePro(clear_screen[0], srcRect2, destRect2, Vector2{ 0,0 }, 0, WHITE);
            }

            DrawText(std::to_string(ronda).c_str(), float(GetScreenWidth() * 0.069), (float)tamanoMargenSup + float(tamanoPantalla * 0.09) + float(tamanoPantalla * 0.077 * (ronda - 1)) + (float)(tamanoPantalla * 0.196 * 0.498), 35, BLACK);

            if (hayP2) {
                destRect2 = {  float(GetScreenWidth() * 0.532), (float)tamanoMargenSup + float(tamanoPantalla * 0.09) + float(tamanoPantalla * 0.077 * (ronda - 1)), float(GetScreenWidth() * 0.443), (float)(tamanoPantalla * 0.196) };
                if (!clear) {
                    DrawTexturePro(gameover_screen[1], srcRect2, destRect2, Vector2{ 0,0 }, 0, WHITE);
                }
                else {
                    DrawTexturePro(clear_screen[1], srcRect2, destRect2, Vector2{ 0,0 }, 0, WHITE);
                }

                DrawText(std::to_string(ronda).c_str(), float(GetScreenWidth() * (1 - (0.069 * 2))), (float)tamanoMargenSup + float(tamanoPantalla * 0.09) + float(tamanoPantalla * 0.077 * (ronda - 1)) + (float)(tamanoPantalla * 0.196 * 0.498), 35, BLACK);
            }

        }
        else {

            int tamano = MeasureText(texto_gameover.c_str(), 40);
            DrawText(texto_gameover.c_str(), (GetScreenWidth() - tamano) / 2, tamanoMargenSup + (tamanoPantalla / 2) - 20, 40, WHITE);
        }
        

    }

};

