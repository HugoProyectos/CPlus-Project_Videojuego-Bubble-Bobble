#pragma once
#include "raylib.h"
#include <iostream>
#include "ini.h"

#define NUM_CONTROLS 11



class Controls{
public:
    unsigned int left_p1 = 0;
    unsigned int right_p1 = 0;
    unsigned int jump_p1 = 0;
    unsigned int spit_p1 = 0;
    unsigned int left_p2 = 0;
    unsigned int right_p2 = 0;
    unsigned int jump_p2 = 0;
    unsigned int spit_p2 = 0;
    unsigned int coin = 0;
    unsigned int play1 = 0;
    unsigned int play2 = 0;
    
    Texture2D fondo = LoadTexture("resources/main_menu/controls.png");
    Rectangle srcRect;
    Rectangle dstRect;

    Sound rollover = LoadSound("resources/music/rollover.mp3");
    Sound clic = LoadSound("resources/music/clic.mp3");

    int selected_key = -1;

    const char* keys_names[NUM_CONTROLS] = {
            "Izquierda P1", "Derecha P1", "Saltar P1", "Escupir P1",
            "Izquierda P2", "Derecha P2", "Saltar P2", "Escupir P2",
            "Insertar moneda", "Jugar 1P", "Jugar 2P"
    };

    // Controles provisionales para mostrarlos por pantalla
    unsigned int keys[NUM_CONTROLS] = {
            this->left_p1, this->right_p1, this->jump_p1, this->spit_p1, 
            this->left_p2, this->right_p2, this->jump_p2, this->spit_p2,
            this->coin, this->play1, this->play2
    };

    const char* resoluciones1_names[6] = {
        "800x450", "800x600", "900x600", "1024x768", "1280x720",
        "1920x1080"
    };
    const unsigned int resoluciones1_width[6] = {
        800, 800, 900, 1024, 1280, 1920
    };
    const unsigned int resoluciones1_height[6] = {
        450, 600, 600, 768, 720, 1080
    };

    const char* resoluciones2_names[3] = {
        "Windowed", "Full-screen", "Full-windowed",
    };

    

    unsigned int resoluciones2_mode = 0;
    unsigned int resoluciones1_mode = 0;
    bool resoluciones1_pulsado = false;
    bool resoluciones2_pulsado = false;

    const char* IA_MODO[2] = {
        "Original", "Dificil"
    };
    unsigned int IA_mode = 0;
    bool IA_pulsado = false;

    
    unsigned int skin_modo = 0;
    bool skin_pulsado = false;
    const char* SKIN_MODE[2] = {
        "OFF", "ON"
    };

    unsigned int mapa_modo = 0;
    bool mapa_pulsado = false;
    const char* TIPO_MAPA[2] = {
        "2.5D", "2D"
    };

    unsigned int music_modo = 0;
    bool music_pulsado = false;
    const char* MUSIC_MODE[2] = {
        "ON", "OFF"
    };

    unsigned int alt_music = 0;
    bool alt_music_pulsado = false;
    const char* ALT_MUSIC_MODE[3] = {
        "OFF", "ON", "ON"
    };

    unsigned int volumen = 0;
    bool volumen_plus_pulsado = false;
    bool volumen_minus_pulsado = false;
    const char* VOLUMEN_MODE[11] = {
        "0","1","2","3","4","5","6","7","8","9","10"
    };

    unsigned int effect_modo = 0;
    bool effect_pulsado = false;
    const char* EFFECT_MODE[2] = {
        "ON", "OFF"
    };

    unsigned int game_modo = 0;
    bool game_pulsado = false;
    const char* GAME_MODE[2] = {
        "COOPERATIVE", "P V IA (BOT)"
    };

    bool soundPlaying = false;

    void playSound(Sound soundToPlay)
    {
        if (!soundPlaying && effect_modo == 0)
        {
            PlaySound(soundToPlay);
            soundPlaying = true;
        }
    }

    Controls() = default;

    Controls(std::string ruta_fichero_configuracion) {
        Inicializador(ruta_fichero_configuracion);
    }

    void Inicializador(std::string ruta_fichero_configuracion)
    {
        cargarControles(ruta_fichero_configuracion);
        srcRect = { 0,0, (float)fondo.width, (float)fondo.height };
    }

    ~Controls() {
        UnloadSound(rollover);
        UnloadSound(clic);
    }

    void Unload() {
        UnloadSound(rollover);
        UnloadSound(clic);
    }

