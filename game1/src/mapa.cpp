#pragma once
#include "raylib.h"
#include <string>
#include <fstream>
#include <vector>
#include <algorithm> // for std::copy
#include <iostream> // for debugging

#define BLOQUE_PEQUENO_ANCHO 32
#define BLOQUE_PEQUENO_ALTO 26
#define BLOQUE_GRANDE_ANCHO 16
#define BLOQUE_GRANDE_ALTO 13
#define FRAMES_CARGAR_SIGUIENTE_NIVEL 120

const double ratioAspecto = 800 / 450;

class Plataforma {
public:
    // Posicion en la cuadricula [1..ancho] [1..alto]
    float positionX = 0;
    float positionY = 0;

    // Forma del rectangulo
    unsigned int tamanoX = 1;
    unsigned int tamanoY = 1;

    // Posicion en pantalla
    float top = 0;
    float bot = 0;
    float left = 0;
    float right = 0;

    //Dirección en la que se acerca el personaje
    int aproach[30]; //Posiciones 0 y 1 reservadas para Bub y Bob. Posiciones a partir de la 20 de las bolas

    Plataforma() = default;

    Plataforma(float positionX, float positionY, unsigned int tamanoX, unsigned int tamanoY) {
        Inicializador(positionX, positionY, tamanoX, tamanoY);
    }

    void Inicializador(float positionX, float positionY, unsigned int tamanoX, unsigned int tamanoY)
    {
        this->positionX = positionX;
        this->positionY = positionY;
        this->tamanoX = tamanoX;
        this->tamanoY = tamanoY;
        this->left = (positionX - 1 + 2) * GetScreenWidth() / (float)BLOQUE_PEQUENO_ANCHO;
        this->top = (positionY - 1) * GetScreenHeight() / (float)BLOQUE_PEQUENO_ALTO;
        float anchura = GetScreenWidth() / (float)BLOQUE_PEQUENO_ANCHO * this->tamanoX;
        float altura = (GetScreenHeight()) / (float)BLOQUE_PEQUENO_ALTO * this->tamanoY;
        this->right = this->left + anchura;
        this->bot = this->top + altura;
    }

    ~Plataforma() {}

    void Unload() {}

    void Actualizar(float ratioMargenSup, float ratioMargenInf)
    {
        float tamanoMargenSup = ratioMargenSup != 0 ? GetScreenHeight() / ratioMargenSup : 0;
        float tamanoMargenInf = ratioMargenInf != 0 ? GetScreenHeight() / ratioMargenInf : 0;
        float anchura = GetScreenWidth() / (float)BLOQUE_PEQUENO_ANCHO * this->tamanoX;
        float altura = (GetScreenHeight() - tamanoMargenSup - tamanoMargenInf) / (float)BLOQUE_PEQUENO_ALTO * this->tamanoY;
        this->left = (positionX - 1 + 2) * GetScreenWidth() / (float)BLOQUE_PEQUENO_ANCHO;
        this->top = tamanoMargenSup + (positionY - 1) * (GetScreenHeight() - tamanoMargenSup - tamanoMargenInf) / (float)BLOQUE_PEQUENO_ALTO;
        this->right = this->left + anchura;
        this->bot = this->top + altura;
        if (this->positionY == 1) {
            this->top -= (altura * 4);
        }
        else if (this->positionY == BLOQUE_PEQUENO_ALTO) {
            this->bot += (altura * 2);
        }
    }

    void Dibujar() {

    }


};



class Plataformas {
public:
    Texture2D bloque_pequeno;
    Texture2D bloque_pequeno_2d;

    std::vector<Plataforma> listaPlataforma;

    Rectangle srcRect;
    Rectangle srcRect_2d;
    Rectangle destRect;

    float ratioMargenSup = 0;
    float ratioMargenInf = 0;

    // Variables para cargar siguiente nivel
    bool cargando_nivel_siguiente = false;
    Texture2D bloque_pequeno_siguiente;
    Texture2D bloque_pequeno_siguiente_2d;
    std::vector<Plataforma> listaPlataformaSiguiente;
    float distancia_ya_movida = 0;

    bool mostrar_inicio_ronda = false;
    bool empezar_contador = false;
    unsigned int iteraciones = 0;
    std::string texto_inicio_ronda = "";

    std::string texto_hurryup = "HURRY UP!!!";
    bool mostrar_hurryup = false;
    bool empezar_contador2 = false;
    unsigned int iteraciones2 = 0;
    unsigned int indice = 0;
    Color colores[2] = { YELLOW, MAGENTA };
    Sound sound = LoadSound("resources/music/sonido_fantasma_inmortal.mp3");

    bool modo_2d = false; 

