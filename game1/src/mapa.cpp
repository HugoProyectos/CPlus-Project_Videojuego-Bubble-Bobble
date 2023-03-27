#pragma once
#include "raylib.h"
#include <string>
#include <fstream>
#include <vector>

#define BLOQUE_PEQUENO_ANCHO 32
#define BLOQUE_PEQUENO_ALTO 26
#define BLOQUE_GRANDE_ANCHO 16
#define BLOQUE_GRANDE_ALTO 13

class Plataforma {
public:
    // Posicion en la cuadricula [1..ancho] [1..alto]
    unsigned int positionX = 0;
    unsigned int positionY = 0;

    // Forma del rectangulo
    unsigned int tamanoX = 1;
    unsigned int tamanoY = 1;

    // Posicion en pantalla
    float top = 0;
    float bot = 0;
    float left = 0;
    float right = 0;

    //Direcci�n en la que se acerca el personaje
    int aproach;

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
        this->left = (positionX - 1 + 2) * GetScreenWidth() / (float)BLOQUE_PEQUENO_ANCHO;
        this->top = (positionY - 1) * GetScreenHeight() / (float)BLOQUE_PEQUENO_ALTO;
        float anchura = GetScreenWidth() / (float)BLOQUE_PEQUENO_ANCHO * this->tamanoX;
        float altura = (GetScreenHeight()) / (float)BLOQUE_PEQUENO_ALTO * this->tamanoY;
        this->right = this->left + anchura * this->tamanoX;
        this->bot = this->top + altura * this->tamanoY;
    }

    ~Plataforma(){}

    void Unload(){}

    void Actualizar(float ratioMargenSup, float ratioMargenInf)
    {
        this->left = (positionX - 1 + 2) * GetScreenWidth() / (float)BLOQUE_PEQUENO_ANCHO;
        float tamanoMargenSup = ratioMargenSup != 0 ? GetScreenHeight() / ratioMargenSup : 0;
        float tamanoMargenInf = ratioMargenInf != 0 ? GetScreenHeight() / ratioMargenInf : 0;
        this->top = tamanoMargenSup + (positionY - 1) * (GetScreenHeight() - tamanoMargenSup - tamanoMargenInf) / (float)BLOQUE_PEQUENO_ALTO;
        float anchura = GetScreenWidth() / (float)BLOQUE_PEQUENO_ANCHO * this->tamanoX;
        float altura = (GetScreenHeight() - tamanoMargenSup - tamanoMargenInf) / (float)BLOQUE_PEQUENO_ALTO * this->tamanoY;
        this->right = this->left + anchura * this->tamanoX;
        this->bot = this->top + altura * this->tamanoY;
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
                listaPlataforma[i].left, // Posicion x de la esquina topleft
                listaPlataforma[i].top, // Posicion y de la esquina topleft
                anchura_bloque,  // anchura bloque
                altura_bloque // altura bloque
            };
            for (int tamanoX = 0; tamanoX < listaPlataforma[i].tamanoX; tamanoX++) {
                for (int tamanoY = 0; tamanoY < listaPlataforma[i].tamanoY; tamanoY++) {
                    DrawTexturePro(bloque_pequeno, srcRect, destRect, Vector2{ 0, 0 }, 0.0f, WHITE);
                    destRect.y += altura_bloque;
                }
                destRect.y = listaPlataforma[i].top;
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

    std::string numeroNivel = "";


    Columnas() = default; //Debe llamarsse a Inicializador

    Columnas(std::string ruta_bloque_grande, float margenSuperior, float margenInferior, unsigned int numeroNivel) {
        Inicializador(ruta_bloque_grande, margenSuperior, margenInferior, numeroNivel);
    };

    void Inicializador(std::string ruta_bloque_grande, float margenSuperior, float margenInferior, unsigned int numeroNivel)
    {
        this->bloque_grande = LoadTexture(ruta_bloque_grande.c_str());
        // Source rectangle (part of the texture to use for drawing)
        srcRect = { 0.0f, 0.0f, (float)bloque_grande.width, (float)bloque_grande.height };

        this->ratioMargenSup = margenSuperior != 0 ? GetScreenHeight() / margenSuperior : 0;
        this->ratioMargenInf = margenInferior != 0 ? GetScreenHeight() / margenInferior : 0;

        this->numeroNivel = std::to_string(numeroNivel);
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

        // Poner n� de nivel
        int tamano_texto = MeasureText(numeroNivel.c_str(), altura_bloque);
        DrawText(numeroNivel.c_str(), anchura_bloque / 2 - tamano_texto / 2, tamanoMargenSup, altura_bloque, RAYWHITE);
    }
};

/*

//cambiar nombre de "not_main" a "main" para que el depurador entre aqu�.
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
    Scores scores = Scores(0, 0, 20, SKYBLUE);

    SetTargetFPS(60);
    // Main game loop
    std::cout << "Inicia bucle" << std::endl;
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
}*/