    void actualizarControles() {
        this->left_p1 = keys[0];
        this->right_p1 = keys[1];
        this->jump_p1 = keys[2];
        this->spit_p1 = keys[3];
        this->left_p2 = keys[4];
        this->right_p2 = keys[5];
        this->jump_p2 = keys[6];
        this->spit_p2 = keys[7];
        this->coin = keys[8];
        this->play1 = keys[9];
        this->play2 = keys[10];
    }

    void actualizarControlesProvisionales() {
        keys[0] = this->left_p1;
        keys[1] = this->right_p1;
        keys[2] = this->jump_p1;
        keys[3] = this->spit_p1;
        keys[4] = this->left_p2;
        keys[5] = this->right_p2;
        keys[6] = this->jump_p2;
        keys[7] = this->spit_p2;
        keys[8] = this->coin;
        keys[9] = this->play1;
        keys[10] = this->play2;
    }

    void cargarControles(std::string ruta_fichero_configuracion) {
        // first, create a file instance
        mINI::INIFile file(ruta_fichero_configuracion);

        // next, create a structure that will hold data
        mINI::INIStructure ini;

        // now we can read the file
        file.read(ini);

        this->left_p1 = stoi(ini["P1"]["left"]);
        this->right_p1 = stoi(ini["P1"]["right"]);
        this->jump_p1 = stoi(ini["P1"]["jump"]);
        this->spit_p1 = stoi(ini["P1"]["spit"]);
        this->left_p2 = stoi(ini["P2"]["left"]);
        this->right_p2 = stoi(ini["P2"]["right"]);
        this->jump_p2 = stoi(ini["P2"]["jump"]);
        this->spit_p2 = stoi(ini["P2"]["spit"]);
        this->coin = stoi(ini["global"]["coin"]);
        this->play1 = stoi(ini["global"]["play1"]);
        this->play2 = stoi(ini["global"]["play2"]);
        this->IA_mode = stoi(ini["global"]["IA_mode"]);
        this->skin_modo  = stoi(ini["global"]["alt_skins"]);
        this->mapa_modo = stoi(ini["global"]["map_2d"]);
        this->resoluciones2_mode = stoi(ini["screen"]["screen_mode"]);
        this->resoluciones1_mode = stoi(ini["screen"]["resolution"]);
        this->music_modo = stoi(ini["sound"]["mute_music"]);
        this->alt_music = stoi(ini["sound"]["alt_music"]);
        this->volumen = stoi(ini["sound"]["volumen"]);
        this->effect_modo = stoi(ini["sound"]["mute_effects"]);
        if ((alt_music == 1) && (skin_modo == 1)) {
            alt_music = 2;
        }
        SetMasterVolume((float)volumen / 10);
        this->game_modo = stoi(ini["global"]["Game_mode"]);
    }

    void guardarControlesNuevos() {
        // create a file instance
        mINI::INIFile file("config.ini");

        // create a data structure
        mINI::INIStructure ini;
        file.read(ini);

        // populate the structure
        ini["P1"]["left"] = std::to_string(this->left_p1);
        ini["P1"]["right"] = std::to_string(this->right_p1);
        ini["P1"]["jump"] = std::to_string(this->jump_p1);
        ini["P1"]["spit"] = std::to_string(this->spit_p1);

        ini["P2"]["left"] = std::to_string(this->left_p2);
        ini["P2"]["right"] = std::to_string(this->right_p2);
        ini["P2"]["jump"] = std::to_string(this->jump_p2);
        ini["P2"]["spit"] = std::to_string(this->spit_p2);

        ini["global"]["play1"] = std::to_string(this->play1);
        ini["global"]["play2"] = std::to_string(this->play2);
        ini["global"]["coin"] = std::to_string(this->coin);
        ini["global"]["IA_mode"] = std::to_string(this->IA_mode);
        ini["global"]["alt_skins"] = std::to_string(this->skin_modo);
        ini["global"]["map_2d"] = std::to_string(this->mapa_modo);
        
        ini["screen"]["screen_mode"] = std::to_string(this->resoluciones2_mode);
        ini["screen"]["resolution"] = std::to_string(this->resoluciones1_mode);

        ini["sound"]["mute_music"] = std::to_string(this->music_modo);
        ini["sound"]["alt_music"] = std::to_string(this->alt_music);
        ini["sound"]["volumen"] = std::to_string(this->volumen);
        ini["sound"]["mute_effects"] = std::to_string(this->effect_modo);

        ini["global"]["Game_mode"] = std::to_string(this->game_modo);

        // generate an INI file (overwrites any previous file)
        file.write(ini);
    }

