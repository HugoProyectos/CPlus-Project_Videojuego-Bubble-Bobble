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


    Controls() = default;

    Controls(std::string ruta_fichero_configuracion) {
        Inicializador(ruta_fichero_configuracion);
    }

    void Inicializador(std::string ruta_fichero_configuracion)
    {
        cargarControles(ruta_fichero_configuracion);
    }

    ~Controls() {}

    void Unload() {}

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
    }

    void guardarControlesNuevos() {
        // create a file instance
        mINI::INIFile file("config.ini");

        // create a data structure
        mINI::INIStructure ini;

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

        // generate an INI file (overwrites any previous file)
        file.generate(ini);
    }

    void Actualizar() {
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
            rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 30 };
            //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
            mousePoint = GetMousePosition();
            
            // Si ponemos el cursor encima
            if (CheckCollisionPointRec(mousePoint, rectAux)) {
                selected_key = i;
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
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 30 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && resoluciones2_mode == 0 && resoluciones1_pulsado == false) {
                resoluciones1_mode = (resoluciones1_mode + 1) % 6;
                SetWindowSize(resoluciones1_width[resoluciones1_mode], resoluciones1_height[resoluciones1_mode]);
                resoluciones1_pulsado = true;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
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
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 30 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && resoluciones2_pulsado == false) {
                resoluciones2_mode = (resoluciones2_mode + 1) % 3;
                if (resoluciones2_mode == 1) {
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
        
        
    }

    void Dibujar() {

        ClearBackground(RAYWHITE);
        unsigned int positionX;
        unsigned int positionY;
        std::string texto = "";
        int tamano_texto = 0;
        Rectangle rectAux;
        Vector2 mousePoint;
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
            rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 30 };
            //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
            mousePoint = GetMousePosition();

            // Si ponemos el cursor encima
            if (CheckCollisionPointRec(mousePoint, rectAux)) {
                // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
                DrawText(texto.c_str(), positionX, positionY, 20, RED);
            }
            else {
                // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
                DrawText(texto.c_str(), positionX, positionY, 20, BLACK);
            }


        }

        // Valores resolucion
        positionX = 20;
        positionY = 20 + 30 * (4 + 2);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("Resolution: %s", this->resoluciones1_names[resoluciones1_mode]);
        tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 30 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, RED);
        }
        else {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, BLACK);
        }


        // Modos ventana
        positionX = GetScreenWidth() / 2;
        positionY = 20 + 30 * (4 + 2);
        // Calculo el tamaño para hacer el rectangulo a medida
        texto = TextFormat("Window mode: %s", this->resoluciones2_names[resoluciones2_mode]);
        tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        rectAux = { (float)positionX, (float)positionY, (float)tamano_texto, 30 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, RED);
        }
        else {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, BLACK);
        }

        // Si se ha seleccionado una funcionalidad, pedir al usuario que presione una tecla para asignarla
        if (selected_key >= 0) {
            DrawText(TextFormat("Pulse una tecla para asignarla a la funcionalidad %s", keys_names[selected_key]), 20, 280, 20, BLACK);
        }
        else {
            DrawText("Pulse ENTER para guardar los controles y empezar a jugar", 20, 280, 20, BLACK);
            DrawText("Pulse BACKSPACE para volver a los controles predeterminados", 20, 280 + 30, 20, BLACK);
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