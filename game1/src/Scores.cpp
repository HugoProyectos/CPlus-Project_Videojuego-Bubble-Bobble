#pragma once
#include "raylib.h"
#include <string>

class Scores {
public:
    unsigned int puntuacion1 = 0;
    unsigned int puntuacion_maxima = 300000;
    unsigned int puntuacion2 = 0;

    bool hayP2 = false;

    std::string texto1 = "1UP ";
    std::string texto2 = "HIGH SCORE ";
    std::string texto3 = "";

    std::string textoP2[3] = { "INSERT COIN", "TO CONTINUE", "2UP " };

    unsigned int frecuencia = 120;
    unsigned int iteraciones = 0;
    unsigned int indice = 0;

    int x = 0;
    int y = 0;
    int tamano_fuente_base;
    int tamano_fuente;
    Color color;

    Scores() = default;

    Scores(int x, int y, int tamano_fuente_base, Color color) {
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

    ~Scores() {

    }

    void Unload() {
    };

    void Actualizar(unsigned int &creditos) {
        if (!hayP2) {
            if (IsKeyPressed(KEY_ONE) && (creditos >= 1)) {
                creditos--;
                // Empezar partida
            }
            else if (IsKeyPressed(KEY_TWO) && (creditos >= 2)) {
                creditos -= 2;
                hayP2 = true;
                // Empezar partida 2 jugadores
            }
            else {
                iteraciones++;
                if (iteraciones == frecuencia) {
                    indice = (indice + 1) % 2;
                    iteraciones = 0;
                }
                texto3 = textoP2[indice];
            }
        }
        else {
            texto3 = textoP2[2];
        }
        
    };

    void Dibujar() {
        // Dibuja el texto "CREDITOS" y el número de créditos
        tamano_fuente = GetScreenHeight() / tamano_fuente_base;

        

        int tamano_texto1 = MeasureText((texto1 + std::to_string(puntuacion1)).c_str(), tamano_fuente);
        int tamano_texto2 = MeasureText((texto2 + std::to_string(puntuacion_maxima)).c_str(), tamano_fuente);

        int tamano_texto3 = 0;
        if (!hayP2) {
            tamano_texto3 = MeasureText((texto3).c_str(), tamano_fuente);
        }
        else {
            tamano_texto3 = MeasureText((texto3 + std::to_string(puntuacion2)).c_str(), tamano_fuente);
        }

        DrawText(texto1.c_str(), 10, 10, tamano_fuente, GREEN);
        DrawText(std::to_string(puntuacion1).c_str(), 10 + MeasureText(texto1.c_str(), tamano_fuente), 10, tamano_fuente, RAYWHITE);

        DrawText(texto2.c_str(), GetScreenWidth() / 2 - tamano_texto2 / 2, 10, tamano_fuente, RED);
        DrawText(std::to_string(puntuacion_maxima).c_str(), GetScreenWidth() / 2 - tamano_texto2 / 2 + MeasureText(texto2.c_str(), tamano_fuente), 10, tamano_fuente, RAYWHITE);

        DrawText(texto3.c_str(), GetScreenWidth() - 10 - tamano_texto3, 10, tamano_fuente, SKYBLUE);
        if (hayP2) {
            DrawText(std::to_string(puntuacion2).c_str(), GetScreenWidth() - 10 - tamano_texto3 + MeasureText(texto3.c_str(), tamano_fuente), 10, tamano_fuente, RAYWHITE);
        }

        
        
    }
};