    int musicNum = 0;
    int lastMusicNum = 0;
    Music music = LoadMusicStream("resources/music/sonido_niveles.mp3");
    Music music2 = LoadMusicStream("resources/music/sonido_niveles_alternativo.mp3");
    Music music3 = LoadMusicStream("resources/music/sonido_niveles_gigachad.mp3");
    Music musicHurry = LoadMusicStream("resources/music/sonido_prisa.mp3");
    Music musicHurry2 = LoadMusicStream("resources/music/sonido_prisa_alternativo.mp3");
    Music musicHurry3 = LoadMusicStream("resources/music/sonido_prisa_gigachad.mp3");
    Music musicboss = LoadMusicStream("resources/music/sonido_boss.mp3");
    Music musicboss2 = LoadMusicStream("resources/music/sonido_boss_alternativo.mp3");
    Music musicboss3 = LoadMusicStream("resources/music/sonido_boss_gigachad.mp3");

    bool lastHurry = false;
    bool hurry = false;
    bool lastBoss = false;
    bool boss = false;

    bool mute_music = false;
    bool mute_effect = false;

    float ratio = 0;

    Plataformas() = default;

    Plataformas(std::string ruta_bloque_pequeno, std::string ruta_ubicacion_bloques, float margenSuperior, float margenInferior, int musicN) {
        Inicializador(ruta_bloque_pequeno, ruta_ubicacion_bloques, margenSuperior, margenInferior, musicN);
    }

    void Inicializador(std::string ruta_bloque_pequeno, std::string ruta_ubicacion_bloques, float margenSuperior, float margenInferior, int musicN)
    {
        //Cambiamos el volumen de la musica demasiado alta
        SetMusicVolume(music2, 0.25f);
        SetMusicVolume(music3, 0.5f);
        SetMusicVolume(musicHurry, 0.5f);
        SetMusicVolume(musicHurry2, 0.5f);
        SetMusicVolume(musicboss2, 0.25f);
        SetMusicVolume(musicboss3, 0.15f);
        //Pitch
        SetMusicPitch(musicHurry3, 0.75f);
        //Seleccionar musica
        lastMusicNum = musicN;
        musicNum = musicN;
        if (musicNum == 0) {
            PlayMusicStream(music);
        }
        else if(musicNum == 1){
            PlayMusicStream(music2);
        }
        else {
            PlayMusicStream(music3);
        }

        // Guardar textura 
        this->bloque_pequeno = LoadTexture(ruta_bloque_pequeno.c_str());
        ruta_bloque_pequeno.resize(ruta_bloque_pequeno.length() - 4);
        ruta_bloque_pequeno += "_old.png";
        this->bloque_pequeno_2d = LoadTexture(ruta_bloque_pequeno.c_str());

        // Crear una plataforma por cada linea de ruta_ubicacion_bloques
        this->listaPlataforma = this->leerArchivo(ruta_ubicacion_bloques);

        srcRect = { 0.0f, 0.0f, (float)bloque_pequeno.width, (float)bloque_pequeno.height };
        srcRect_2d = { 0.0f, 0.0f, (float)bloque_pequeno_2d.width, (float)bloque_pequeno_2d.height };

        this->ratioMargenSup = margenSuperior != 0 ? GetScreenHeight() / margenSuperior : 0;
        this->ratioMargenInf = margenInferior != 0 ? GetScreenHeight() / margenInferior : 0;
        this->ratio = (GetScreenHeight() - margenInferior - margenSuperior) / 40;
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
        UnloadMusicStream(music);
        UnloadMusicStream(music2);
        UnloadMusicStream(music3);
        UnloadSound(sound);
    }

    void Unload() {
        UnloadTexture(bloque_pequeno);
        UnloadMusicStream(music);
        UnloadMusicStream(music2);
        UnloadMusicStream(music3);
        UnloadSound(sound);
    }

