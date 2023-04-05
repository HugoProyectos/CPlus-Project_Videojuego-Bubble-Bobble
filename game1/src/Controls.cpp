#include "raylib.h"
#include <iostream>
#include "ini.h"

struct Controls {
    int left;
    int right;
    int jump;
    int spit;
    int coin;
    int play1;
    int play2;
};

void pruebita777(Controls& controls) {

    

    const char* keys_names[] = {
        "Izquierda", "Derecha", "Saltar", "Escupir", "Insertar moneda", "Jugar 1P", "Jugar 2P"
    };
    int keys[] = {
        controls.left, controls.right, controls.jump, controls.spit, controls.coin,
        controls.play1, controls.play2
    };
    int selected_key = -1;

    ClearBackground(RAYWHITE);

    for (int i = 0; i < 7; i++) {
        // Dibujar el nombre de la funcionalidad y la tecla actualmente asignada
        DrawText(TextFormat("%s: %d", keys_names[i], keys[i]), 20, 20 + 30 * i, 20, BLACK);

        // Si el usuario hace clic en la funcionalidad, seleccionarla para cambiar la tecla
        Rectangle rectAux = { 20, 20 + 30 * i, 200, 30 };
        //DrawRectangle(20, 20 + 30 * i, 200, 30, SKYBLUE);
        Vector2 mousePoint = GetMousePosition();
        if (CheckCollisionPointRec(mousePoint, rectAux)) {
            selected_key = i;
        }
    }

    // Si se ha seleccionado una funcionalidad, pedir al usuario que presione una tecla para asignarla
    if (selected_key >= 0) {
        DrawText(TextFormat("Pulse una tecla para asignarla a la funcionalidad %s", keys_names[selected_key]), 20, 280, 20, BLACK);


        int keyPressed = GetKeyPressed();
        if (keyPressed != 0 && keyPressed != KEY_ENTER && keyPressed != KEY_DELETE) {
            keys[selected_key] = keyPressed; 
            controls.left = keys[0];
            controls.right = keys[1];
            controls.jump = keys[2];
            controls.spit = keys[3];
            controls.coin = keys[4];
            controls.play1 = keys[5];
            controls.play2 = keys[6];
            selected_key = -1;
        }
    }
    else {
        DrawText("Pulse ENTER para guardar o DELETE para salir", 20, 280, 20, BLACK);

        int keyPressed = GetKeyPressed();
        if (keyPressed == KEY_ENTER) {
            keys[selected_key] = keyPressed;
            controls.left = keys[0];
            controls.right = keys[1];
            controls.jump = keys[2];
            controls.spit = keys[3];
            controls.coin = keys[4];
            controls.play1 = keys[5];
            controls.play2 = keys[6];
            selected_key = -1;
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
    Controls controls = { KEY_A, KEY_D, KEY_SPACE, KEY_W, KEY_SIX, KEY_ONE, KEY_TWO };
    const int screenWidth = 800;
    const int screenHeight = 450;

    // create a file instance
    mINI::INIFile file("config.ini");

    // create a data structure
    mINI::INIStructure ini;

    // populate the structure
    ini["things"]["chairs"] = "20";
    ini["things"]["balloons"] = "100";

    // generate an INI file (overwrites any previous file)
    file.generate(ini);

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