    void Actualizar(int & IA_MODE, int  &SKIN_MODE, int & MAPA_MODE, int &MUSIC_OUT, int &ALT_MUSIC_OUT ,int & EFFECT_OUT, int & GAME_OUT) {
        this->actualizarControlesProvisionales();

        /*if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            resoluciones1_pulsado = false;
            resoluciones2_pulsado = false;
        }*/
        unsigned int positionX;
        unsigned int positionY;
        std::string texto = "";
        int tamano_texto = 0;
        Rectangle rectAux;
        Vector2 mousePoint;
        selected_key = -1;
        for (int i = 0; i < NUM_CONTROLS; i++) {
            if (i < 4) {
                positionX = 20;
                positionY = 20 + 30 * i;
            }
            else if (i < 8) {
                positionX = GetScreenWidth() / 2;
                positionY = 20 + 30 * (i - 4);
            }
            else {
                positionX = 20 + (GetScreenWidth() / 3) * (i - 8);
                positionY = 20 + 30 * (4 + 1);
            }
            // Calculo el tamaño para hacer el rectangulo a medida
            texto = TextFormat("%s: %s", keys_names[i], this->GetKeyName(keys[i]).c_str());
            tamano_texto = MeasureText(texto.c_str(), 20);

            // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
            rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
            //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
            mousePoint = GetMousePosition();
            
            // Si ponemos el cursor encima
            if (CheckCollisionPointRec(mousePoint, rectAux)) {
                selected_key = i;
                this->playSound(rollover);
            }
        }

        // Si se ha seleccionado una funcionalidad, pedir al usuario que presione una tecla para asignarla
        if (selected_key >= 0) {
            int keyPressed = GetKeyPressed();
            if (keyPressed != 0 && keyPressed != KEY_ENTER && keyPressed != KEY_BACKSPACE) {
                keys[selected_key] = keyPressed;
                this->actualizarControles();
                selected_key = -1;
            }
        }
        else {
            soundPlaying = false;
            int keyPressed = GetKeyPressed();
            if (keyPressed == KEY_ENTER) {
                this->guardarControlesNuevos();
            }
            else if (keyPressed == KEY_BACKSPACE) {
                this->cargarControles("config_default.ini");
            }
        }

        // Valores resolucion
        positionX = 20;
        positionY = 20 + 30 * (4 + 2);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("Resolution: %s", this->resoluciones1_names[resoluciones1_mode]);
        tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && resoluciones2_mode == 0 && resoluciones1_pulsado == false) {
                if (effect_modo == 0) {
                    PlaySound(clic);
                }
                resoluciones1_mode = (resoluciones1_mode + 1) % 6;
                SetWindowSize(resoluciones1_width[resoluciones1_mode], resoluciones1_height[resoluciones1_mode]);
                resoluciones1_pulsado = true;
            }
            else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && resoluciones2_mode == 0 && resoluciones1_pulsado == false) {
                if (effect_modo == 0) {
                    PlaySound(clic);
                }
                resoluciones1_mode = (resoluciones1_mode + 6 - 1) % 6;
                SetWindowSize(resoluciones1_width[resoluciones1_mode], resoluciones1_height[resoluciones1_mode]);
                resoluciones1_pulsado = true;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) || IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
                resoluciones1_pulsado = false;
            }
        }


        // Modos ventana
        positionX = GetScreenWidth() / 2;
        positionY = 20 + 30 * (4 + 2);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("Window mode: %s", this->resoluciones2_names[resoluciones2_mode]);
        tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && resoluciones2_pulsado == false) {
                if (effect_modo == 0) {
                    PlaySound(clic);
                }
                resoluciones2_mode = (resoluciones2_mode + 1) % 3;
                if (resoluciones2_mode == 1) {
                    SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
                    ToggleFullscreen();
                }
                else if (resoluciones2_mode == 2) {
                    ToggleFullscreen();
                    SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
                }
                else if (resoluciones2_mode == 0) {
                    SetWindowSize(resoluciones1_width[resoluciones1_mode], resoluciones1_height[resoluciones1_mode]);
                }
                resoluciones2_pulsado = true;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                resoluciones2_pulsado = false;
            }
        }

        // Modos IA
        positionX = 20;
        positionY = 20 + 30 * (4 + 3);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("IA mode: %s", this->IA_MODO[IA_mode]);
        tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IA_pulsado == false) {
                if (effect_modo == 0) {
                    PlaySound(clic);
                }
                IA_mode = (IA_mode + 1) % 2;
                IA_pulsado = true;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                IA_pulsado = false;
            }
        }
        IA_MODE = IA_mode;


        // Modos SKIN
        positionX = 20 + (GetScreenWidth() / 3);
        positionY = 20 + 30 * (4 + 3);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("Alternative skins: %s", this->SKIN_MODE[skin_modo]);
        tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && skin_pulsado == false) {
                if (effect_modo == 0) {
                    PlaySound(clic);
                }
                skin_modo = (skin_modo + 1) % 2;
                skin_pulsado = true;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                skin_pulsado = false;
            }
        }
        SKIN_MODE = skin_modo;

        // Modos MAPA
        positionX = 20 + (GetScreenWidth() / 3) * 2;
        positionY = 20 + 30 * (4 + 3);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("Tipo de mapa: %s", this->TIPO_MAPA[mapa_modo]);
        tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && mapa_pulsado == false) {
                if (effect_modo == 0) {
                    PlaySound(clic);
                }
                mapa_modo = (mapa_modo + 1) % 2;
                mapa_pulsado = true;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                mapa_pulsado = false;
            }
        }
        MAPA_MODE = mapa_modo;


        // Modos MUSIC
        positionX = 20;
        positionY = 20 + 30 * (4 + 4);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("Musica: %s", this->MUSIC_MODE[music_modo]);
        tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && music_pulsado == false) {
                if (effect_modo == 0) {
                    PlaySound(clic);
                }
                music_modo = (music_modo + 1) % 2;
                music_pulsado = true;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                music_pulsado = false;
            }
        }
        MUSIC_OUT = music_modo;

        // Modos ALT_MUSIC
        positionX = 20;
        positionY = 20 + 30 * (4 + 5);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("Musica alternativa: %s", this->ALT_MUSIC_MODE[alt_music]);
        tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && alt_music_pulsado == false) {
                if (effect_modo == 0) {
                    PlaySound(clic);
                }
                if (alt_music == 0) {
                    if (skin_modo == 1) {
                        alt_music = 2;
                    }
                    else {
                        alt_music = 1;
                    }
                }
                else {
                    alt_music = 0;
                }
                alt_music_pulsado = true;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                alt_music_pulsado = false;
            }
        }
        ALT_MUSIC_OUT = alt_music;

        // VOLUMEN
        positionX = 20;
        positionY = 20 + 30 * (4 + 6);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("Volumen: %s0 %s", VOLUMEN_MODE[volumen], "%");
        tamano_texto = MeasureText(texto.c_str(), 20);
        positionX = positionX + tamano_texto;

        //BOTON DE MENOS DEL VOLUMEN
        texto = TextFormat("-");
        tamano_texto = MeasureText(texto.c_str(), 20);
        positionX = positionX + tamano_texto;
        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && volumen_minus_pulsado == false) {
                if (volumen > 0) {
                    volumen--;
                    SetMasterVolume((float)volumen/10);
                }
                volumen_minus_pulsado = true;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                volumen_minus_pulsado = false;
            }
        }

        //BOTON DE MAS DEL VOLUMEN
        positionX += 2*tamano_texto;
        texto = TextFormat("+");
        tamano_texto = MeasureText(texto.c_str(), 20);
        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && volumen_plus_pulsado == false) {
                if (volumen < 10) {
                    volumen++;
                    SetMasterVolume((float)volumen / 10);
                }
                volumen_plus_pulsado = true;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                volumen_plus_pulsado = false;
            }
        }

        // Modos EFFECT
        positionX = 20 + (GetScreenWidth() / 2);
        positionY = 20 + 30 * (4 + 4);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("Efectos de sonido: %s", this->EFFECT_MODE[effect_modo]);
        tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && effect_pulsado == false) {
                effect_modo = (effect_modo + 1) % 2;
                if (effect_modo == 0) {
                    PlaySound(clic);
                }
                
                effect_pulsado = true;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                effect_pulsado = false;
            }
        }
        EFFECT_OUT = effect_modo;

        // Modos GAME
        positionX = 20 + (GetScreenWidth() / 2);
        positionY = 20 + 30 * (4 + 5);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("Modo de juego: %s", this->GAME_MODE[game_modo]);
        tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && game_pulsado == false) {
                game_modo = (game_modo + 1) % 2;
                if (effect_modo == 0) {
                    PlaySound(clic);
                }

                game_pulsado = true;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                game_pulsado = false;
            }
        }
        GAME_OUT = game_modo;

    }

    void Dibujar() {

        ClearBackground(BLACK);

        dstRect = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
        DrawTexturePro(fondo, srcRect, dstRect, Vector2{ 0,0 }, 0, WHITE);

        unsigned int positionX;
        unsigned int positionY;
        Color color;
        std::string texto = "";
        int tamano_texto = 0;
        Rectangle rectAux;
        Vector2 mousePoint;
        for (int i = 0; i < NUM_CONTROLS; i++) {
            
            if (i < 4) {
                positionX = 20;
                positionY = 20 + 30 * i;
                color = GREEN;
            }
            else if (i < 8) {
                positionX = GetScreenWidth() / 2;
                positionY = 20 + 30 * (i - 4);
                color = BLUE;
            }
            else {
                positionX = 20 + (GetScreenWidth() / 3) * (i - 8);
                positionY = 20 + 30 * (4 + 1);
                color = YELLOW;
            }
            // Calculo el tamaño para hacer el rectangulo a medida
            texto = TextFormat("%s: %s", keys_names[i], this->GetKeyName(keys[i]).c_str());
            tamano_texto = MeasureText(texto.c_str(), 20);

            // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
            rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
            //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
            mousePoint = GetMousePosition();

            if (i >= 8) {
                DrawRectangle(positionX - 3, positionY, tamano_texto + 6, 20+ 6, BLACK);
            }

            // Si ponemos el cursor encima
            if (CheckCollisionPointRec(mousePoint, rectAux)) {
                // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
                DrawText(texto.c_str(), positionX, positionY, 20, RED);
            }
            else {
                // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
                DrawText(texto.c_str(), positionX, positionY, 20, color);
            }


        }

        // Valores resolucion
        positionX = 20;
        positionY = 20 + 30 * (4 + 2);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("Resolution: %s", this->resoluciones1_names[resoluciones1_mode]);
        tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, RED);
        }
        else {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, WHITE);
        }


        // Modos ventana
        positionX = GetScreenWidth() / 2;
        positionY = 20 + 30 * (4 + 2);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("Window mode: %s", this->resoluciones2_names[resoluciones2_mode]);
        tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, RED);
        }
        else {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, WHITE);
        }

        // Modo IA
        positionX = 20;
        positionY = 20 + 30 * (4 + 3);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("IA mode: %s", this->IA_MODO[IA_mode]);
        tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, RED);
        }
        else {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, WHITE);
        }

        // Modo SKINS
        positionX = 20 + GetScreenWidth()/3;
        positionY = 20 + 30 * (4 + 3);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("Alternative skins: %s", this->SKIN_MODE[skin_modo]);
        tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, RED);
        }
        else {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, WHITE);
        }

        // Modo MAPA
        positionX = 20 + (GetScreenWidth() / 3) * 2;
        positionY = 20 + 30 * (4 + 3);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("Tipo de mapa: %s", this->TIPO_MAPA[mapa_modo]);
        tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, RED);
        }
        else {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, WHITE);
        }


        // Modo MUSIC
        positionX = 20 ;
        positionY = 20 + 30 * (4 + 4);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("Musica: %s", this->MUSIC_MODE[music_modo]);
        tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, RED);
        }
        else {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, WHITE);
        }

        // Modo ALT_MUSIC
        positionX = 20;
        positionY = 20 + 30 * (4 + 5);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("Musica alternativa: %s", this->ALT_MUSIC_MODE[alt_music]);
        tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, RED);
        }
        else {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, WHITE);
        }

        // VOLUMEN
        positionX = 20;
        positionY = 20 + 30 * (4 + 6);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("Volumen: %s0 %s", VOLUMEN_MODE[volumen],"%");
        tamano_texto = MeasureText(texto.c_str(), 20);
        DrawText(texto.c_str(), positionX, positionY, 20, WHITE);
        positionX = positionX + tamano_texto;

        //BOTON DE MENOS DEL VOLUMEN
        texto = TextFormat("-");
        tamano_texto = MeasureText(texto.c_str(), 20);
        positionX = positionX + tamano_texto;
        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
                // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, RED);
        }
        else {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, WHITE);
        }

        //BOTON DE MAS DEL VOLUMEN
        positionX += 2*tamano_texto;
        texto = TextFormat("+");
        tamano_texto = MeasureText(texto.c_str(), 20);
        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
                // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, RED);
        }
        else {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, WHITE);
        }

        // Modo EFFECT
        positionX = 20 + (GetScreenWidth() / 2) ;
        positionY = 20 + 30 * (4 + 4);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("Efectos de sonido: %s", this->EFFECT_MODE[effect_modo]);
        tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, RED);
        }
        else {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, WHITE);
        }

        // Modo GAME
        positionX = 20 + (GetScreenWidth() / 2);
        positionY = 20 + 30 * (4 + 5);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("Modo de juego: %s", this->GAME_MODE[game_modo]);
        tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 20 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, RED);
        }
        else {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, WHITE);
        }

        // Si se ha seleccionado una funcionalidad, pedir al usuario que presione una tecla para asignarla
        if (selected_key >= 0) {
            DrawText(TextFormat("Pulse una tecla para asignarla a la funcionalidad %s", keys_names[selected_key]), 20, GetScreenHeight() - 30, 20, WHITE);
        }
        else {
            DrawText("Pulse ENTER para guardar los controles y empezar a jugar", 20, GetScreenHeight() - 60, 20, WHITE);
            DrawText("Pulse BACKSPACE para volver a los controles predeterminados", 20, GetScreenHeight() - 60 + 30, 20, WHITE);
        }
    }

    std::string GetKeyName(unsigned int numero) {
        switch (numero) {
        case 39: return "'";
        case 44: return ",";
        case 45: return "-";
        case 46: return ".";
        case 47: return "/";
        case 48: return "0";
        case 49: return "1";
        case 50: return "2";
        case 51: return "3";
        case 52: return "4";
        case 53: return "5";
        case 54: return "6";
        case 55: return "7";
        case 56: return "8";
        case 57: return "9";
        case 59: return ";";
        case 61: return "=";
        case 65: return "A";
        case 66: return "B";
        case 67: return "C";
        case 68: return "D";
        case 69: return "E";
        case 70: return "F";
        case 71: return "G";
        case 72: return "H";
        case 73: return "I";
        case 74: return "J";
        case 75: return "K";
        case 76: return "L";
        case 77: return "M";
        case 78: return "N";
        case 79: return "O";
        case 80: return "P";
        case 81: return "Q";
        case 82: return "R";
        case 83: return "S";
        case 84: return "T";
        case 85: return "U";
        case 86: return "V";
        case 87: return "W";
        case 88: return "X";
        case 89: return "Y";
        case 90: return "Z";
        case 91: return "[";
        case 92: return "'\'";
        case 93: return "]";
        case 96: return "`";
        case 32: return "Space";
        case 256: return "Esc";
        case 257: return "Enter";
        case 258: return "Tab";
        case 259: return "Backspace";
        case 260: return "Ins";
        case 261: return "Del";
        case 262: return "Cursor right";
        case 263: return "Cursor left";
        case 264: return "Cursor down";
        case 265: return "Cursor up";
        case 266: return "Page up";
        case 267: return "Page down";
        case 268: return "Home";
        case 269: return "End";
        case 280: return "Caps lock";
        case 281: return "Scroll down";
        case 282: return "Num lock";
        case 283: return "Print screen";
        case 284: return "Pause";
        case 290: return "F1";
        case 291: return "F2";
        case 292: return "F3";
        case 293: return "F4";
        case 294: return "F5";
        case 295: return "F6";
        case 296: return "F7";
        case 297: return "F8";
        case 298: return "F9";
        case 299: return "F10";
        case 300: return "F11";
        case 301: return "F12";
        case 340: return "Shift left";
        case 341: return "Control left";
        case 342: return "Alt left";
        case 343: return "Super left";
        case 344: return "Shift right";
        case 345: return "Control right";
        case 346: return "Alt right";
        case 347: return "Super right";
        case 348: return "KB menu";
        case 320: return "Keypad 0";
        case 321: return "Keypad 1";
        case 322: return "Keypad 2";
        case 323: return "Keypad 3";
        case 324: return "Keypad 4";
        case 325: return "Keypad 5";
        case 326: return "Keypad 6";
        case 327: return "Keypad 7";
        case 328: return "Keypad 8";
        case 329: return "Keypad 9";
        case 330: return "Keypad .";
        case 331: return "Keypad /";
        case 332: return "Keypad *";
        case 333: return "Keypad -";
        case 334: return "Keypad +";
        case 335: return "Keypad Enter";
        case 336: return "Keypad =";
        default: return "Undefined";
        }
    }

};