    void Actualizar() {
        if (!mute_music) {
            if (musicNum != lastMusicNum) {
                if (lastMusicNum == 0) {
                    StopMusicStream(music);
                }
                else if(lastMusicNum == 1){
                    StopMusicStream(music2);
                }
                else {
                    StopMusicStream(music3);
                }
                if (musicNum == 0) {
                    PlayMusicStream(music);
                }
                else if(musicNum == 1){
                    PlayMusicStream(music2);
                }
                else {
                    PlayMusicStream(music3);
                }
                lastMusicNum = musicNum;
            }
            if (musicNum == 0) {
                if (boss) {
                    if (!lastBoss) {
                        if (!lastHurry) {
                            StopMusicStream(music);
                        }
                        else {
                            StopMusicStream(musicHurry);
                        }
                        PlayMusicStream(musicboss);
                    }
                    UpdateMusicStream(musicboss);
                }
                else if (hurry) {
                    if (!lastHurry) {
                        StopMusicStream(music);
                        PlayMusicStream(musicHurry);
                    }
                    UpdateMusicStream(musicHurry);
                }
                else {
                    if (lastHurry) {
                        StopMusicStream(musicHurry);
                        PlayMusicStream(music);
                    }
                    UpdateMusicStream(music);
                }  
            }
            else if(musicNum == 1){
                if (boss) {
                    if (!lastBoss) {
                        if (!lastHurry) {
                            StopMusicStream(music2);
                        }
                        else {
                            StopMusicStream(musicHurry);
                        }
                        PlayMusicStream(musicboss2);
                    }
                    UpdateMusicStream(musicboss2);
                }
                else if (hurry) {
                    if (!lastHurry) {
                        StopMusicStream(music2);
                        PlayMusicStream(musicHurry2);
                    }
                    UpdateMusicStream(musicHurry2);
                }
                else {
                    if (lastHurry) {
                        StopMusicStream(musicHurry2);
                        PlayMusicStream(music2);
                    }
                    UpdateMusicStream(music2);
                }
            }
            else {
                if (boss) {
                    if (!lastBoss) {
                        if (!lastHurry) {
                            StopMusicStream(music3);
                        }
                        else {
                            StopMusicStream(musicHurry3);
                        }
                        PlayMusicStream(musicboss3);
                    }
                    UpdateMusicStream(musicboss3);
                }
                else if (hurry) {
                    if (!lastHurry) {
                        StopMusicStream(music3);
                        PlayMusicStream(musicHurry3);
                    }
                    UpdateMusicStream(musicHurry3);
                }
                else {
                    if (lastHurry) {
                        StopMusicStream(musicHurry3);
                        PlayMusicStream(music3);
                    }
                    UpdateMusicStream(music3);
                }
            }
            lastHurry = hurry;
            lastBoss = boss;
        }
        
        for (int i = 0; i < listaPlataforma.size(); i++) {
            listaPlataforma[i].Actualizar(ratioMargenSup, ratioMargenInf);
        }
        if (empezar_contador) {
            iteraciones++;
            if (iteraciones >= 120 && iteraciones < 300) {
                mostrar_inicio_ronda = true;
            }
            else if (iteraciones >= 300) {
                mostrar_inicio_ronda = false;
                empezar_contador = false;
            }
        }


        if (empezar_contador2) {
            iteraciones2++;
            if ((iteraciones2 % 20) == 0) {
                indice = 1 - indice;
            }
            if (iteraciones2 >= 120) {
                mostrar_hurryup = false;
                empezar_contador2 = false;
            }
        }

    }

