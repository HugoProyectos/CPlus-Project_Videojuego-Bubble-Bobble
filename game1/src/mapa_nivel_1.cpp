#include "raylib.h"
#include "clasesConSprite/Personaje.cpp"
#include <string>
#include <fstream>

#define BLOQUES_ANCHO 28
#define BLOQUES_ALTO 26

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

class Columnas {
public:
    Texture2D bloque_grande;

    Rectangle srcRect;
    Rectangle destRect;

    unsigned int margenSuperior = 0;
    unsigned int margenInferior = 0;

    Columnas() = default; //Debe llamarsse a Inicializador

    Columnas(std::string ruta_bloque_grande, int margenSuperior, int margenInferior) {
        Inicializador(ruta_bloque_grande, margenSuperior, margenInferior);
    };

    void Inicializador(std::string ruta_bloque_grande, int margenSuperior, int margenInferior)
    {
        this->bloque_grande = LoadTexture(ruta_bloque_grande.c_str());
        // Source rectangle (part of the texture to use for drawing)
        srcRect = {0.0f, 0.0f, (float)bloque_grande.width, (float)bloque_grande.height};

        this->margenSuperior = margenSuperior;
        this->margenInferior = margenInferior;
    }

    ~Columnas() {
        UnloadTexture(bloque_grande);
    };

    void Unload() {
        UnloadTexture(bloque_grande);
    };

    void Actualizar() {
        //TODO

    };

    void Dibujar() {
        // Destination rectangle (screen rectangle where drawing part of texture)
        float altura_bloque = (GetScreenHeight() - margenInferior - margenSuperior) / (float)13;
        float anchura_bloque = GetScreenWidth() / (float)16;

        // Columna izquierda
        destRect = { 0, (float)margenSuperior, anchura_bloque, altura_bloque };
        for (int i = 0; i < 13 ; i++)
        {
            DrawTexturePro(bloque_grande, srcRect, destRect, Vector2{ 0, 0 }, 0.0f, WHITE);
            destRect.y += altura_bloque;
        }

        // Columna derecha
        destRect = { (GetScreenWidth() - anchura_bloque), (float)margenSuperior, anchura_bloque, altura_bloque};
        for (int i = 0; i < 13 ; i++)
        {
            DrawTexturePro(bloque_grande, srcRect, destRect, Vector2{ 0, 0 }, 0.0f, WHITE);
            destRect.y += altura_bloque;
        }
    }
};

class Mapa {
public:
    Texture2D bloque_pequeno;
    bool huecos[BLOQUES_ALTO][BLOQUES_ANCHO];
    Rectangle srcRect;
    Rectangle destRect;

    unsigned int margenSuperior = 0;
    unsigned int margenInferior = 0;

    Mapa() = default;

    Mapa(std::string ruta_bloque_pequeno, bool (*huecos)[BLOQUES_ANCHO], int margenSuperior, int margenInferior) {
        
        Inicializador(ruta_bloque_pequeno, huecos, margenSuperior, margenInferior);
    }
    void Inicializador(std::string ruta_bloque_pequeno, bool (*huecos)[BLOQUES_ANCHO], int margenSuperior, int margenInferior)
    {
        for (int i = 0; i < BLOQUES_ALTO; i++) {
            for (int j = 0; j < BLOQUES_ANCHO; j++) {
                this->huecos[i][j] = huecos[i][j];
            }
        }
        this->bloque_pequeno = LoadTexture(ruta_bloque_pequeno.c_str());
        // Source rectangle (part of the texture to use for drawing)
        srcRect = {0.0f, 0.0f, (float)bloque_pequeno.width, (float)bloque_pequeno.height};

        this->margenSuperior = margenSuperior;
        this->margenInferior = margenInferior;
    }
    ~Mapa() {
        UnloadTexture(bloque_pequeno);
    };

    void Unload() {
        UnloadTexture(bloque_pequeno);
    };

    void Actualizar() {}

    
    void Dibujar() {
        // Destination rectangle (screen rectangle where drawing part of texture)
        float altura_bloque = (GetScreenHeight() - margenSuperior - margenInferior )/ (float)26;
        float anchura_bloque = GetScreenWidth()/ (float)32;

        float anchura_columna = GetScreenWidth() / (float)16; // Anchura de cada columna

        destRect = {
            anchura_columna, // Posicion x de la esquina topleft
            (float)margenSuperior, // Posicion y de la esquina topleft
            anchura_bloque,  // anchura bloque
            altura_bloque // altura bloque
        };

        for (int i = 0; i < BLOQUES_ALTO; i++) {
            for (int j = 0; j < BLOQUES_ANCHO; j++) {
                if (huecos[i][j]) {
                    DrawTexturePro(bloque_pequeno, srcRect, destRect, Vector2{ 0, 0 }, 0.0f, WHITE);  
                }
                destRect.x += anchura_bloque;
            }
            destRect.y += destRect.height;
            destRect.x = anchura_columna;
        }
    }
};

void leerArchivo(std::string nombreArchivo, bool matriz[BLOQUES_ALTO][BLOQUES_ANCHO]) {
    std::ifstream archivo(nombreArchivo);

    if (!archivo) {
        std::cerr << "No se pudo abrir el archivo " << nombreArchivo << std::endl;
        exit(1);
    }

    for (int i = 0; i < BLOQUES_ALTO; i++) {
        for (int j = 0; j < BLOQUES_ANCHO; j++) {
            char caracter;
            archivo >> caracter;
            if (caracter == '1') {
                matriz[i][j] = true;
            }
            else if (caracter == '0') {
                matriz[i][j] = false;
            }
            else {
                j--; // retrocede una columna para repetir la lectura
            }
        }
    }

    archivo.close();
}

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
    Columnas columnas = Columnas("resources/mapa_nivel_1/bloque_grande.png", 40,0);

    bool huecos[BLOQUES_ALTO][BLOQUES_ANCHO];

    leerArchivo("resources/mapa_nivel_1/mapa_nivel_1.txt", huecos);

    Mapa mapa = Mapa("resources/mapa_nivel_1/bloque_pequeno.png", huecos, 40, 0);
    Marcadores marcadores = Marcadores(0, 0, 20, SKYBLUE);
    NumeroNivel numero_nivel= NumeroNivel(40, 0, 20, SKYBLUE, 1);

    SetTargetFPS(60);
    // Main game loop
    std::cout << "Inicia bucle" << std::endl;
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        columnas.Actualizar();
        mapa.Actualizar();
        marcadores.Actualizar();
        numero_nivel.Actualizar();
        //----------------------------------------------------------------------------------


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        columnas.Dibujar();
        mapa.Dibujar();
        marcadores.Dibujar();
        numero_nivel.Dibujar();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    columnas.Unload();
    mapa.Unload();
    marcadores.Unload();
    numero_nivel.Unload();

    CloseAudioDevice();
    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}