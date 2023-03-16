#pragma once
#include "raylib.h"
#include <string>
#include <fstream>
#include <vector>

#define BLOQUE_PEQUENO_ANCHO 32
#define BLOQUE_PEQUENO_ALTO 26
#define BLOQUE_GRANDE_ANCHO 16
#define BLOQUE_GRANDE_ALTO 13

class Marcadores {
public:
    unsigned int puntuacion1 = 0;
    unsigned int puntuacion_maxima = 300000;
    unsigned int puntuacion2 = 0;

    float x = 0;
    float y = 0;
    float tamano_fuente_base;
    float tamano_fuente;
    Color color;

    Marcadores() = default;

    Marcadores(float x, float y, float tamano_fuente_base, Color color) {
        Inicializador(x, y, tamano_fuente_base, color);
    }

    void Inicializador(float x, float y, float tamano_fuente_base, Color color)
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

        float tamano_texto1 = MeasureText((texto1 + std::to_string(puntuacion1)).c_str(), tamano_fuente);
        float tamano_texto2 = MeasureText((texto2 + std::to_string(puntuacion_maxima)).c_str(), tamano_fuente);
        float tamano_texto3 = MeasureText((texto3 + std::to_string(puntuacion2)).c_str(), tamano_fuente);

        DrawText(texto1.c_str(), 10, 10, tamano_fuente, GREEN);
        DrawText(std::to_string(puntuacion1).c_str(), 10 + MeasureText(texto1.c_str(), tamano_fuente), 10, tamano_fuente, RAYWHITE);

        DrawText(texto2.c_str(), GetScreenWidth() / 2 - tamano_texto2 / 2, 10, tamano_fuente, RED);
        DrawText(std::to_string(puntuacion_maxima).c_str(), GetScreenWidth() / 2 - tamano_texto2 / 2 + MeasureText(texto2.c_str(), tamano_fuente), 10, tamano_fuente, RAYWHITE);

        DrawText(texto3.c_str(), GetScreenWidth() - 10 - tamano_texto3, 10, tamano_fuente, SKYBLUE);
        DrawText(std::to_string(puntuacion2).c_str(), GetScreenWidth() - 10 - tamano_texto3 + MeasureText(texto3.c_str(), tamano_fuente), 10, tamano_fuente, RAYWHITE);
    }
};

/*class Creditos {
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
*/


class NumeroNivel {
public:

    unsigned int numero_nivel = 1;
    int x = 0;
    int y = 0;
    float tamano_fuente_base;
    float tamano_fuente;
    Color color;

    NumeroNivel() = default;

    NumeroNivel(int x, int y, int tamano_fuente_base, Color color, int numero_nivel) {
        Inicializador(x, y, tamano_fuente_base, color, numero_nivel);
    }

    void Inicializador(int x, int y, int tamano_fuente_base, Color color, int numero_nivel)
    {
        this->x = x;
        this->y = y;
        this->color = color;
        this->tamano_fuente_base = tamano_fuente_base;
        this->tamano_fuente = GetScreenHeight() / (float)tamano_fuente_base;
        this->numero_nivel = numero_nivel;
    }

    ~NumeroNivel() {

    }

    void Unload() {
    };

    void Actualizar() {

    };

    void Dibujar() {
        // Dibuja el texto "CREDITOS" y el número de créditos
        tamano_fuente = GetScreenHeight() / (float)tamano_fuente_base;

        std::string texto = std::to_string(numero_nivel);

        DrawText(texto.c_str(), 10 + MeasureText(texto.c_str(), tamano_fuente), 10, tamano_fuente, RAYWHITE);

    }
};

class Plataforma {
public:
    // Posicion en la cuadricula [1..ancho] [1..alto]
    unsigned int positionX = 0;
    unsigned int positionY = 0;

    // Forma del rectangulo
    unsigned int tamanoX = 1;
    unsigned int tamanoY = 1;

    // Posicion en pantalla
    float topleft[2] = { 0,0 };


    Plataforma() = default;

    Plataforma(unsigned int positionX, unsigned int positionY, unsigned int tamanoX, unsigned int tamanoY) {
        Inicializador(positionX, positionY, tamanoX, tamanoY);
    }