    void Dibujar() {
        float tamanoMargenSup = ratioMargenSup != 0 ? GetScreenHeight() / ratioMargenSup : 0;
        float tamanoMargenInf = ratioMargenInf != 0 ? GetScreenHeight() / ratioMargenInf : 0;
        // Destination rectangle (screen rectangle where drawing part of texture)
        float altura_bloque = (float)(GetScreenHeight() - tamanoMargenSup - tamanoMargenInf) / (float)BLOQUE_PEQUENO_ALTO;
        float anchura_bloque = (float)GetScreenWidth() / (float)BLOQUE_PEQUENO_ANCHO;

        if (!cargando_nivel_siguiente) {
            for (int i = 0; i < listaPlataforma.size(); i++) {
                destRect = {
                    listaPlataforma[i].left, // Posicion x de la esquina topleft
                    listaPlataforma[i].top, // Posicion y de la esquina topleft
                    modo_2d ? anchura_bloque : anchura_bloque + anchura_bloque/2.0f,  // anchura bloque
                    modo_2d ? altura_bloque : altura_bloque  + altura_bloque/2.0f // altura bloque
                };
                for (int tamanoX = 0; tamanoX < listaPlataforma[i].tamanoX; tamanoX++) {
                    int bloques_a_dibujar = listaPlataforma[i].tamanoY;
                    // Si es el bloque de mas arriba se ponen 4 bloques mas encima
                    if (listaPlataforma[i].positionY == 1) {
                        bloques_a_dibujar += 4;
                    }
                    for (int tamanoY = 0; tamanoY < bloques_a_dibujar; tamanoY++) {
                        DrawTexturePro(modo_2d ? bloque_pequeno_2d : bloque_pequeno, modo_2d ? srcRect_2d : srcRect, destRect, Vector2{ 0, 0 }, 0.0f, WHITE);
                        destRect.y += altura_bloque;
                    }
                    destRect.y = listaPlataforma[i].top;
                    destRect.x += anchura_bloque;
                }
            }
            if (mostrar_hurryup) {
                float tamanoPantalla = (float)GetScreenHeight() - tamanoMargenSup - tamanoMargenInf;
                float tamano = MeasureText(texto_hurryup.c_str(), 40);
                DrawText(texto_hurryup.c_str(), (GetScreenWidth() - tamano) / 2, tamanoMargenSup + (tamanoPantalla / 2) - 20, GetScreenHeight() /  ratio, colores[indice]);
            }
            DrawRectangle(listaPlataforma[0].left, 0, GetScreenWidth() - listaPlataforma[0].left, tamanoMargenSup, BLACK);

        }
        else {
            float movimiento_por_frame = altura_bloque * (BLOQUE_PEQUENO_ALTO + 6)/ float(FRAMES_CARGAR_SIGUIENTE_NIVEL);
            this->distancia_ya_movida += movimiento_por_frame;
            for (int i = 0; i < listaPlataforma.size(); i++) {
                destRect = {
                    listaPlataforma[i].left, // Posicion x de la esquina topleft
                    listaPlataforma[i].top - distancia_ya_movida, // Posicion y de la esquina topleft
                    modo_2d ? anchura_bloque : anchura_bloque + anchura_bloque / 2,  // anchura bloque
                    modo_2d ? altura_bloque : altura_bloque + altura_bloque / 2  // altura bloque
                };
                for (int tamanoX = 0; tamanoX < listaPlataforma[i].tamanoX; tamanoX++) {
                    int bloques_a_dibujar = listaPlataforma[i].tamanoY;
                    // Si es el bloque de mas abajo se ponen 2 bloques mas debajo
                    if (listaPlataforma[i].positionY == BLOQUE_PEQUENO_ALTO) {
                        bloques_a_dibujar += 2;
                    }
                    if (listaPlataforma[i].positionY == 1) {
                        bloques_a_dibujar += 4;
                    }
                    for (int tamanoY = 0; tamanoY < bloques_a_dibujar; tamanoY++) {
                        DrawTexturePro(modo_2d ? bloque_pequeno_2d : bloque_pequeno, modo_2d ? srcRect_2d : srcRect, destRect, Vector2{ 0, 0 }, 0.0f, WHITE);
                        destRect.y += altura_bloque;
                    }
                    destRect.y = listaPlataforma[i].top - distancia_ya_movida;
                    destRect.x += anchura_bloque;
                }
            }
            for (int i = 0; i < listaPlataformaSiguiente.size(); i++) {
                destRect = {
                    listaPlataformaSiguiente[i].left, // Posicion x de la esquina topleft
                    listaPlataformaSiguiente[i].top + (GetScreenHeight() - tamanoMargenInf - tamanoMargenSup + (altura_bloque * 6)) - distancia_ya_movida, // Posicion y de la esquina topleft
                    modo_2d ? anchura_bloque : anchura_bloque + anchura_bloque / 2,  // anchura bloque
                    modo_2d ? altura_bloque : altura_bloque + altura_bloque / 2  // altura bloque
                };
                for (int tamanoX = 0; tamanoX < listaPlataformaSiguiente[i].tamanoX; tamanoX++) {
                    int bloques_a_dibujar = listaPlataformaSiguiente[i].tamanoY;
                    // Si es el bloque de mas arriba se ponen 4 bloques mas encima
                    if (listaPlataformaSiguiente[i].positionY == 1) {
                        bloques_a_dibujar += 4;
                    }
                    for (int tamanoY = 0; tamanoY < bloques_a_dibujar; tamanoY++) {
                        DrawTexturePro(modo_2d ? bloque_pequeno_siguiente_2d : bloque_pequeno_siguiente, modo_2d ? srcRect_2d : srcRect, destRect, Vector2{ 0, 0 }, 0.0f, WHITE);
                        destRect.y += altura_bloque;
                    }
                    destRect.y = listaPlataformaSiguiente[i].top + (GetScreenHeight() - tamanoMargenInf - tamanoMargenSup + (altura_bloque * 6)) - distancia_ya_movida;
                    destRect.x += anchura_bloque;
                }
            }
            DrawRectangle(listaPlataforma[0].left, 0, GetScreenWidth() - listaPlataforma[0].left, tamanoMargenSup, BLACK);

            // Resetear valores a la normalidad
            if (distancia_ya_movida >= altura_bloque * (BLOQUE_PEQUENO_ALTO + 6)) {
                this->cargando_nivel_siguiente = false;
                this->bloque_pequeno = this->bloque_pequeno_siguiente;
                this->bloque_pequeno_2d = this->bloque_pequeno_siguiente_2d;
                this->distancia_ya_movida = 0;

                this->listaPlataforma.assign(this->listaPlataformaSiguiente.begin(), this->listaPlataformaSiguiente.end());
                this->listaPlataforma.resize(this->listaPlataformaSiguiente.size());
            }
        }
        if (mostrar_inicio_ronda) {
            float tamanoPantalla = (float)GetScreenHeight() - tamanoMargenSup - tamanoMargenInf;
            float tamano = MeasureText(texto_inicio_ronda.c_str(), GetScreenHeight() / ratio);
            DrawText(texto_inicio_ronda.c_str(), (GetScreenWidth() - tamano) / 2, tamanoMargenSup + (tamanoPantalla / 2) - 20, GetScreenHeight() / ratio, WHITE);
        }

        

    }

