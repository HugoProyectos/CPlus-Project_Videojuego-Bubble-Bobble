#include "MainMenu.cpp"
#include "mapa.cpp"
#include "AdministradorPompas.cpp"
#include "clasesConSprite/Bub.cpp"
#include "Controls.cpp"

#include "clasesConSprite/Agua.hpp"
#include <clasesConSprite/Fantasma.cpp>
#include <clasesConSprite/Morado.cpp>
#include <clasesConSprite/Robot.cpp>
#include "GameOver.cpp"
#include <clasesConSprite/Rosa.cpp>
#include <clasesConSprite/Blanco.cpp>


const int TARGET_FPS = 60;

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
typedef enum GameScreen { MAIN_MENU, NIVEL_1, NIVEL_2, NIVEL_3, NIVEL_4, NIVEL_5, NIVEL_6, NIVEL_7, NIVEL_8, NIVEL_9, NIVEL_10, CONTROLS_MENU, GAME_OVER } GameScreen;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;//600;//800;

    InitWindow(screenWidth, screenHeight, "Bubble Bobble");
    SetWindowMinSize(200, 200);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
     
    InitAudioDevice();

    GameScreen currentScreen = MAIN_MENU;

    // TODO: Initialize all required variables and load all required data here!
    // 
    // Global:
    //--------------------------------------------------------------------------------------
    Credits credits = Credits(15, 10, 20, KEY_SIX);

    ContadorVidas contadorVidas = ContadorVidas("resources/Players/Bobblun/ContadorVida.png", "resources/Players/Bobblun/ContadorVida2.png", 40.0f, 0.0f);
    //-------------------------------------------------------------------------------------- 

    // Main Menu:
    //--------------------------------------------------------------------------------------
    MainMenu main_menu = MainMenu(10, 40, 70);
    unsigned int tecla_p1 = KEY_ONE;
    unsigned int tecla_p2 = KEY_TWO;
    //--------------------------------------------------------------------------------------

    // Nivel 1:
    //--------------------------------------------------------------------------------------
    Columnas columnas = Columnas("resources/mapa_nivel_1/bloque_grande.png", 40.0f, 0.0f, 1);
    Plataformas plataformas = Plataformas("resources/mapa_nivel_1/bloque_pequeno.png", "resources/mapa_nivel_1/mapa.txt", 40.0f, 0.0f);
    //--------------------------------------------------------------------------------------

    // Controls:
    //--------------------------------------------------------------------------------------
    Controls controls = Controls("config.ini"); 
    int Modo_IA = controls.IA_mode;  // 0 -> IA original, 1 -> IA propia
    // A�adir inicializacion de IA  
    int Modo_skins = controls.skin_modo; // 0 -> original, 1 -> alternativo
    // A�adir inicializacion de skins 
    int Modo_mapa = controls.mapa_modo; // 0 -> 2.5D, 1 -> 2D   
    plataformas.modo_2d = Modo_mapa == 1 ? true : false;
    columnas.modo_2d = Modo_mapa == 1 ? true : false;
    int Mute_music = controls.music_modo; // 1 -> muted
    int Mute_effect = controls.effect_modo; // 1 -> muted
    main_menu.mute_music = Mute_music == 1 ? true : false;  
    main_menu.mute_sound = Mute_effect == 1 ? true : false;
    credits.mute_sound = Mute_effect == 1 ? true : false;   
    plataformas.mute_effect = Mute_effect == 1 ? true : false;
    plataformas.mute_music = Mute_music == 1 ? true : false; 

    SetWindowSize((controls.resoluciones1_width[controls.resoluciones1_mode]), (controls.resoluciones1_height[controls.resoluciones1_mode]));

    if (controls.resoluciones2_mode == 1) {
        SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
        ToggleFullscreen();
    }
    else if (controls.resoluciones2_mode == 2) {
        SetWindowSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
    }
    
    //--------------------------------------------------------------------------------------

    // GameOver:
    //--------------------------------------------------------------------------------------
    GameOver gameover = GameOver(300, 40.0f, 0.0f);
    gameover.mute_music = Mute_music == 1 ? true : false; 
    //--------------------------------------------------------------------------------------

    //int numPlat = plataformas.listaPlataforma.size();

    AdministradorPompas admin = AdministradorPompas();
    Texture2D spriteAgua = LoadTexture("resources/agua.png");
    Rectangle destAgua = { -100, -100, GetScreenHeight() / 34.6f, 61.5f };
    admin.agua = Agua(destAgua, true, spriteAgua, plataformas.listaPlataforma.size());
    admin.agua.existe = false;
    admin.col = &columnas;
    admin.plat = &plataformas;
    admin.iniciaMapa(4, 30*60);
    admin.CambioDeMapa(0); 
    Texture2D spritePompa = LoadTexture("resources/Players/Bobblun/Pompa.png");
    Rectangle destRec = { GetScreenWidth() / 2.0f + 20, GetScreenHeight() / 2.0f - 20, (float)32, 32.0f }; //Dos primeros, ubicacion. Dos ultimos, dimensiones
    //Pompa p = Pompa(spritePompa, destRec, 5.0, 200.0, true, 100);
    //Pompa p = Pompa(spritePompa, destRec, 5.0, 200.0, true, 100);




    Rectangle destRob = { GetScreenWidth() / 2, 30, 32, 32 };

    sh_Enemigo rosa = std::make_shared<Robot>(Robot("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
    destRob = { (float)GetScreenWidth() / 2, 80, 32, 32 };
    sh_Enemigo robot = std::make_shared<Robot>(Robot("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
    destRob = { (float)GetScreenWidth() / 2, 30, 32, 32 };
    sh_Enemigo robot2 = std::make_shared<Robot>(Robot("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
    destRob = { (float)GetScreenWidth() / 2, 70, 36, 32 };
    sh_Enemigo robot3 = std::make_shared<Robot>(Robot("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));

    admin.enemigos.push_back(rosa);
    admin.enemigos.push_back(robot);
    admin.mute_sound = Mute_effect == 1 ? true : false;
    admin.enemigos.push_back(robot2);
    admin.enemigos.push_back(robot3);

    /*

    destRob = { (float)GetScreenWidth() / 2, 60, 32, 32 };
    sh_Enemigo mor = std::make_shared<Morado>(Morado("resources/enemyRobot/robotBasic.png", 2.0f, 80.0f, 1.0f, 1.0f, TARGET_FPS, destRob));
    admin.enemigos.push_back(mor); 
    destRob = { (float)GetScreenWidth() / 2, 30, 32, 32 };
    sh_Enemigo robot2 = std::make_shared<Robot>(Robot("resources/enemyRobot/robotBasic.png", 2.0f, 80.0f, 1.0f, 1.0f, TARGET_FPS, destRob));
    admin.enemigos.push_back(robot2);
    */
    /*
    Rectangle destPompa = { 100, GetScreenHeight() - GetScreenHeight()/2, 32, 32 };
    Pompa p = Pompa(spritePompa, destPompa, 0, 0, false, Pompa::INFINITA);
    Rectangle destPompa2 = { 120, GetScreenHeight() - GetScreenHeight() / 2, 32, 32 };
    Pompa p2 = Pompa(spritePompa, destPompa, 0, 0, false, Pompa::INFINITA);
    p.modulo = Pompa::MODULO_AGUA;
    p2.modulo = Pompa::MODULO_AGUA;
    p.indiceAnimacion = 2 * Pompa::NUM_FOTOGRAMAS;
    p2.indiceAnimacion = 2 * Pompa::NUM_FOTOGRAMAS;
    //admin.pompas.push_back(std::make_shared<Pompa>(p));
    //admin.pompas.push_back(std::make_shared<Pompa>(p2));
    */

    Rectangle destBub = { 750, 50, 32, 32};//{ 100, GetScreenHeight() - 50, 32, 32 };
    Bub bub = Bub(2.0f, 30.0f, 4.0f, 2.0f, TARGET_FPS, destBub, admin, true); 
    //bub.imTheThunder = true;//DEBUG
    Bub bob = Bub(2.0f, 30.0f, 4.0f, 2.0f, TARGET_FPS, destBub, admin, false);
    bub.mute_sound = Mute_effect == 1 ? true : false;
    bob.mute_sound = Mute_effect == 1 ? true : false;
    bub.segundaSkin = Modo_skins == 1 ? true : false;
    bob.segundaSkin = Modo_skins == 1 ? true : false;
    //bub.nivel = 4;
    //bob.nivel = 4;

    //--------------------------------------------------------------------------------------
    // Asignar controles iniciales
    credits.tecla = controls.coin;
    tecla_p1 = controls.play1;
    tecla_p2 = controls.play2;
    bub.left = controls.left_p1;
    bub.right = controls.right_p1;
    bub.jump = controls.jump_p1;
    bub.shoot = controls.spit_p1;
    bob.left = controls.left_p2;
    bob.right = controls.right_p2;
    bob.jump = controls.jump_p2;
    bob.shoot = controls.spit_p2;
    Pompa::jumpJ1 = controls.jump_p1;
    Pompa::jumpJ2 = controls.jump_p2;
    //--------------------------------------------------------------------------------------
    
    bool jugando_nivel = true; 

    //bub.destRec.x = 100; bub.destRec.y = 100;

    //--------------------------------------------------------------------------------------

    int framesCounter = 0;          // Useful to count frames

    SetTargetFPS(60);               // Set desired framerate (frames-per-second)
    //--------------------------------------------------------------------------------------

    /*bub.cayendo = true;
    bub.enElAire = true;*/ 

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        switch (currentScreen)
        {
        case MAIN_MENU:
        {
            // TODO: Update MAIN_MENU screen variables here!
            if (main_menu.Actualizar(credits.creditos)) { 
                currentScreen = CONTROLS_MENU;
            }
            credits.Actualizar();
            admin.scores.Actualizar();

            // Wait for 2 seconds (120 frames) before jumping to TITLE screen
            if (IsKeyPressed(tecla_p1) && credits.creditos >= 1)
            {
                currentScreen = NIVEL_1;
                credits.creditos -= 1;
                admin.scores.hayP1 = true;
                plataformas.inicio_de_ronda(1);
                bub.destRec = {(float) 750, 50, 32, 32 };
                // bub.resizeMe();
                bub.lastWidth = 800;
                bub.lastHeight = 450;
                bub.resetPosicionOriginal();
                bob.lastWidth = 800;
                bob.lastHeight = 450;
                bob.resetPosicionOriginal();
            }
            else if (IsKeyPressed(tecla_p2) && credits.creditos >= 2) {
                currentScreen = NIVEL_1;
                credits.creditos -= 2;
                admin.scores.hayP1 = true;
                admin.scores.hayP2 = true;
                contadorVidas.hayP2 = true;
                gameover.hayP2 = true;
                plataformas.inicio_de_ronda(1);
                bub.destRec = { (float)750, 50, 32, 32 };
                bob.destRec = { (float)750, 50, 32, 32 };
                //bub.destRec.x =(float)GetScreenWidth() - 50;
                // bub.resizeMe(); 
                //bob.destRec.x =(float)GetScreenWidth() - 50;
                bub.lastWidth = 800;
                bub.lastHeight = 450;
                bub.resetPosicionOriginal();
                bob.lastWidth = 800;
                bob.lastHeight = 450;
                bob.resetPosicionOriginal();
                // bob.resizeMe();
                //bob = Bub(2.0f, 30.0f, 4.0f, 2.0f, TARGET_FPS, destBub, admin, false);
            }
        } break;
        case NIVEL_1:
        {
            // TODO: Update NIVEL_1 screen variables here!
            if (!admin.cambiaNivel && jugando_nivel) {
                columnas.Actualizar();
                plataformas.Actualizar();
                credits.Actualizar();
                admin.scores.Actualizar();
                bub.Actualizar();
                for (int i = 0; i < plataformas.listaPlataforma.size(); i++) {
                    bub.compruebaColision(plataformas.listaPlataforma[i]);
                }
                bub.compruebaPared(columnas);
                if (bub.enElAgua) {
                    bub.destRec.x = admin.agua.stream[admin.agua.bubTile].destRec.x;
                    bub.destRec.y = admin.agua.stream[admin.agua.bubTile].destRec.y;
                }
                else if (admin.agua.existe) {
                    bub.enElAgua = admin.agua.colisionBub(bub.destRec, bub.waterlessFrames);
                }
                if (contadorVidas.hayP2) {
                    bob.Actualizar();
                    for (int i = 0; i < plataformas.listaPlataforma.size(); i++) {
                        bob.compruebaColision(plataformas.listaPlataforma[i]);
                    }
                    bob.compruebaPared(columnas);
                    if (bob.enElAgua) {
                        bob.destRec.x = admin.agua.stream[admin.agua.bubTile].destRec.x;
                        bob.destRec.y = admin.agua.stream[admin.agua.bubTile].destRec.y;
                    }
                    else if (admin.agua.existe) {
                        bob.enElAgua = admin.agua.colisionBub(bob.destRec, bob.waterlessFrames);
                    }
                }

                admin.agua.Actualizar(plataformas, columnas);
                admin.actualizaPompas();
                for (int i = 0; i < admin.enemigos.size(); i++) {
                    admin.agua.colisionEnemigo(*admin.enemigos.at(i));
                }

                admin.actualizaEnemigos(plataformas, columnas);

                admin.actualizaFrutas(plataformas);

                admin.actualizaRayos();

                contadorVidas.Actualizar(bub.numVidas, bob.numVidas, credits.creditos);  

                if (admin.hurryUp) {
                    plataformas.SenalHurryUp(); 
                    admin.hurryUp = false;
                }

                if (bub.muerto && bub.numVidas == 0 && !gameover.hayP2) {
                    gameover.ronda = 1;
                    currentScreen = GAME_OVER;
                }
                else if (gameover.hayP2 && bub.muerto && bob.muerto && bub.numVidas == 0 && bob.numVidas == 0) {
                    gameover.ronda = 1;
                    currentScreen = GAME_OVER;
                }

                if (IsKeyPressed(tecla_p2) && credits.creditos >= 1 && admin.scores.hayP1 && !admin.scores.hayP2)
                {
                    credits.creditos -= 1;
                    admin.scores.hayP2 = true;
                    contadorVidas.hayP2 = true;
                    gameover.hayP2 = true;
                    //bob.destRec.x = (float)GetScreenWidth() - 50;
                    //bob.resizeMe();
                }

            }
            else if (admin.cambiaNivel) {
                admin.iniciaMapa(4, 60 * 30);
                admin.CambioDeMapa(1);
                columnas.CargarSiguienteNivel("resources/mapa_nivel_2/bloque_grande.png", 2);
                plataformas.CargarSiguienteNivel("resources/mapa_nivel_2/bloque_pequeno.png", "resources/mapa_nivel_2/mapa.txt");
                contadorVidas.cargar_siguiente_nivel();
                bub.cambioMapa = 2;
                bob.cambioMapa = 2;
                bub.nivel = 1;
                bob.nivel = 1;
                jugando_nivel = false;
                for (int i = 0; i < 10; i++) {
                    admin.agua.stream[i].numPlataformas = plataformas.listaPlataformaSiguiente.size();
                }
            }
            else {
                currentScreen = NIVEL_2;
                destRob = { (float)GetScreenWidth() / 2 + 20, 100, 32, 32 };
                robot = std::make_shared<Morado>(Morado("USELESS", 2.0f, 40.0f, 1.0f, 1.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 - 20, 100, 32, 32 };
                robot = std::make_shared<Morado>(Morado("USELESS", 2.0f, 40.0f, 1.0f, 1.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 + 50, 190, 32, 32 };
                robot = std::make_shared<Morado>(Morado("USELESS", 2.0f, 40.0f, 1.0f, 1.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 - 50, 190, 32, 32 };
                robot = std::make_shared<Morado>(Morado("USELESS", 2.0f, 40.0f, 1.0f, 1.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                admin.frutas.clear();

                jugando_nivel = true;
                plataformas.inicio_de_ronda(2);
            }
        } break;
        case NIVEL_2:
        {
            // TODO: Update NIVEL_2 screen variables here!
            if (!admin.cambiaNivel && jugando_nivel) {
                columnas.Actualizar();
                plataformas.Actualizar();
                credits.Actualizar();
                admin.scores.Actualizar();
                bub.Actualizar();
                for (int i = 0; i < plataformas.listaPlataforma.size(); i++) {
                    bub.compruebaColision(plataformas.listaPlataforma[i]);
                }
                bub.compruebaPared(columnas);
                if (bub.enElAgua) {
                    bub.destRec.x = admin.agua.stream[admin.agua.bubTile].destRec.x;
                    bub.destRec.y = admin.agua.stream[admin.agua.bubTile].destRec.y;
                }
                else if (admin.agua.existe) {
                    bub.enElAgua = admin.agua.colisionBub(bub.destRec, bub.waterlessFrames);
                }
                if (contadorVidas.hayP2) {
                    bob.Actualizar();
                    for (int i = 0; i < plataformas.listaPlataforma.size(); i++) {
                        bob.compruebaColision(plataformas.listaPlataforma[i]);
                    }
                    bob.compruebaPared(columnas);
                    if (bob.enElAgua) {
                        bob.destRec.x = admin.agua.stream[admin.agua.bubTile].destRec.x;
                        bob.destRec.y = admin.agua.stream[admin.agua.bubTile].destRec.y;
                    }
                    else if (admin.agua.existe) {
                        bob.enElAgua = admin.agua.colisionBub(bob.destRec, bob.waterlessFrames);
                    }
                }
                admin.agua.Actualizar(plataformas, columnas);
                admin.actualizaPompas();
                for (int i = 0; i < admin.enemigos.size(); i++) {
                    admin.agua.colisionEnemigo(*admin.enemigos.at(i));
                }

                admin.actualizaEnemigos(plataformas, columnas);
                admin.actualizaFrutas(plataformas);

                admin.actualizaRayos();

                contadorVidas.Actualizar(bub.numVidas, bob.numVidas, credits.creditos);

                if (admin.hurryUp) {
                    plataformas.SenalHurryUp();
                    admin.hurryUp = false;
                }

                if (bub.muerto && bub.numVidas == 0 && !gameover.hayP2) {
                    gameover.ronda = 2;
                    currentScreen = GAME_OVER;
                }
                else if (gameover.hayP2 && bub.muerto && bob.muerto && bub.numVidas == 0 && bob.numVidas == 0) {
                    gameover.ronda = 2;
                    currentScreen = GAME_OVER;
                }

                if (IsKeyPressed(tecla_p2) && credits.creditos >= 1 && admin.scores.hayP1 && !admin.scores.hayP2)
                {
                    credits.creditos -= 1;
                    admin.scores.hayP2 = true;
                    contadorVidas.hayP2 = true;
                    gameover.hayP2 = true; 
                    //bob.destRec.x = (float)GetScreenWidth() - 50;
                    //bob.resizeMe();
                }

            }
            else if (admin.cambiaNivel) {
                admin.iniciaMapa(4, 30*60); // TODO 
                admin.CambioDeMapa(2); // TODO
                columnas.CargarSiguienteNivel("resources/mapa_nivel_3/bloque_grande.png", 3);
                plataformas.CargarSiguienteNivel("resources/mapa_nivel_3/bloque_pequeno.png", "resources/mapa_nivel_3/mapa.txt");
                contadorVidas.cargar_siguiente_nivel();
                bub.cambioMapa = 2; // TODO
                bob.cambioMapa = 2; // TODO
                bub.nivel = 2;
                bob.nivel = 2;
                jugando_nivel = false;
                for (int i = 0; i < 10; i++) {
                    admin.agua.stream[i].numPlataformas = plataformas.listaPlataformaSiguiente.size();
                }
            }
            else {
                currentScreen = NIVEL_3;
                destRob = { (float)GetScreenWidth() / 2 + 100, 100, 32, 32 };
                robot = std::make_shared<Robot>(Robot("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 - 100, 100, 32, 32 };
                robot = std::make_shared<Robot>(Robot("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)200, 180, 32, 32 };
                robot = std::make_shared<Robot>(Robot("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() - 200, 180, 32, 32 };
                robot = std::make_shared<Robot>(Robot("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                admin.frutas.clear();

                jugando_nivel = true;
                plataformas.inicio_de_ronda(3);
            }
        } break;
        case NIVEL_3:
        {
            // TODO: Update NIVEL_3 screen variables here!
            if (!admin.cambiaNivel && jugando_nivel) {
                columnas.Actualizar();
                plataformas.Actualizar();
                credits.Actualizar();
                admin.scores.Actualizar();
                bub.Actualizar();
                for (int i = 0; i < plataformas.listaPlataforma.size(); i++) {
                    bub.compruebaColision(plataformas.listaPlataforma[i]);
                }
                bub.compruebaPared(columnas);
                if (bub.enElAgua) {
                    bub.destRec.x = admin.agua.stream[admin.agua.bubTile].destRec.x;
                    bub.destRec.y = admin.agua.stream[admin.agua.bubTile].destRec.y;
                }
                else if (admin.agua.existe) {
                    bub.enElAgua = admin.agua.colisionBub(bub.destRec, bub.waterlessFrames);
                }
                if (contadorVidas.hayP2) {
                    bob.Actualizar();
                    for (int i = 0; i < plataformas.listaPlataforma.size(); i++) {
                        bob.compruebaColision(plataformas.listaPlataforma[i]);
                    }
                    bob.compruebaPared(columnas);
                    if (bob.enElAgua) {
                        bob.destRec.x = admin.agua.stream[admin.agua.bubTile].destRec.x;
                        bob.destRec.y = admin.agua.stream[admin.agua.bubTile].destRec.y;
                    }
                    else if (admin.agua.existe) {
                        bob.enElAgua = admin.agua.colisionBub(bob.destRec, bob.waterlessFrames);
                    }
                }
                admin.agua.Actualizar(plataformas, columnas);
                admin.actualizaPompas();
                for (int i = 0; i < admin.enemigos.size(); i++) {
                    admin.agua.colisionEnemigo(*admin.enemigos.at(i));
                }

                admin.actualizaEnemigos(plataformas, columnas);
                admin.actualizaFrutas(plataformas);

                admin.actualizaRayos();

                contadorVidas.Actualizar(bub.numVidas, bob.numVidas, credits.creditos);

                if (admin.hurryUp) {
                    plataformas.SenalHurryUp();
                    admin.hurryUp = false;
                }

                if (bub.muerto && bub.numVidas == 0 && !gameover.hayP2) {
                    gameover.ronda = 3;
                    currentScreen = GAME_OVER;
                }
                else if (gameover.hayP2 && bub.muerto && bob.muerto && bub.numVidas == 0 && bob.numVidas == 0) {
                    gameover.ronda = 3;
                    currentScreen = GAME_OVER;
                }

                if (IsKeyPressed(tecla_p2) && credits.creditos >= 1 && admin.scores.hayP1 && !admin.scores.hayP2)
                {
                    credits.creditos -= 1;
                    admin.scores.hayP2 = true;
                    contadorVidas.hayP2 = true;
                    gameover.hayP2 = true;
                    //bob.destRec.x = (float)GetScreenWidth() - 50;
                    //bob.resizeMe();
                }

            }
            else if (admin.cambiaNivel) {
                admin.iniciaMapa(6, 30*60); // TODO 
                admin.CambioDeMapa(3); // TODO
                columnas.CargarSiguienteNivel("resources/mapa_nivel_4/bloque_grande.png", 4);
                plataformas.CargarSiguienteNivel("resources/mapa_nivel_4/bloque_pequeno.png", "resources/mapa_nivel_4/mapa.txt");
                contadorVidas.cargar_siguiente_nivel();
                bub.cambioMapa = 2; // TODO
                bob.cambioMapa = 2; // TODO
                bub.nivel = 3;
                bob.nivel = 3;
                jugando_nivel = false;
                for (int i = 0; i < 10; i++) {
                    admin.agua.stream[i].numPlataformas = plataformas.listaPlataformaSiguiente.size();
                }
            }
            else {
                currentScreen = NIVEL_4;
                destRob = { (float)(GetScreenWidth() * 0.18), 90, 32, 32 };
                robot = std::make_shared<Rosa>(Rosa("USELESS", 2.0f, 40.0f, 1.0f, 1.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)(GetScreenWidth() * 0.82), 90, 32, 32 };
                robot = std::make_shared<Robot>(Robot("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)(GetScreenWidth() * 0.25), 150, 32, 32 };
                robot = std::make_shared<Robot>(Robot("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)(GetScreenWidth() * 0.75), 150, 32, 32 };
                robot = std::make_shared<Rosa>(Rosa("USELESS", 2.0f, 40.0f, 1.0f, 1.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { 200, (float)GetScreenHeight() - 150, 32, 32 };
                robot = std::make_shared<Robot>(Robot("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() - 200, (float)GetScreenHeight() - 150, 32, 32 };
                robot = std::make_shared<Robot>(Robot("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);
                 
                admin.frutas.clear();

                jugando_nivel = true;
                plataformas.inicio_de_ronda(4);
            }
        } break;
        case NIVEL_4:
        {
            // TODO: Update NIVEL_4 screen variables here!     
            if (!admin.cambiaNivel && jugando_nivel) {
                columnas.Actualizar();
                plataformas.Actualizar();
                credits.Actualizar();
                admin.scores.Actualizar();
                bub.Actualizar();
                for (int i = 0; i < plataformas.listaPlataforma.size(); i++) {
                    bub.compruebaColision(plataformas.listaPlataforma[i]);
                }
                bub.compruebaPared(columnas);
                if (bub.enElAgua) {
                    bub.destRec.x = admin.agua.stream[admin.agua.bubTile].destRec.x;
                    bub.destRec.y = admin.agua.stream[admin.agua.bubTile].destRec.y;
                }
                else if (admin.agua.existe) {
                    bub.enElAgua = admin.agua.colisionBub(bub.destRec, bub.waterlessFrames);
                }
                if (contadorVidas.hayP2) {
                    bob.Actualizar();
                    for (int i = 0; i < plataformas.listaPlataforma.size(); i++) {
                        bob.compruebaColision(plataformas.listaPlataforma[i]);
                    }
                    bob.compruebaPared(columnas);
                    if (bob.enElAgua) {
                        bob.destRec.x = admin.agua.stream[admin.agua.bubTile].destRec.x;
                        bob.destRec.y = admin.agua.stream[admin.agua.bubTile].destRec.y;
                    }
                    else if (admin.agua.existe) {
                        bob.enElAgua = admin.agua.colisionBub(bob.destRec, bob.waterlessFrames);
                    }
                }
                admin.agua.Actualizar(plataformas, columnas);
                admin.actualizaPompas();
                for (int i = 0; i < admin.enemigos.size(); i++) {
                    admin.agua.colisionEnemigo(*admin.enemigos.at(i));
                }

                admin.actualizaEnemigos(plataformas, columnas);
                admin.actualizaFrutas(plataformas);

                admin.actualizaRayos();

                contadorVidas.Actualizar(bub.numVidas, bob.numVidas, credits.creditos); 

                if (admin.hurryUp) {
                    plataformas.SenalHurryUp();
                    admin.hurryUp = false;
                }

                if (bub.muerto && bub.numVidas == 0 && !gameover.hayP2) {
                    gameover.ronda = 4;
                    currentScreen = GAME_OVER;
                }
                else if (gameover.hayP2 && bub.muerto && bob.muerto && bub.numVidas == 0 && bob.numVidas == 0) {
                    gameover.ronda = 4;
                    currentScreen = GAME_OVER;
                }

                if (IsKeyPressed(tecla_p2) && credits.creditos >= 1 && admin.scores.hayP1 && !admin.scores.hayP2)
                {
                    credits.creditos -= 1;
                    admin.scores.hayP2 = true;
                    contadorVidas.hayP2 = true;
                    gameover.hayP2 = true;
                    //bob.destRec.x = (float)GetScreenWidth() - 50;
                    //bob.resizeMe();
                }

            }
            else if (admin.cambiaNivel) {
                //CAMBIADO PARA PRUEBAS DE AGUA
                admin.iniciaMapa(4, 30 * 60); // TODO
                admin.CambioDeMapa(4); // TODO
                columnas.CargarSiguienteNivel("resources/mapa_nivel_5/bloque_grande.png", 5);
                plataformas.CargarSiguienteNivel("resources/mapa_nivel_5/bloque_pequeno.png", "resources/mapa_nivel_5/mapa.txt"); 
                contadorVidas.cargar_siguiente_nivel();
                bub.cambioMapa = 2; // TODO
                bob.cambioMapa = 2; // TODO
                bub.nivel = 4;
                bob.nivel = 4;
                jugando_nivel = false;
                for (int i = 0; i < 10; i++) {
                    admin.agua.stream[i].numPlataformas = plataformas.listaPlataformaSiguiente.size();
                }
            }
            else {
                currentScreen = NIVEL_5;
                destRob = { (float)GetScreenWidth() / 2 + 70, 60, 32, 32 }; 
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 + 20, 160, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 - 20, 240, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 - 70, 320, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                admin.frutas.clear();

                jugando_nivel = true;
                plataformas.inicio_de_ronda(5);
            }
        } break;
        case NIVEL_5:
        {
            // TODO: Update NIVEL_5 screen variables here!
            if (!admin.cambiaNivel && jugando_nivel) {
                columnas.Actualizar();
                plataformas.Actualizar();
                credits.Actualizar();
                admin.scores.Actualizar();
                bub.Actualizar();
                for (int i = 0; i < plataformas.listaPlataforma.size(); i++) {
                    bub.compruebaColision(plataformas.listaPlataforma[i]);
                }
                bub.compruebaPared(columnas);
                if (bub.enElAgua) {
                    bub.destRec.x = admin.agua.stream[admin.agua.bubTile].destRec.x;
                    bub.destRec.y = admin.agua.stream[admin.agua.bubTile].destRec.y;
                }
                else if (admin.agua.existe) {
                    bub.enElAgua = admin.agua.colisionBub(bub.destRec, bub.waterlessFrames);
                }
                if (contadorVidas.hayP2) {
                    bob.Actualizar();
                    for (int i = 0; i < plataformas.listaPlataforma.size(); i++) {
                        bob.compruebaColision(plataformas.listaPlataforma[i]);  
                    }
                    bob.compruebaPared(columnas);
                    if (bob.enElAgua) {
                        bob.destRec.x = admin.agua.stream[admin.agua.bubTile].destRec.x;
                        bob.destRec.y = admin.agua.stream[admin.agua.bubTile].destRec.y;
                    }
                    else if (admin.agua.existe) {
                        bob.enElAgua = admin.agua.colisionBub(bob.destRec, bob.waterlessFrames); 
                    }
                }
                admin.agua.Actualizar(plataformas, columnas);
                admin.actualizaPompas();
                for (int i = 0; i < admin.enemigos.size(); i++) {
                    admin.agua.colisionEnemigo(*admin.enemigos.at(i));
                }

                admin.actualizaEnemigos(plataformas, columnas);
                admin.actualizaFrutas(plataformas);

                admin.actualizaRayos();

                contadorVidas.Actualizar(bub.numVidas, bob.numVidas, credits.creditos);

                if (admin.hurryUp) {
                    plataformas.SenalHurryUp();
                    admin.hurryUp = false;
                }

                if (bub.muerto && bub.numVidas == 0 && !gameover.hayP2) {
                    gameover.ronda = 5;
                    currentScreen = GAME_OVER;  
                }
                else if (gameover.hayP2 && bub.muerto && bob.muerto && bub.numVidas == 0 && bob.numVidas == 0) {
                    gameover.ronda = 5;
                    currentScreen = GAME_OVER; 
                }

                if (IsKeyPressed(tecla_p2) && credits.creditos >= 1 && admin.scores.hayP1 && !admin.scores.hayP2)
                {
                    credits.creditos -= 1;
                    admin.scores.hayP2 = true;
                    contadorVidas.hayP2 = true;
                    gameover.hayP2 = true;
                    //bob.destRec.x = (float)GetScreenWidth() - 50;
                    //bob.resizeMe();
                }

            }
            else if (admin.cambiaNivel) {
                //CAMBIADO PARA PRUEBAS DE AGUA
                admin.iniciaMapa(4, 30 * 60); // TODO
                admin.CambioDeMapa(5); // TODO
                columnas.CargarSiguienteNivel("resources/mapa_nivel_6/bloque_grande.png", 6);
                plataformas.CargarSiguienteNivel("resources/mapa_nivel_6/bloque_pequeno.png", "resources/mapa_nivel_6/mapa.txt");
                contadorVidas.cargar_siguiente_nivel();
                bub.cambioMapa = 2; // TODO
                bob.cambioMapa = 2; // TODO
                bub.nivel = 5;
                bob.nivel = 5;
                jugando_nivel = false;
                for (int i = 0; i < 10; i++) {
                    admin.agua.stream[i].numPlataformas = plataformas.listaPlataformaSiguiente.size();
                }
            }
            else {
                currentScreen = NIVEL_6;

                // ----------------------------------------------------------------------------------------------------------
                // Poner enemigos correctos
                destRob = {(float)GetScreenWidth() / 2 + 70, 60, 32, 32};
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 + 20, 160, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 - 20, 240, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 - 70, 320, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);
                // ----------------------------------------------------------------------------------------------------------

                admin.frutas.clear();
                jugando_nivel = true;
                plataformas.inicio_de_ronda(6);
            }
        } break;
        case NIVEL_6:
        {
            // TODO: Update NIVEL_6 screen variables here!
            if (!admin.cambiaNivel && jugando_nivel) {
                columnas.Actualizar();
                plataformas.Actualizar();
                credits.Actualizar();
                admin.scores.Actualizar();
                bub.Actualizar();
                for (int i = 0; i < plataformas.listaPlataforma.size(); i++) {
                    bub.compruebaColision(plataformas.listaPlataforma[i]);
                }
                bub.compruebaPared(columnas);
                if (bub.enElAgua) {
                    bub.destRec.x = admin.agua.stream[admin.agua.bubTile].destRec.x;
                    bub.destRec.y = admin.agua.stream[admin.agua.bubTile].destRec.y;
                }
                else if (admin.agua.existe) {
                    bub.enElAgua = admin.agua.colisionBub(bub.destRec, bub.waterlessFrames);
                }
                if (contadorVidas.hayP2) {
                    bob.Actualizar();
                    for (int i = 0; i < plataformas.listaPlataforma.size(); i++) {
                        bob.compruebaColision(plataformas.listaPlataforma[i]);
                    }
                    bob.compruebaPared(columnas);
                    if (bob.enElAgua) {
                        bob.destRec.x = admin.agua.stream[admin.agua.bubTile].destRec.x;
                        bob.destRec.y = admin.agua.stream[admin.agua.bubTile].destRec.y;
                    }
                    else if (admin.agua.existe) {
                        bob.enElAgua = admin.agua.colisionBub(bob.destRec, bob.waterlessFrames);
                    }
                }
                admin.agua.Actualizar(plataformas, columnas);
                admin.actualizaPompas();
                for (int i = 0; i < admin.enemigos.size(); i++) {
                    admin.agua.colisionEnemigo(*admin.enemigos.at(i));
                }

                admin.actualizaEnemigos(plataformas, columnas);
                admin.actualizaFrutas(plataformas);

                contadorVidas.Actualizar(bub.numVidas, bob.numVidas, credits.creditos);

                if (admin.hurryUp) {
                    plataformas.SenalHurryUp();
                    admin.hurryUp = false;
                }

                if (bub.muerto && bub.numVidas == 0 && !gameover.hayP2) {
                    gameover.ronda = 6;
                    currentScreen = GAME_OVER;
                }
                else if (gameover.hayP2 && bub.muerto && bob.muerto && bub.numVidas == 0 && bob.numVidas == 0) {
                    gameover.ronda = 6;
                    currentScreen = GAME_OVER;
                }

                if (IsKeyPressed(tecla_p2) && credits.creditos >= 1 && admin.scores.hayP1 && !admin.scores.hayP2)
                {
                    credits.creditos -= 1;
                    admin.scores.hayP2 = true;
                    contadorVidas.hayP2 = true;
                    gameover.hayP2 = true;
                }

            }
            else if (admin.cambiaNivel) {
                //CAMBIADO PARA PRUEBAS DE AGUA
                admin.iniciaMapa(4, 30 * 60); // TODO
                admin.CambioDeMapa(6); // TODO
                columnas.CargarSiguienteNivel("resources/mapa_nivel_7/bloque_grande.png", 7);
                plataformas.CargarSiguienteNivel("resources/mapa_nivel_7/bloque_pequeno.png", "resources/mapa_nivel_7/mapa.txt");
                contadorVidas.cargar_siguiente_nivel();
                bub.cambioMapa = 2; // TODO
                bob.cambioMapa = 2; // TODO
                bub.nivel = 6;
                bob.nivel = 6;
                jugando_nivel = false;
                for (int i = 0; i < 10; i++) {
                    admin.agua.stream[i].numPlataformas = plataformas.listaPlataformaSiguiente.size();
                }
            }
            else {
                currentScreen = NIVEL_7;

                // ----------------------------------------------------------------------------------------------------------
                // Poner enemigos correctos
                destRob = { (float)GetScreenWidth() / 2 + 70, 60, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 + 20, 160, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 - 20, 240, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 - 70, 320, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);
                // ----------------------------------------------------------------------------------------------------------

                admin.frutas.clear();

                jugando_nivel = true;
                plataformas.inicio_de_ronda(7);
            }
        } break;
        case NIVEL_7:
        {
            // TODO: Update NIVEL_7 screen variables here!
            if (!admin.cambiaNivel && jugando_nivel) {
                columnas.Actualizar();
                plataformas.Actualizar();
                credits.Actualizar();
                admin.scores.Actualizar();
                bub.Actualizar();
                for (int i = 0; i < plataformas.listaPlataforma.size(); i++) {
                    bub.compruebaColision(plataformas.listaPlataforma[i]);
                }
                bub.compruebaPared(columnas);
                if (bub.enElAgua) {
                    bub.destRec.x = admin.agua.stream[admin.agua.bubTile].destRec.x;
                    bub.destRec.y = admin.agua.stream[admin.agua.bubTile].destRec.y;
                }
                else if (admin.agua.existe) {
                    bub.enElAgua = admin.agua.colisionBub(bub.destRec, bub.waterlessFrames);
                }
                if (contadorVidas.hayP2) {
                    bob.Actualizar();
                    for (int i = 0; i < plataformas.listaPlataforma.size(); i++) {
                        bob.compruebaColision(plataformas.listaPlataforma[i]);
                    }
                    bob.compruebaPared(columnas);
                    if (bob.enElAgua) {
                        bob.destRec.x = admin.agua.stream[admin.agua.bubTile].destRec.x;
                        bob.destRec.y = admin.agua.stream[admin.agua.bubTile].destRec.y;
                    }
                    else if (admin.agua.existe) {
                        bob.enElAgua = admin.agua.colisionBub(bob.destRec, bob.waterlessFrames);
                    }
                }
                admin.agua.Actualizar(plataformas, columnas);
                admin.actualizaPompas();
                for (int i = 0; i < admin.enemigos.size(); i++) {
                    admin.agua.colisionEnemigo(*admin.enemigos.at(i));
                }

                admin.actualizaEnemigos(plataformas, columnas);
                admin.actualizaFrutas(plataformas);

                contadorVidas.Actualizar(bub.numVidas, bob.numVidas, credits.creditos);

                if (admin.hurryUp) {
                    plataformas.SenalHurryUp();
                    admin.hurryUp = false;
                }

                if (bub.muerto && bub.numVidas == 0 && !gameover.hayP2) {
                    gameover.ronda = 7;
                    currentScreen = GAME_OVER;
                }
                else if (gameover.hayP2 && bub.muerto && bob.muerto && bub.numVidas == 0 && bob.numVidas == 0) {
                    gameover.ronda = 7;
                    currentScreen = GAME_OVER;
                }

                if (IsKeyPressed(tecla_p2) && credits.creditos >= 1 && admin.scores.hayP1 && !admin.scores.hayP2)
                {
                    credits.creditos -= 1;
                    admin.scores.hayP2 = true;
                    contadorVidas.hayP2 = true;
                    gameover.hayP2 = true;
                }

            }
            else if (admin.cambiaNivel) {
                //CAMBIADO PARA PRUEBAS DE AGUA
                admin.iniciaMapa(4, 30 * 60); // TODO
                admin.CambioDeMapa(7); // TODO
                columnas.CargarSiguienteNivel("resources/mapa_nivel_8/bloque_grande.png", 8);
                plataformas.CargarSiguienteNivel("resources/mapa_nivel_8/bloque_pequeno.png", "resources/mapa_nivel_8/mapa.txt");
                contadorVidas.cargar_siguiente_nivel();
                bub.cambioMapa = 2; // TODO
                bob.cambioMapa = 2; // TODO
                bub.nivel = 7;
                bob.nivel = 7;
                jugando_nivel = false;
                for (int i = 0; i < 10; i++) {
                    admin.agua.stream[i].numPlataformas = plataformas.listaPlataformaSiguiente.size();
                }
            }
            else {
                currentScreen = NIVEL_8;

                // ----------------------------------------------------------------------------------------------------------
                // Poner enemigos correctos
                destRob = { (float)GetScreenWidth() / 2 + 70, 60, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 + 20, 160, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 - 20, 240, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 - 70, 320, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);
                // ----------------------------------------------------------------------------------------------------------

                admin.frutas.clear();

                jugando_nivel = true;
                plataformas.inicio_de_ronda(8);
            }
        } break;
        case NIVEL_8:
        {
            // TODO: Update NIVEL_8 screen variables here!
            if (!admin.cambiaNivel && jugando_nivel) {
                columnas.Actualizar();
                plataformas.Actualizar();
                credits.Actualizar();
                admin.scores.Actualizar();
                bub.Actualizar();
                for (int i = 0; i < plataformas.listaPlataforma.size(); i++) {
                    bub.compruebaColision(plataformas.listaPlataforma[i]);
                }
                bub.compruebaPared(columnas);
                if (bub.enElAgua) {
                    bub.destRec.x = admin.agua.stream[admin.agua.bubTile].destRec.x;
                    bub.destRec.y = admin.agua.stream[admin.agua.bubTile].destRec.y;
                }
                else if (admin.agua.existe) {
                    bub.enElAgua = admin.agua.colisionBub(bub.destRec, bub.waterlessFrames);
                }
                if (contadorVidas.hayP2) {
                    bob.Actualizar();
                    for (int i = 0; i < plataformas.listaPlataforma.size(); i++) {
                        bob.compruebaColision(plataformas.listaPlataforma[i]);
                    }
                    bob.compruebaPared(columnas);
                    if (bob.enElAgua) {
                        bob.destRec.x = admin.agua.stream[admin.agua.bubTile].destRec.x;
                        bob.destRec.y = admin.agua.stream[admin.agua.bubTile].destRec.y;
                    }
                    else if (admin.agua.existe) {
                        bob.enElAgua = admin.agua.colisionBub(bob.destRec, bob.waterlessFrames);
                    }
                }
                admin.agua.Actualizar(plataformas, columnas);
                admin.actualizaPompas();
                for (int i = 0; i < admin.enemigos.size(); i++) {
                    admin.agua.colisionEnemigo(*admin.enemigos.at(i));
                }

                admin.actualizaEnemigos(plataformas, columnas);
                admin.actualizaFrutas(plataformas);

                contadorVidas.Actualizar(bub.numVidas, bob.numVidas, credits.creditos);

                if (admin.hurryUp) {
                    plataformas.SenalHurryUp();
                    admin.hurryUp = false;
                }

                if (bub.muerto && bub.numVidas == 0 && !gameover.hayP2) {
                    gameover.ronda = 8;
                    currentScreen = GAME_OVER;
                }
                else if (gameover.hayP2 && bub.muerto && bob.muerto && bub.numVidas == 0 && bob.numVidas == 0) {
                    gameover.ronda = 8;
                    currentScreen = GAME_OVER;
                }

                if (IsKeyPressed(tecla_p2) && credits.creditos >= 1 && admin.scores.hayP1 && !admin.scores.hayP2)
                {
                    credits.creditos -= 1;
                    admin.scores.hayP2 = true;
                    contadorVidas.hayP2 = true;
                    gameover.hayP2 = true;
                }

            }
            else if (admin.cambiaNivel) {
                //CAMBIADO PARA PRUEBAS DE AGUA
                admin.iniciaMapa(4, 30 * 60); // TODO
                admin.CambioDeMapa(8); // TODO
                columnas.CargarSiguienteNivel("resources/mapa_nivel_9/bloque_grande.png", 9);
                plataformas.CargarSiguienteNivel("resources/mapa_nivel_9/bloque_pequeno.png", "resources/mapa_nivel_9/mapa.txt");
                contadorVidas.cargar_siguiente_nivel();
                bub.cambioMapa = 2; // TODO
                bob.cambioMapa = 2; // TODO
                bub.nivel = 8;
                bob.nivel = 8;
                jugando_nivel = false;
                for (int i = 0; i < 10; i++) {
                    admin.agua.stream[i].numPlataformas = plataformas.listaPlataformaSiguiente.size();
                }
            }
            else {
                currentScreen = NIVEL_9;

                // ----------------------------------------------------------------------------------------------------------
                // Poner enemigos correctos
                destRob = { (float)GetScreenWidth() / 2 + 70, 60, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 + 20, 160, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 - 20, 240, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 - 70, 320, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);
                // ----------------------------------------------------------------------------------------------------------

                admin.frutas.clear();

                jugando_nivel = true;
                plataformas.inicio_de_ronda(9);
            }
        } break;
        case NIVEL_9:
        {
            // TODO: Update NIVEL_9 screen variables here!
            if (!admin.cambiaNivel && jugando_nivel) {
                columnas.Actualizar();
                plataformas.Actualizar();
                credits.Actualizar();
                admin.scores.Actualizar();
                bub.Actualizar();
                for (int i = 0; i < plataformas.listaPlataforma.size(); i++) {
                    bub.compruebaColision(plataformas.listaPlataforma[i]);
                }
                bub.compruebaPared(columnas);
                if (bub.enElAgua) {
                    bub.destRec.x = admin.agua.stream[admin.agua.bubTile].destRec.x;
                    bub.destRec.y = admin.agua.stream[admin.agua.bubTile].destRec.y;
                }
                else if (admin.agua.existe) {
                    bub.enElAgua = admin.agua.colisionBub(bub.destRec, bub.waterlessFrames);
                }
                if (contadorVidas.hayP2) {
                    bob.Actualizar();
                    for (int i = 0; i < plataformas.listaPlataforma.size(); i++) {
                        bob.compruebaColision(plataformas.listaPlataforma[i]);
                    }
                    bob.compruebaPared(columnas);
                    if (bob.enElAgua) {
                        bob.destRec.x = admin.agua.stream[admin.agua.bubTile].destRec.x;
                        bob.destRec.y = admin.agua.stream[admin.agua.bubTile].destRec.y;
                    }
                    else if (admin.agua.existe) {
                        bob.enElAgua = admin.agua.colisionBub(bob.destRec, bob.waterlessFrames);
                    }
                }
                admin.agua.Actualizar(plataformas, columnas);
                admin.actualizaPompas();
                for (int i = 0; i < admin.enemigos.size(); i++) {
                    admin.agua.colisionEnemigo(*admin.enemigos.at(i));
                }

                admin.actualizaEnemigos(plataformas, columnas);
                admin.actualizaFrutas(plataformas);

                contadorVidas.Actualizar(bub.numVidas, bob.numVidas, credits.creditos);

                if (admin.hurryUp) {
                    plataformas.SenalHurryUp();
                    admin.hurryUp = false;
                }

                if (bub.muerto && bub.numVidas == 0 && !gameover.hayP2) {
                    gameover.ronda = 9;
                    currentScreen = GAME_OVER;
                }
                else if (gameover.hayP2 && bub.muerto && bob.muerto && bub.numVidas == 0 && bob.numVidas == 0) {
                    gameover.ronda = 9;
                    currentScreen = GAME_OVER;
                }

                if (IsKeyPressed(tecla_p2) && credits.creditos >= 1 && admin.scores.hayP1 && !admin.scores.hayP2)
                {
                    credits.creditos -= 1;
                    admin.scores.hayP2 = true;
                    contadorVidas.hayP2 = true;
                    gameover.hayP2 = true;
                }

            }
            else if (admin.cambiaNivel) {
                //CAMBIADO PARA PRUEBAS DE AGUA
                admin.iniciaMapa(4, 30 * 60); // TODO
                admin.CambioDeMapa(9); // TODO
                columnas.CargarSiguienteNivel("resources/mapa_nivel_10/bloque_grande.png", 10);
                plataformas.CargarSiguienteNivel("resources/mapa_nivel_10/bloque_pequeno.png", "resources/mapa_nivel_10/mapa.txt");
                contadorVidas.cargar_siguiente_nivel();
                bub.cambioMapa = 2; // TODO
                bob.cambioMapa = 2; // TODO
                bub.nivel = 9;
                bob.nivel = 9;
                jugando_nivel = false;
                for (int i = 0; i < 10; i++) {
                    admin.agua.stream[i].numPlataformas = plataformas.listaPlataformaSiguiente.size();
                }
            }
            else {
                currentScreen = NIVEL_10;

                // ----------------------------------------------------------------------------------------------------------
                // Poner enemigos correctos
                destRob = { (float)GetScreenWidth() / 2 + 70, 60, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 + 20, 160, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 - 20, 240, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);

                destRob = { (float)GetScreenWidth() / 2 - 70, 320, 32, 32 };
                robot = std::make_shared<Fantasma>(Fantasma("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);
                // ----------------------------------------------------------------------------------------------------------

                admin.frutas.clear();

                jugando_nivel = true;
                plataformas.inicio_de_ronda(10);
            }
        } break;
        case NIVEL_10:
        {
            // TODO: Update NIVEL_10 screen variables here!
            if (!admin.cambiaNivel && jugando_nivel) {
                columnas.Actualizar();
                plataformas.Actualizar();
                credits.Actualizar();
                admin.scores.Actualizar();
                bub.Actualizar();
                for (int i = 0; i < plataformas.listaPlataforma.size(); i++) {
                    bub.compruebaColision(plataformas.listaPlataforma[i]);
                }
                bub.compruebaPared(columnas);
                if (bub.enElAgua) {
                    bub.destRec.x = admin.agua.stream[admin.agua.bubTile].destRec.x;
                    bub.destRec.y = admin.agua.stream[admin.agua.bubTile].destRec.y;
                }
                else if (admin.agua.existe) {
                    bub.enElAgua = admin.agua.colisionBub(bub.destRec, bub.waterlessFrames);
                }
                if (contadorVidas.hayP2) {
                    bob.Actualizar();
                    for (int i = 0; i < plataformas.listaPlataforma.size(); i++) {
                        bob.compruebaColision(plataformas.listaPlataforma[i]);
                    }
                    bob.compruebaPared(columnas);
                    if (bob.enElAgua) {
                        bob.destRec.x = admin.agua.stream[admin.agua.bubTile].destRec.x;
                        bob.destRec.y = admin.agua.stream[admin.agua.bubTile].destRec.y;
                    }
                    else if (admin.agua.existe) {
                        bob.enElAgua = admin.agua.colisionBub(bob.destRec, bob.waterlessFrames);
                    }
                }
                admin.agua.Actualizar(plataformas, columnas);
                admin.actualizaPompas();
                for (int i = 0; i < admin.enemigos.size(); i++) {
                    admin.agua.colisionEnemigo(*admin.enemigos.at(i));
                }

                admin.actualizaEnemigos(plataformas, columnas);
                admin.actualizaFrutas(plataformas);

                contadorVidas.Actualizar(bub.numVidas, bob.numVidas, credits.creditos);

                if (admin.hurryUp) {
                    plataformas.SenalHurryUp();
                    admin.hurryUp = false;
                }

                if (bub.muerto && bub.numVidas == 0 && !gameover.hayP2) {
                    gameover.ronda = 10;
                    currentScreen = GAME_OVER;
                }
                else if (gameover.hayP2 && bub.muerto && bob.muerto && bub.numVidas == 0 && bob.numVidas == 0) {
                    gameover.ronda = 10;
                    currentScreen = GAME_OVER;
                }

                if (IsKeyPressed(tecla_p2) && credits.creditos >= 1 && admin.scores.hayP1 && !admin.scores.hayP2)
                {
                    credits.creditos -= 1;
                    admin.scores.hayP2 = true;
                    contadorVidas.hayP2 = true;
                    gameover.hayP2 = true;
                }

            }
            else if (admin.cambiaNivel) {
                gameover.ronda = 10;
                gameover.clear = true;
                admin.frutas.clear();
                currentScreen = GAME_OVER;
            }
        } break;
        case CONTROLS_MENU:
        { 
            controls.Actualizar(Modo_IA, Modo_skins, Modo_mapa, Mute_music, Mute_effect);
            if (IsKeyPressed(KEY_ENTER)) {
                // ASIGNAR TODOS LOS BOTONES
                controls.guardarControlesNuevos();
                credits.tecla = controls.coin;
                tecla_p1 = controls.play1; 
                tecla_p2 = controls.play2; 
                columnas.modo_2d = Modo_mapa == 1 ? true : false;
                plataformas.modo_2d = Modo_mapa == 1 ? true : false;
                main_menu.mute_music = Mute_music == 1 ? true : false;
                main_menu.mute_sound = Mute_effect == 1 ? true : false;
                credits.mute_sound = Mute_effect == 1 ? true : false;
                plataformas.mute_effect = Mute_effect == 1 ? true : false;
                plataformas.mute_music = Mute_music == 1 ? true : false;
                gameover.mute_music = Mute_music == 1 ? true : false;
                bub.mute_sound = Mute_effect == 1 ? true : false;
                bob.mute_sound = Mute_effect == 1 ? true : false;
                admin.mute_sound = Mute_effect == 1 ? true : false;
                bub.segundaSkin = Modo_skins == 1 ? true : false;
                bob.segundaSkin = Modo_skins == 1 ? true : false;
                // ...
                // Asignamos los controles de los jugadores 
                /*controls.keys[0] = controls.left_p1;
                controls.keys[1] = controls.right_p1;
                controls.keys[2] = controls.jump_p1;
                controls.keys[3] = controls.spit_p1;
                controls.keys[4] = controls.left_p2;  
                controls.keys[5] = controls.right_p2;
                controls.keys[6] = controls.jump_p2;
                controls.keys[7] = controls.spit_p2;*/ 
                bub.left = controls.left_p1;
                bub.right = controls.right_p1;
                bub.jump = controls.jump_p1;
                bub.shoot = controls.spit_p1;
                bob.left = controls.left_p2;
                bob.right = controls.right_p2;
                bob.jump = controls.jump_p2; 
                bob.shoot = controls.spit_p2;

                //Pompa::controlesJugador = controls;
                Pompa::jumpJ1 = controls.jump_p1;
                Pompa::jumpJ2 = controls.jump_p2;

                currentScreen = MAIN_MENU;
            }
        } break;
        case GAME_OVER:
        {
            admin.scores.Actualizar();
            credits.Actualizar();
            if (gameover.Actualizar()) {
                currentScreen = MAIN_MENU;
                admin.scores.hayP1 = false;
                bub.numVidas = 2;
                //bub.destRec = {GetScreenWidth() * (destBub.x / 800), GetScreenHeight() * (destBub.y/450), GetScreenWidth() / 25.0f, GetScreenHeight() / 14.0625f };
                bub.cambioMapa = 2;
                bub.muerto = false;
                bub.muriendo = false;
                bub.primeraActualizacion = true;
                if (admin.scores.hayP2) {
                    admin.scores.hayP2 = false;
                    contadorVidas.hayP2 = false;
                    bob.numVidas = 2;
                    //bob.destRec = { GetScreenWidth() * (destBub.x / 800), GetScreenHeight() * (destBub.y / 450), GetScreenWidth() / 25.0f, GetScreenHeight() / 14.0625f };
                    bob.cambioMapa = 2;
                    bob.muerto = false;
                    bob.muriendo = false;
                    bob.primeraActualizacion = true;
                }
                gameover.clear = false;
                gameover.hayP2 = false;
                admin.scores.puntuacion1 = 0;
                admin.scores.puntuacion2 = 0;

                //Reseteo del nivel
                admin.iniciaMapa(3, 30*60); 
                admin.CambioDeMapa(0);
                columnas.VolverAlPrimerNivel("resources/mapa_nivel_1/bloque_grande.png", 1);
                plataformas.VolverAlPrimerNivel("resources/mapa_nivel_1/bloque_pequeno.png", "resources/mapa_nivel_1/mapa.txt");
                
                Rectangle destRob = { GetScreenWidth() / 2, 30, 32, 32 };
                sh_Enemigo rosa = std::make_shared<Robot>(Robot("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                destRob = { (float)GetScreenWidth() / 2, 80, 32, 32 };
                sh_Enemigo robot = std::make_shared<Robot>(Robot("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                destRob = { (float)GetScreenWidth() / 2, 30, 32, 32 };
                sh_Enemigo robot2 = std::make_shared<Robot>(Robot("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                destRob = { (float)GetScreenWidth() / 2, 70, 36, 32 };
                sh_Enemigo robot3 = std::make_shared<Robot>(Robot("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));

                admin.enemigos.push_back(rosa);
                admin.enemigos.push_back(robot);
                admin.enemigos.push_back(robot2);
                admin.enemigos.push_back(robot3);
            }
        } break;
        default: break;
        }
        //----------------------------------------------------------------------------------
        /*
        if (IsKeyDown(KEY_D)) {
            char a;
            std::cin >> a;
        }*/ 

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);
        ///DEBUG
        //DrawLine(0, bub.posicionOriginalBub.y + bub.destRec.height/2, GetScreenWidth(), bub.posicionOriginalBub.y + bub.destRec.height / 2, GRAY);
        //DrawLine(bub.posicionOriginalBub.x + bub.destRec.width / 2, 0, bub.posicionOriginalBub.x + bub.destRec.width / 2, GetScreenHeight(), GRAY);
        /// DEBUG

        switch (currentScreen)
        {
        case MAIN_MENU:
        {
            // TODO: Draw MAIN_MENU screen here!
            main_menu.Dibujar();
            credits.Dibujar();
            //scores.Dibujar();
            admin.scores.Dibujar();

        } break;
        case NIVEL_1:
        {
            // TODO: Draw NIVEL_1 screen here! 
            columnas.DibujarIzquierda();
            plataformas.Dibujar();
            columnas.DibujarDerecha();
            //scores.Dibujar();   
            admin.scores.Dibujar();
            admin.agua.Dibujar();
            contadorVidas.Dibujar();
            if (!bub.muerto) {
                bub.Dibujar();
            }
            if (contadorVidas.hayP2 && !bob.muerto) {
                bob.Dibujar();
            }
            admin.dibujaPompas();
            admin.dibujaEnemigos();
            admin.dibujaFrutas();
            admin.dibujarPuntosCadena();
        } break;
        case NIVEL_2:
        {
            // TODO: Draw NIVEL_2 screen here!
            columnas.DibujarIzquierda();
            plataformas.Dibujar();
            columnas.DibujarDerecha();
            admin.scores.Dibujar();
            admin.agua.Dibujar();
            contadorVidas.Dibujar();
            if (!bub.muerto) {
                bub.Dibujar();
            }
            if (contadorVidas.hayP2 && !bob.muerto) {
                bob.Dibujar();
            }
            admin.dibujaPompas();
            admin.dibujaEnemigos();
            admin.dibujaFrutas();
            admin.DibujaRayos();
            admin.dibujarPuntosCadena();

        } break;
        case NIVEL_3:
        {
            // TODO: Draw NIVEL_3 screen here!
            columnas.DibujarIzquierda();
            plataformas.Dibujar();
            columnas.DibujarDerecha();
            admin.scores.Dibujar();
            admin.agua.Dibujar();
            contadorVidas.Dibujar();
            if (!bub.muerto) {
                bub.Dibujar();
            }
            if (contadorVidas.hayP2 && !bob.muerto) {
                bob.Dibujar();
            }
            admin.dibujaPompas();
            admin.dibujaEnemigos();
            admin.dibujaFrutas();
            admin.DibujaRayos();
            admin.dibujarPuntosCadena();

        } break;
        case NIVEL_4:
        {
            // TODO: Draw NIVEL_4 screen here!
            columnas.DibujarIzquierda();
            plataformas.Dibujar();
            columnas.DibujarDerecha();
            admin.scores.Dibujar();
            admin.agua.Dibujar();
            contadorVidas.Dibujar();
            if (!bub.muerto) {
                bub.Dibujar();
            }
            if (contadorVidas.hayP2 && !bob.muerto) {
                bob.Dibujar();
            }
            admin.dibujaPompas();
            admin.dibujaEnemigos();
            admin.dibujaFrutas();
            admin.DibujaRayos();
            admin.dibujarPuntosCadena();

        } break;
        case NIVEL_5:
        {
            // TODO: Draw NIVEL_5 screen here!
            columnas.DibujarIzquierda();  
            plataformas.Dibujar();
            columnas.DibujarDerecha();
            admin.scores.Dibujar();
            admin.agua.Dibujar();
            contadorVidas.Dibujar();
            if (!bub.muerto) {
                bub.Dibujar();
            }
            if (contadorVidas.hayP2 && !bob.muerto) {
                bob.Dibujar();
            }
            admin.dibujaPompas();
            admin.dibujaEnemigos();
            admin.dibujaFrutas();
            admin.DibujaRayos();
            admin.dibujarPuntosCadena();

        } break;
        case NIVEL_6:
        {
            // TODO: Draw NIVEL_6 screen here!
            columnas.DibujarIzquierda();
            plataformas.Dibujar();
            columnas.DibujarDerecha();
            admin.scores.Dibujar();
            admin.agua.Dibujar();
            contadorVidas.Dibujar();
            if (!bub.muerto) {
                bub.Dibujar();
            }
            if (contadorVidas.hayP2 && !bob.muerto) {
                bob.Dibujar();
            }
            admin.dibujaPompas();
            admin.dibujaEnemigos();
            admin.dibujaFrutas();
            admin.DibujaRayos();
            admin.dibujarPuntosCadena();

        } break;
        case NIVEL_7:
        {
            // TODO: Draw NIVEL_7 screen here!
            columnas.DibujarIzquierda();
            plataformas.Dibujar();
            columnas.DibujarDerecha();
            admin.scores.Dibujar();
            admin.agua.Dibujar();
            contadorVidas.Dibujar();
            if (!bub.muerto) {
                bub.Dibujar();
            }
            if (contadorVidas.hayP2 && !bob.muerto) {
                bob.Dibujar();
            }
            admin.dibujaPompas();
            admin.dibujaEnemigos();
            admin.dibujaFrutas();
            admin.DibujaRayos();
            admin.dibujarPuntosCadena();

        } break;
        case NIVEL_8:
        {
            // TODO: Draw NIVEL_8 screen here!
            columnas.DibujarIzquierda();
            plataformas.Dibujar();
            columnas.DibujarDerecha();
            admin.scores.Dibujar();
            admin.agua.Dibujar();
            contadorVidas.Dibujar();
            if (!bub.muerto) {
                bub.Dibujar();
            }
            if (contadorVidas.hayP2 && !bob.muerto) {
                bob.Dibujar();
            }
            admin.dibujaPompas();
            admin.dibujaEnemigos();
            admin.dibujaFrutas();
            admin.DibujaRayos();
            admin.dibujarPuntosCadena();

        } break;
        case NIVEL_9:
        {
            // TODO: Draw NIVEL_9 screen here!
            columnas.DibujarIzquierda();
            plataformas.Dibujar();
            columnas.DibujarDerecha();
            admin.scores.Dibujar();
            admin.agua.Dibujar();
            contadorVidas.Dibujar();
            if (!bub.muerto) {
                bub.Dibujar();
            }
            if (contadorVidas.hayP2 && !bob.muerto) {
                bob.Dibujar();
            }
            admin.dibujaPompas();
            admin.dibujaEnemigos();
            admin.dibujaFrutas();
            admin.DibujaRayos();
            admin.dibujarPuntosCadena();

        } break;
        case NIVEL_10:
        {
            // TODO: Draw NIVEL_10 screen here!
            columnas.DibujarIzquierda();
            plataformas.Dibujar();
            columnas.DibujarDerecha();
            admin.scores.Dibujar();
            admin.agua.Dibujar();
            contadorVidas.Dibujar();
            if (!bub.muerto) {
                bub.Dibujar();
            }
            if (contadorVidas.hayP2 && !bob.muerto) {
                bob.Dibujar();
            }
            admin.dibujaPompas();
            admin.dibujaEnemigos();
            admin.dibujaFrutas();
            admin.DibujaRayos();
            admin.dibujarPuntosCadena();

        } break;
        case CONTROLS_MENU:
        {
            controls.Dibujar();
        } break;
        case GAME_OVER:
        {
            admin.scores.Dibujar();
            gameover.Dibujar();
        } break;
        default: break;
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    // TODO: Unload all loaded data (textures, fonts, audio) here!
    main_menu.Unload();   
    columnas.Unload();
    for (int i = plataformas.listaPlataforma.size() - 1; i >= 0; i--) {
        plataformas.listaPlataforma[i].Unload();
    }
    plataformas.Unload();
    credits.Unload();
    admin.scores.Unload();
    gameover.Unload();
    controls.Unload();
    contadorVidas.Unload();

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}