    void Inicializador(unsigned int positionX, unsigned int positionY, unsigned int tamanoX, unsigned int tamanoY)
    {
        this->positionX = positionX;
        this->positionY = positionY;
        this->tamanoX = tamanoX;
        this->tamanoY = tamanoY;
        this->topleft[0] = (positionX - 1 + 2) * GetScreenWidth() / (float)BLOQUE_PEQUENO_ANCHO;
        this->topleft[1] = (positionY - 1) * GetScreenHeight() / (float)BLOQUE_PEQUENO_ALTO;
    }

    ~Plataforma(){}

    void Unload(){}

    void Actualizar(float ratioMargenSup, float ratioMargenInf)
    {
        this->topleft[0] = (positionX - 1 + 2) * GetScreenWidth() / (float)BLOQUE_PEQUENO_ANCHO;
        float tamanoMargenSup = ratioMargenSup != 0 ? GetScreenHeight() / ratioMargenSup : 0;
        float tamanoMargenInf = ratioMargenInf != 0 ? GetScreenHeight() / ratioMargenInf : 0;
        this->topleft[1] = tamanoMargenSup + (positionY - 1) * (GetScreenHeight() - tamanoMargenSup - tamanoMargenInf) / (float)BLOQUE_PEQUENO_ALTO;
    }

    void Dibujar() {

    }


};



class Plataformas {
public:
    Texture2D bloque_pequeno;

    std::vector<Plataforma> listaPlataforma;

    Rectangle srcRect;
    Rectangle destRect;

    float ratioMargenSup = 0;
    float ratioMargenInf = 0;

    Plataformas() = default;
    
    Plataformas(std::string ruta_bloque_pequeno, std::string ruta_ubicacion_bloques, float margenSuperior, float margenInferior) {
        Inicializador(ruta_bloque_pequeno, ruta_ubicacion_bloques, margenSuperior, margenInferior);
    }

    void Inicializador(std::string ruta_bloque_pequeno, std::string ruta_ubicacion_bloques, float margenSuperior, float margenInferior)
    {
        // Guardar textura 
        this->bloque_pequeno = LoadTexture(ruta_bloque_pequeno.c_str());

        // Crear una plataforma por cada linea de ruta_ubicacion_bloques
        this->listaPlataforma = this->leerArchivo(ruta_ubicacion_bloques);

        srcRect = { 0.0f, 0.0f, (float)bloque_pequeno.width, (float)bloque_pequeno.height };

        this->ratioMargenSup = margenSuperior != 0 ? GetScreenHeight() / margenSuperior : 0;
        this->ratioMargenInf = margenInferior != 0 ? GetScreenHeight() / margenInferior : 0;

    }

    std::vector<Plataforma> leerArchivo(std::string nombreArchivo) {

        std::ifstream archivo(nombreArchivo);

        std::vector<Plataforma> lista = std::vector<Plataforma>();

        if (!archivo) {
            //std::cerr << "No se pudo abrir el archivo " << nombreArchivo << std::endl;
            exit(1);
        }

        float positionX, positionY;
        int tamanoX, tamanoY;

        while (archivo >> positionX >> positionY >> tamanoX >> tamanoY) {
            Plataforma aux = Plataforma(positionX, positionY, tamanoX, tamanoY);
            lista.push_back(aux);
        }

        archivo.close();

        return lista;
    }

    ~Plataformas() {
        UnloadTexture(bloque_pequeno);
    }

    void Unload() {
        UnloadTexture(bloque_pequeno);
    }

    void Actualizar() {
        for (int i = 0; i < listaPlataforma.size(); i++) {
            listaPlataforma[i].Actualizar(ratioMargenSup, ratioMargenInf);
        }
    }