    void CargarSiguienteNivel(std::string ruta_bloque_pequeno_siguiente, std::string ruta_ubicacion_bloques_siguiente) {
        if (this->cargando_nivel_siguiente == false) {
            this->hurry = false;
            this->cargando_nivel_siguiente = true;
            this->bloque_pequeno_siguiente = LoadTexture(ruta_bloque_pequeno_siguiente.c_str());
            ruta_bloque_pequeno_siguiente.resize(ruta_bloque_pequeno_siguiente.length() - 4);
            ruta_bloque_pequeno_siguiente += "_old.png";
            this->bloque_pequeno_siguiente_2d = LoadTexture(ruta_bloque_pequeno_siguiente.c_str());
            // Crear una plataforma por cada linea de ruta_ubicacion_bloques
            this->listaPlataformaSiguiente = this->leerArchivo(ruta_ubicacion_bloques_siguiente);
            // Se actualiza para poder tener los valores top y bot correctos
            for (int i = 0; i < listaPlataformaSiguiente.size(); i++) {
                listaPlataformaSiguiente[i].Actualizar(ratioMargenSup, ratioMargenInf);
            }
        }
        
    }

    void inicio_de_ronda(unsigned int ronda) {
        if (!empezar_contador) {
            iteraciones = 0;
            if (ronda != 11) {
                texto_inicio_ronda = "ROUND " + std::to_string(ronda) + "\nREADY !";
            }
            else {
                texto_inicio_ronda = "BOSS ROUND\nREADY !";
            }
            
            empezar_contador = true;
        }
    }

    void VolverAlPrimerNivel(std::string ruta_bloque_pequeno_siguiente, std::string ruta_ubicacion_bloques_siguiente) {
        
        this->bloque_pequeno_siguiente = LoadTexture(ruta_bloque_pequeno_siguiente.c_str());
        ruta_bloque_pequeno_siguiente.resize(ruta_bloque_pequeno_siguiente.length() - 4);
        ruta_bloque_pequeno_siguiente += "_old.png";
        this->bloque_pequeno_siguiente_2d = LoadTexture(ruta_bloque_pequeno_siguiente.c_str());
        // Crear una plataforma por cada linea de ruta_ubicacion_bloques
        this->listaPlataformaSiguiente = this->leerArchivo(ruta_ubicacion_bloques_siguiente);
        // Se actualiza para poder tener los valores top y bot correctos
        for (int i = 0; i < listaPlataformaSiguiente.size(); i++) {
            listaPlataformaSiguiente[i].Actualizar(ratioMargenSup, ratioMargenInf);
        }

        this->bloque_pequeno = this->bloque_pequeno_siguiente;
        this->bloque_pequeno_2d = this->bloque_pequeno_siguiente_2d;
        this->distancia_ya_movida = 0;

        this->listaPlataforma.assign(this->listaPlataformaSiguiente.begin(), this->listaPlataformaSiguiente.end());
        this->listaPlataforma.resize(this->listaPlataformaSiguiente.size());
    }

    void SenalHurryUp() {
        if (!empezar_contador2) {
            if (!mute_effect) {
                PlaySound(sound);
            }
            
            iteraciones2 = 0;
            empezar_contador2 = true;
            mostrar_hurryup = true;
            hurry = true;
        }
    }
};

class Columnas {
public:
    Texture2D bloque_grande;
    Texture2D bloque_grande_2d;

    Rectangle srcRect;
    Rectangle srcRect_2d;
    Rectangle destRect;

    float ratioMargenSup = 0;
    float ratioMargenInf = 0;

    float top = 0;
    float bot = 0;
    float left_izq = 0;
    float left_der = 0;
    float right_izq = 0;
    float right_der = 0;

    // Variables para cargar siguiente nivel
    bool cargando_nivel_siguiente = false;
    Texture2D bloque_grande_siguiente;
    Texture2D bloque_grande_siguiente_2d;
    std::string numeroNivelSiguiente = "";
    float distancia_ya_movida = 0;

    std::string numeroNivel = "";

    bool modo_2d = false;


    Columnas() = default; //Debe llamarsse a Inicializador

    Columnas(std::string ruta_bloque_grande, float margenSuperior, float margenInferior, unsigned int numeroNivel) {
        Inicializador(ruta_bloque_grande, margenSuperior, margenInferior, numeroNivel);
    };

    void Inicializador(std::string ruta_bloque_grande, float margenSuperior, float margenInferior, unsigned int numeroNivel)
    {
        this->bloque_grande = LoadTexture(ruta_bloque_grande.c_str());
        ruta_bloque_grande.resize(ruta_bloque_grande.length() - 4);
        ruta_bloque_grande += "_old.png";
        this->bloque_grande_2d = LoadTexture(ruta_bloque_grande.c_str());
        // Source rectangle (part of the texture to use for drawing)
        srcRect = { 0.0f, 0.0f, (float)bloque_grande.width, (float)bloque_grande.height };
        srcRect_2d = { 0.0f, 0.0f, (float)bloque_grande_2d.width, (float)bloque_grande_2d.height };

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
        float tamanoMargenSup = ratioMargenSup != 0 ? GetScreenHeight() / ratioMargenSup : 0;
        float tamanoMargenInf = ratioMargenInf != 0 ? GetScreenHeight() / ratioMargenInf : 0;
        float altura_bloque = (GetScreenHeight() - tamanoMargenSup - tamanoMargenInf) / (float)BLOQUE_GRANDE_ALTO;
        float anchura_bloque = GetScreenWidth() / (float)BLOQUE_GRANDE_ANCHO;

        this->top = tamanoMargenSup - altura_bloque;
        this->bot = tamanoMargenSup + altura_bloque * (BLOQUE_GRANDE_ALTO + 2);

        // Left y right de la columna de la izquierda
        this->left_izq = 0;
        this->right_izq = anchura_bloque;

        // Left y right de la columna de la derecha
        this->left_der = GetScreenWidth() - anchura_bloque;
        this->right_der = GetScreenWidth() - anchura_bloque + anchura_bloque;
    };

