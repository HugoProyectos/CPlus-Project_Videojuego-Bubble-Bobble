#pragma once
#include "raylib.h"
#include <string>

class Scores {
public:
    unsigned int puntuacion1 = 0;
    unsigned int puntuacion_maxima = 300000;
    unsigned int puntuacion2 = 0;

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