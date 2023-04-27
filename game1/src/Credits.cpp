#pragma once
#include <string>
#include "raylib.h"
class Credits {
public:
    unsigned int creditos = 0;
    bool teclaPresionada = false;
    bool visibleEnPantalla = true;

    int x = 0;
    int y = 0;
    int tamano_fuente_base;
    int tamano_fuente;
    int tecla = KEY_SIX;

    bool mute_sound = false;
    Sound sound = LoadSound("resources/music/sonido_insertar_credito.mp3");

    Credits() = default;

    Credits(int x, int y, int tamano_fuente_base, int tecla, bool visibleEnPantalla = true) {
        Inicializador(x, y, tamano_fuente_base, tecla, visibleEnPantalla);
    }

    void Inicializador(int x, int y, int tamano_fuente_base, int tecla, bool visibleEnPantalla = true)
    {
        this->x = x;
        this->y = y;
        this->tamano_fuente_base = tamano_fuente_base;
        this->tecla = tecla;
        this->tamano_fuente = GetScreenHeight() / tamano_fuente_base;
        this->visibleEnPantalla = visibleEnPantalla;
    }

    ~Credits() {
        UnloadSound(sound);
    }

    void Unload() {
        UnloadSound(sound);
    };

    void Actualizar() {
        if (IsKeyDown(tecla) && !teclaPresionada)
        {
            // Realiza la acción que desees cuando se presione la tecla
            // Por ejemplo, puedes imprimir un mensaje en la consola
            teclaPresionada = true;
            creditos++;
            if (!mute_sound) {
                PlaySound(sound);
            }
            
        }

        // Comprueba si se ha liberado la tecla
        if (!IsKeyDown(tecla) && teclaPresionada)
        {
            teclaPresionada = false;  // Actualiza la variable para indicar que la tecla ha sido liberada
        }
    };

    void Dibujar() {
        if (visibleEnPantalla) {
            // Dibuja el texto "CREDITOS" y el número de créditos
            tamano_fuente = GetScreenHeight() / tamano_fuente_base;
            std::string texto = "CREDITS: " + std::to_string(creditos);
            int tamano_texto = MeasureText(texto.c_str(), tamano_fuente);
            DrawText(texto.c_str(), GetScreenWidth() - x - tamano_texto, GetScreenHeight() - y - tamano_fuente, tamano_fuente, RAYWHITE);
        }
    }


};