    void DibujarIzquierda() {
        // Destination rectangle (screen rectangle where drawing part of texture)
        float tamanoMargenSup = ratioMargenSup != 0 ? GetScreenHeight() / ratioMargenSup : 0;
        float tamanoMargenInf = ratioMargenInf != 0 ? GetScreenHeight() / ratioMargenInf : 0;
        float altura_bloque = (GetScreenHeight() - tamanoMargenSup - tamanoMargenInf) / (float)BLOQUE_GRANDE_ALTO;
        float anchura_bloque = GetScreenWidth() / (float)BLOQUE_GRANDE_ANCHO;

              

        if (!cargando_nivel_siguiente) {
            
            // Columna izquierda
            destRect = { 0, tamanoMargenSup, modo_2d ? anchura_bloque : anchura_bloque + anchura_bloque/ 4, modo_2d ? altura_bloque : altura_bloque + altura_bloque/4 };
            for (int i = 0; i < BLOQUE_GRANDE_ALTO; i++)
            {
                DrawTexturePro(modo_2d ? bloque_grande_2d : bloque_grande, modo_2d ? srcRect_2d : srcRect, destRect, Vector2{ 0, 0 }, 0.0f, WHITE);
                destRect.y += altura_bloque;
            }
            if (stoi(numeroNivel) != 11) {
                // Poner nº de nivel
                float tamano_texto = MeasureText(numeroNivel.c_str(), altura_bloque);
                DrawText(numeroNivel.c_str(), anchura_bloque / 2 - tamano_texto / 2, tamanoMargenSup, altura_bloque, RAYWHITE);
            }
        }
        else {
            float movimiento_por_frame = altura_bloque * (BLOQUE_GRANDE_ALTO + 3) / float(FRAMES_CARGAR_SIGUIENTE_NIVEL);
            this->distancia_ya_movida += movimiento_por_frame;
            // Columna izquierda
            destRect = { 0, tamanoMargenSup - distancia_ya_movida, modo_2d ? anchura_bloque : anchura_bloque + anchura_bloque / 4, modo_2d ? altura_bloque : altura_bloque + altura_bloque / 4 };
            for (int i = 0; i < (BLOQUE_GRANDE_ALTO + 1); i++)
            {
                DrawTexturePro(modo_2d ? bloque_grande_2d : bloque_grande, modo_2d ? srcRect_2d : srcRect, destRect, Vector2{ 0, 0 }, 0.0f, WHITE);
                destRect.y += altura_bloque;
            }
            for (int i = 0; i < (BLOQUE_GRANDE_ALTO + 2); i++)
            {
                DrawTexturePro(modo_2d ? bloque_grande_siguiente_2d : bloque_grande_siguiente, modo_2d ? srcRect_2d : srcRect, destRect, Vector2{ 0, 0 }, 0.0f, WHITE);
                destRect.y += altura_bloque;
            }
            DrawRectangle(0, 0, anchura_bloque, tamanoMargenSup, BLACK);

            if (stoi(numeroNivel) != 11) {
                // Poner nº de nivel
                float tamano_texto = MeasureText(numeroNivel.c_str(), altura_bloque);
                DrawText(numeroNivel.c_str(), anchura_bloque / 2 - tamano_texto / 2, tamanoMargenSup, altura_bloque, RAYWHITE);
            }
            

        }
        
        
    }
    void DibujarDerecha() {
        // Destination rectangle (screen rectangle where drawing part of texture)
        float tamanoMargenSup = ratioMargenSup != 0 ? GetScreenHeight() / ratioMargenSup : 0;
        float tamanoMargenInf = ratioMargenInf != 0 ? GetScreenHeight() / ratioMargenInf : 0;
        float altura_bloque = (GetScreenHeight() - tamanoMargenSup - tamanoMargenInf) / (float)BLOQUE_GRANDE_ALTO;
        float anchura_bloque = GetScreenWidth() / (float)BLOQUE_GRANDE_ANCHO;


        if (!cargando_nivel_siguiente) {

            // Columna derecha
            destRect = { (GetScreenWidth() - anchura_bloque), tamanoMargenSup, modo_2d ? anchura_bloque : anchura_bloque + anchura_bloque / 4, modo_2d ? altura_bloque : altura_bloque + altura_bloque / 4 };
            for (int i = 0; i < BLOQUE_GRANDE_ALTO; i++)
            {
                DrawTexturePro(modo_2d ? bloque_grande_2d : bloque_grande, modo_2d ? srcRect_2d : srcRect, destRect, Vector2{ 0, 0 }, 0.0f, WHITE);
                destRect.y += altura_bloque;
            }

        }
        else {
            float movimiento_por_frame = altura_bloque * (BLOQUE_GRANDE_ALTO + 3) / float(FRAMES_CARGAR_SIGUIENTE_NIVEL);

            // Columna derecha
            destRect = { (GetScreenWidth() - anchura_bloque), tamanoMargenSup - distancia_ya_movida, modo_2d ? anchura_bloque : anchura_bloque + anchura_bloque / 4, modo_2d ? altura_bloque : altura_bloque + altura_bloque / 4 };
            for (int i = 0; i < (BLOQUE_GRANDE_ALTO + 1); i++)
            {
                DrawTexturePro(modo_2d ? bloque_grande_2d : bloque_grande, modo_2d ? srcRect_2d : srcRect, destRect, Vector2{ 0, 0 }, 0.0f, WHITE);
                destRect.y += altura_bloque;
            }
            for (int i = 0; i < (BLOQUE_GRANDE_ALTO + 2); i++)
            {
                DrawTexturePro(modo_2d ? bloque_grande_siguiente_2d : bloque_grande_siguiente, modo_2d ? srcRect_2d : srcRect, destRect, Vector2{ 0, 0 }, 0.0f, WHITE);
                destRect.y += altura_bloque;
            }
            DrawRectangle(GetScreenWidth() - anchura_bloque, 0, GetScreenWidth(), tamanoMargenSup, BLACK);

            // Resetear valores a la normalidad
            if (distancia_ya_movida >= altura_bloque * (BLOQUE_GRANDE_ALTO + 3)) {
                this->cargando_nivel_siguiente = false;
                this->bloque_grande = this->bloque_grande_siguiente;
                this->bloque_grande_2d = this->bloque_grande_siguiente_2d;
                this->distancia_ya_movida = 0;
                this->numeroNivel = this->numeroNivelSiguiente;
            }
        }


    }