    void Dibujar() {
        float tamanoMargenSup = ratioMargenSup != 0 ? GetScreenHeight() / ratioMargenSup : 0;
        float tamanoMargenInf = ratioMargenInf != 0 ? GetScreenHeight() / ratioMargenInf : 0;
        // Destination rectangle (screen rectangle where drawing part of texture)
        float altura_bloque = (GetScreenHeight() - tamanoMargenSup - tamanoMargenInf) / (float)BLOQUE_PEQUENO_ALTO;
        float anchura_bloque = GetScreenWidth() / (float)BLOQUE_PEQUENO_ANCHO;

        for (int i = 0; i < listaPlataforma.size(); i++) {
            destRect = {
                listaPlataforma[i].topleft[0], // Posicion x de la esquina topleft
                listaPlataforma[i].topleft[1], // Posicion y de la esquina topleft
                anchura_bloque,  // anchura bloque
                altura_bloque // altura bloque
            };
            for (int tamanoX = 0; tamanoX < listaPlataforma[i].tamanoX; tamanoX++) {
                for (int tamanoY = 0; tamanoY < listaPlataforma[i].tamanoY; tamanoY++) {
                    DrawTexturePro(bloque_pequeno, srcRect, destRect, Vector2{ 0, 0 }, 0.0f, WHITE);
                    destRect.y += altura_bloque;
                }
                destRect.y = listaPlataforma[i].topleft[1];
                destRect.x += anchura_bloque;
            }
        }

    }
};

class Columnas {
public:
    Texture2D bloque_grande;

    Rectangle srcRect;
    Rectangle destRect;

    float ratioMargenSup = 0;
    float ratioMargenInf = 0;

    Columnas() = default; //Debe llamarsse a Inicializador

    Columnas(std::string ruta_bloque_grande, float margenSuperior, float margenInferior) {
        Inicializador(ruta_bloque_grande, margenSuperior, margenInferior);
    };

    void Inicializador(std::string ruta_bloque_grande, float margenSuperior, float margenInferior)
    {
        this->bloque_grande = LoadTexture(ruta_bloque_grande.c_str());
        // Source rectangle (part of the texture to use for drawing)
        srcRect = { 0.0f, 0.0f, (float)bloque_grande.width, (float)bloque_grande.height };

        this->ratioMargenSup = margenSuperior != 0 ? GetScreenHeight() / margenSuperior : 0;
        this->ratioMargenInf = margenInferior != 0 ? GetScreenHeight() / margenInferior : 0;
    }

    ~Columnas() {
        UnloadTexture(bloque_grande);
    };

    void Unload() {
        UnloadTexture(bloque_grande);
    };

    void Actualizar() {
        // TODO
    };

    void Dibujar() {
        // Destination rectangle (screen rectangle where drawing part of texture)
        float tamanoMargenSup = ratioMargenSup != 0 ? GetScreenHeight() / ratioMargenSup : 0;
        float tamanoMargenInf = ratioMargenInf != 0 ? GetScreenHeight() / ratioMargenInf : 0;
        float altura_bloque = (GetScreenHeight() - tamanoMargenSup - tamanoMargenInf) / (float)BLOQUE_GRANDE_ALTO;
        float anchura_bloque = GetScreenWidth() / (float)BLOQUE_GRANDE_ANCHO;

        // Columna izquierda
        destRect = { 0, tamanoMargenSup, anchura_bloque, altura_bloque };
        for (int i = 0; i < BLOQUE_GRANDE_ALTO; i++)
        {
            DrawTexturePro(bloque_grande, srcRect, destRect, Vector2{ 0, 0 }, 0.0f, WHITE);
            destRect.y += altura_bloque;
        }

        // Columna derecha
        destRect = {(GetScreenWidth() - anchura_bloque), tamanoMargenSup, anchura_bloque, altura_bloque};
        for (int i = 0; i < BLOQUE_GRANDE_ALTO; i++)
        {
            DrawTexturePro(bloque_grande, srcRect, destRect, Vector2{ 0, 0 }, 0.0f, WHITE);
            destRect.y += altura_bloque;
        }
    }
};

/*

//cambiar nombre de "not_main" a "main" para que el depurador entre aquí.
//Se mueve con A y S, y se salta con el espacio
int main2(void)
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
    Marcadores marcadores = Marcadores(0, 0, 20, SKYBLUE);

    SetTargetFPS(60);
    // Main game loop
    std::cout << "Inicia bucle" << std::endl;
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        columnas.Actualizar();
        plataformas.Actualizar();
        marcadores.Actualizar();
        //----------------------------------------------------------------------------------


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        columnas.Dibujar();
        plataformas.Dibujar();
        marcadores.Dibujar();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    columnas.Unload();
    plataformas.Unload();
    marcadores.Unload();

    CloseAudioDevice();
    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}*/