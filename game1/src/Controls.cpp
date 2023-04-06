#include "raylib.h"
#include <iostream>
#include "ini.h"

struct Controls {
    unsigned int left_p1;
    unsigned int right_p1;
    unsigned int jump_p1;
    unsigned int spit_p1;
    unsigned int left_p2;
    unsigned int right_p2;
    unsigned int jump_p2;
    unsigned int spit_p2;
    unsigned int coin;
    unsigned int play1;
    unsigned int play2;
};

void actualizarControles(Controls& controls, unsigned int keys[]) {
    controls.left_p1 = keys[0];
    controls.right_p1 = keys[1];
    controls.jump_p1 = keys[2];
    controls.spit_p1 = keys[3];
    controls.left_p2 = keys[4];
    controls.right_p2 = keys[5];
    controls.jump_p2 = keys[6];
    controls.spit_p2 = keys[7];
    controls.coin = keys[8];
    controls.play1 = keys[9];
    controls.play2 = keys[10];
}

void cargarControles(Controls& controls, std::string ruta_fichero_configuracion) {
    // first, create a file instance
    mINI::INIFile file(ruta_fichero_configuracion);

    // next, create a structure that will hold data
    mINI::INIStructure ini;

    // now we can read the file
    file.read(ini);

    controls.left_p1 = stoi(ini["P1"]["left"]);
    controls.right_p1 = stoi(ini["P1"]["right"]);
    controls.jump_p1 = stoi(ini["P1"]["jump"]);
    controls.spit_p1 = stoi(ini["P1"]["spit"]);
    controls.left_p2 = stoi(ini["P2"]["left"]);
    controls.right_p2 = stoi(ini["P2"]["right"]);
    controls.jump_p2 = stoi(ini["P2"]["jump"]);
    controls.spit_p2 = stoi(ini["P2"]["spit"]);
    controls.coin = stoi(ini["global"]["coin"]);
    controls.play1 = stoi(ini["global"]["play1"]);
    controls.play2 = stoi(ini["global"]["play2"]);
}

void guardarControlesNuevos(Controls& controls) {
    // create a file instance
    mINI::INIFile file("config.ini");

    // create a data structure
    mINI::INIStructure ini;

    // populate the structure
    ini["P1"]["left"] = std::to_string(controls.left_p1);
    ini["P1"]["right"] = std::to_string(controls.right_p1);
    ini["P1"]["jump"] = std::to_string(controls.jump_p1);
    ini["P1"]["spit"] = std::to_string(controls.jump_p1);

    ini["P2"]["left"] = std::to_string(controls.left_p2);
    ini["P2"]["right"] = std::to_string(controls.right_p2);
    ini["P2"]["jump"] = std::to_string(controls.jump_p2);
    ini["P2"]["spit"] = std::to_string(controls.jump_p2);

    ini["global"]["play1"] = std::to_string(controls.play1);
    ini["global"]["play2"] = std::to_string(controls.play2);
    ini["global"]["coin"] = std::to_string(controls.coin);

    // generate an INI file (overwrites any previous file)
    file.generate(ini);
}


void pruebita777(Controls& controls) {

    

    const char* keys_names[] = {
        "Izquierda P1", "Derecha P1", "Saltar P1", "Escupir P1", 
        "Izquierda P2", "Derecha P2", "Saltar P2", "Escupir P2",
        "Insertar moneda", "Jugar 1P", "Jugar 2P"
    };
    unsigned int keys[] = {
        controls.left_p1, controls.right_p1, controls.jump_p1, controls.spit_p1, 
        controls.left_p2, controls.right_p2, controls.jump_p2, controls.spit_p2, 
        controls.coin, controls.play1, controls.play2
    };

    int selected_key = -1;
    int num_elements = sizeof(keys) / sizeof(keys[0]);

    ClearBackground(RAYWHITE);

    for (int i = 0; i < num_elements; i++) {
        unsigned int positionX;
        unsigned int positionY;
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
        std::string texto = TextFormat("%s: %d", keys_names[i], keys[i]);
        int tamano_texto = MeasureText(texto.c_str(), 20);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        Rectangle rectAux = { positionX, positionY, tamano_texto, 30 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        Vector2 mousePoint = GetMousePosition();

        // Si ponemos el cursor encima
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            selected_key = i;
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, RED);
        }
        else {
            // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
            DrawText(texto.c_str(), positionX, positionY, 20, BLACK);
        }


    }

    // Si se ha seleccionado una funcionalidad, pedir al usuario que presione una tecla para asignarla
    if (selected_key >= 0) {
        DrawText(TextFormat("Pulse una tecla para asignarla a la funcionalidad %s", keys_names[selected_key]), 20, 280, 20, BLACK);


        int keyPressed = GetKeyPressed();
        if (keyPressed != 0 && keyPressed != KEY_ENTER && keyPressed != KEY_BACKSPACE) {
            keys[selected_key] = keyPressed; 
            actualizarControles(controls, keys);
            selected_key = -1;
        }
    }
    else {
        DrawText("Pulse ENTER para guardar los controles nuevos y salir", 20, 280, 20, BLACK);
        DrawText("Pulse BACKSPACE para volver a los controles predeterminados", 20, 280 + 30, 20, BLACK);

        int keyPressed = GetKeyPressed();
        if (keyPressed == KEY_ENTER) {
            guardarControlesNuevos(controls);
        }
        else if (keyPressed == KEY_BACKSPACE) {
            cargarControles(controls, "config_default.ini");
        }
    }
}

void ChangeControls(Controls& controls) {
    

    DrawText("Press ENTER to save changes or ESCAPE to cancel", 10, 430, 20, BLACK);

    if (IsKeyPressed(KEY_ENTER)) {
        // Guardar cambios en un archivo o en una estructura global del juego
    }
    else if (IsKeyPressed(KEY_ESCAPE)) {
        // Descartar cambios y volver al menú principal del juego
        // o cargar los controles originales si se guardaron en una estructura global
        // antes de abrir la ventana de cambio de controles
        // (por ejemplo, si se implementó una opción en el menú del juego)
    }
}

int main() {
    // Inicializar ventana y otros elementos del juego
    Controls controls;
    cargarControles(controls, "config.ini");

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Bubble Bobble");
    SetWindowMinSize(200, 200);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    while (!WindowShouldClose()) {
        // Procesar eventos de juego
        //ChangeControls(controls);
        pruebita777(controls);

        // Dibujar elementos del juego en la pantalla

        EndDrawing();
    }

    // Cerrar ventana y liberar memoria utilizada por el juego

    return 0;
}