    void CargarSiguienteNivel(std::string ruta_bloque_grande_siguiente, unsigned int numeroNivelSiguiente) {
        if (this->cargando_nivel_siguiente == false){
            this->cargando_nivel_siguiente = true;
            this->bloque_grande_siguiente = LoadTexture(ruta_bloque_grande_siguiente.c_str());
            ruta_bloque_grande_siguiente.resize(ruta_bloque_grande_siguiente.length() - 4);
            ruta_bloque_grande_siguiente += "_old.png";
            this->bloque_grande_siguiente_2d = LoadTexture(ruta_bloque_grande_siguiente.c_str());
            this->numeroNivelSiguiente = std::to_string(numeroNivelSiguiente);
        }
    }

    void VolverAlPrimerNivel(std::string ruta_bloque_grande_siguiente, unsigned int numeroNivelSiguiente) {
        this->bloque_grande_siguiente = LoadTexture(ruta_bloque_grande_siguiente.c_str());
        ruta_bloque_grande_siguiente.resize(ruta_bloque_grande_siguiente.length() - 4);
        ruta_bloque_grande_siguiente += "_old.png";
        this->bloque_grande_siguiente_2d = LoadTexture(ruta_bloque_grande_siguiente.c_str());
        this->numeroNivelSiguiente = std::to_string(numeroNivelSiguiente);

        this->bloque_grande = this->bloque_grande_siguiente;
        this->bloque_grande_2d = this->bloque_grande_siguiente_2d;
        this->distancia_ya_movida = 0;
        this->numeroNivel = this->numeroNivelSiguiente;
    }

    
};


class ContadorVidas {
public:

    Texture2D imagen_vida_p1;
    Texture2D imagen_vida_p2;

    unsigned int vidas_p1 = 0;
    unsigned int vidas_p2 = 0;

    Rectangle srcRect;
    Rectangle destRect;

    bool hayP2 = false;
    bool hayCreditos = false;

    Rectangle srcRect_push2P_sprites;
    Texture2D push_2P_sprites[2] = {
        LoadTexture("resources/Push2P/Push2P_1.png"),
        LoadTexture("resources/Push2P/Push2P_2.png")
    };

    // Cambiar esta variable mientras dure el cambio de nivel para que no se vea el sprite de PUSH_2P
    bool cargando_siguiente_nivel = false;

    unsigned int frecuencia = 45;
    unsigned int iteraciones = 0;
    unsigned int indice = 0;

    float ratioMargenSup = 0;
    float ratioMargenInf = 0;

    ContadorVidas() = default;

    ContadorVidas(std::string ruta_imagen_vida_p1, std::string ruta_imagen_vida_p2, float margenSuperior, float margenInferior) {
        Inicializador(ruta_imagen_vida_p1, ruta_imagen_vida_p2, margenSuperior, margenInferior);
    }

    void Inicializador(std::string ruta_imagen_vida_p1, std::string ruta_imagen_vida_p2, float margenSuperior, float margenInferior)
    {
        // Guardar imagenes
        this->imagen_vida_p1 = LoadTexture(ruta_imagen_vida_p1.c_str());
        this->imagen_vida_p2 = LoadTexture(ruta_imagen_vida_p2.c_str());

        srcRect = { 0.0f, 0.0f, (float)imagen_vida_p1.width, (float)imagen_vida_p1.height };
        srcRect_push2P_sprites = { 0.0f, 0.0f, (float)push_2P_sprites[0].width, (float)push_2P_sprites[0].height };

        this->ratioMargenSup = margenSuperior != 0 ? GetScreenHeight() / margenSuperior : 0;
        this->ratioMargenInf = margenInferior != 0 ? GetScreenHeight() / margenInferior : 0;
    }

    ~ContadorVidas() {
        UnloadTexture(imagen_vida_p1);
        UnloadTexture(imagen_vida_p2);
    }

    void Unload() {
        UnloadTexture(imagen_vida_p1);
        UnloadTexture(imagen_vida_p2);
    };

    void Actualizar(unsigned int vidas_p1, unsigned int vidas_p2, unsigned int creditos) {
        this->vidas_p1 = vidas_p1;
        this->vidas_p2 = vidas_p2;

        float tamanoMargenSup = ratioMargenSup != 0 ? GetScreenHeight() / ratioMargenSup : 0;
        float tamanoMargenInf = ratioMargenInf != 0 ? GetScreenHeight() / ratioMargenInf : 0;
        float anchura = GetScreenWidth() / (float)BLOQUE_PEQUENO_ANCHO;
        float altura = (GetScreenHeight() - tamanoMargenSup - tamanoMargenInf) / (float)BLOQUE_PEQUENO_ALTO;

        if (!hayP2) {
            if (creditos >= 1) {
                hayCreditos = true;
            }
            else {
                hayCreditos = false;
            }
        }
        

    };

    void Dibujar() {
        float tamanoMargenSup = ratioMargenSup != 0 ? GetScreenHeight() / ratioMargenSup : 0;
        float tamanoMargenInf = ratioMargenInf != 0 ? GetScreenHeight() / ratioMargenInf : 0;
        // Destination rectangle (screen rectangle where drawing part of texture)
        float altura_bloque = (GetScreenHeight() - tamanoMargenSup - tamanoMargenInf) / (float)BLOQUE_PEQUENO_ALTO;
        float anchura_bloque = GetScreenWidth() / (float)BLOQUE_PEQUENO_ANCHO;

        // Vidas p1
        destRect = {
            0, // Posicion x de la esquina topleft
            GetScreenHeight() - tamanoMargenInf - altura_bloque, // Posicion y de la esquina topleft
            anchura_bloque,  // anchura bloque
            altura_bloque // altura bloque
        };
        for (int i = 0; i < this->vidas_p1; i++) {
            DrawTexturePro(imagen_vida_p1, srcRect, destRect, Vector2{ 0, 0 }, 0.0f, WHITE);
            destRect.x += anchura_bloque;
        }

        // Vidas p2
        if (hayP2) { // Si hay p2 se dibujan las vidas
            destRect = {
                GetScreenWidth() - anchura_bloque, // Posicion x de la esquina topleft
                GetScreenHeight() - tamanoMargenInf - altura_bloque, // Posicion y de la esquina topleft
                anchura_bloque,  // anchura bloque
                altura_bloque // altura bloque
            };
            for (int i = 0; i < this->vidas_p2; i++) {
                DrawTexturePro(imagen_vida_p2, srcRect, destRect, Vector2{ 0, 0 }, 0.0f, WHITE);
                destRect.x -= anchura_bloque;
            }
        }
        else if (!cargando_siguiente_nivel && hayCreditos) { // Si no hay p2 y hay creditos se muestra el sprite PUSH_2P
            iteraciones++;
            if (iteraciones == frecuencia) {
                indice = (indice + 1) % 2;
                iteraciones = 0;
            }
            destRect = {
                GetScreenWidth() - anchura_bloque * 7,
                GetScreenHeight() - tamanoMargenInf - altura_bloque * 5,
                anchura_bloque * 4,
                altura_bloque * 4
            };
            DrawTexturePro(push_2P_sprites[indice], srcRect_push2P_sprites, destRect, Vector2{0, 0}, 0.0f, WHITE);
        }
        else if (cargando_siguiente_nivel) { // Si se está cargando el siguiente nivel 
            iteraciones++;
            if (iteraciones == FRAMES_CARGAR_SIGUIENTE_NIVEL) {
                iteraciones = 0;
                cargando_siguiente_nivel = false;
            }
        }
        
    }

    void cargar_siguiente_nivel() {
        cargando_siguiente_nivel = true;
        iteraciones = 0;
    }
};