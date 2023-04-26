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
typedef enum GameScreen { MAIN_MENU, NIVEL_1, NIVEL_2, NIVEL_3, NIVEL_4, NIVEL_5, CONTROLS_MENU, GAME_OVER } GameScreen;

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
    int Modo_IA = 0;  // 0 -> IA original, 1 -> IA propia
    //--------------------------------------------------------------------------------------

    // GameOver:
    //--------------------------------------------------------------------------------------
    GameOver gameover = GameOver(300, 40.0f, 0.0f);
    //--------------------------------------------------------------------------------------
    
    //int numPlat = plataformas.listaPlataforma.size();

    AdministradorPompas admin = AdministradorPompas();
    Texture2D spriteAgua = LoadTexture("resources/agua.png");
    Rectangle destAgua = { -100, -100, 13, 13 };
    admin.agua = Agua(destAgua, true, spriteAgua, plataformas.listaPlataforma.size());
    admin.agua.existe = false;
    admin.col = &columnas;
    admin.plat = &plataformas;
    admin.iniciaMapa(3, 300*60);
    admin.CambioDeMapa(0); //DEBUG 4->0

    Texture2D spritePompa = LoadTexture("resources/Players/Bobblun/Pompa.png");
    Rectangle destRec = { GetScreenWidth() / 2.0f + 20, GetScreenHeight() / 2.0f - 20, (float)32, 32.0f }; //Dos primeros, ubicacion. Dos ultimos, dimensiones
    //Pompa p = Pompa(spritePompa, destRec, 5.0, 200.0, true, 100);
    //Pompa p = Pompa(spritePompa, destRec, 5.0, 200.0, true, 100);
     


    
    Rectangle destRob = { GetScreenWidth() / 2, 30, 32, 32 };
    sh_Enemigo fantasma = std::make_shared<Fantasma>(Fantasma("resources/enemyFantasma/fantasmaBasic.png", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
    admin.enemigos.push_back(fantasma);
    destRob = { (float)GetScreenWidth() / 2, 70, 32, 32 };
    sh_Enemigo rosa = std::make_shared<Rosa>(Rosa("USELESS", 2.0f, 40.0f, 1.0f, 1.0f, TARGET_FPS, destRob, admin));
    destRob = { (float)GetScreenWidth() / 2, 80, 32, 32 };
    sh_Enemigo robot = std::make_shared<Robot>(Robot("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));

    admin.enemigos.push_back(rosa);
    admin.enemigos.push_back(robot);

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

    Rectangle destBub = { GetScreenWidth() - 50, 50, 32, 32};//{ 100, GetScreenHeight() - 50, 32, 32 };
    Bub bub = Bub(2.0f, 30.0f, 4.0f, 2.0f, TARGET_FPS, destBub, admin, true); 
    Bub bob = Bub(2.0f, 30.0f, 4.0f, 2.0f, TARGET_FPS, destBub, admin, false);
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
            }
            else if (IsKeyPressed(tecla_p2) && credits.creditos >= 2) {  
                currentScreen = NIVEL_1;
                credits.creditos -= 2;
                admin.scores.hayP1 = true;
                admin.scores.hayP2 = true;
                contadorVidas.hayP2 = true;
                gameover.hayP2 = true;
                plataformas.inicio_de_ronda(1);
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

                contadorVidas.Actualizar(bub.numVidas, bob.numVidas, credits.creditos);  

                if (admin.hurryUp) {
                    plataformas.SeñalHurryUp();
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
            	}
                  
            }
            else if (admin.cambiaNivel) {
                admin.iniciaMapa(4, 60*30);
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

                contadorVidas.Actualizar(bub.numVidas, bob.numVidas, credits.creditos);

                if (admin.hurryUp) {
                    plataformas.SeñalHurryUp();
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

                contadorVidas.Actualizar(bub.numVidas, bob.numVidas, credits.creditos);

                if (admin.hurryUp) {
                    plataformas.SeñalHurryUp();
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

                contadorVidas.Actualizar(bub.numVidas, bob.numVidas, credits.creditos); 

                if (admin.hurryUp) {
                    plataformas.SeñalHurryUp();
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
                }

            }
            else if (admin.cambiaNivel) {
                admin.iniciaMapa(4, 30*60); // TODO
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

                contadorVidas.Actualizar(bub.numVidas, bob.numVidas, credits.creditos);

                if (admin.hurryUp) {
                    plataformas.SeñalHurryUp();
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
                }

            }
            else if (admin.cambiaNivel) {
                gameover.ronda = 5;
                gameover.clear = true;
                admin.frutas.clear();
                currentScreen = GAME_OVER;
            }  
        } break;
        case CONTROLS_MENU:
        {
            controls.Actualizar(Modo_IA);
            if (IsKeyPressed(KEY_ENTER)) {
                // ASIGNAR TODOS LOS BOTONES
                controls.guardarControlesNuevos();
                credits.tecla = controls.coin;
                tecla_p1 = controls.play1; 
                tecla_p2 = controls.play2;
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
                //Pompa::controlesJugador = controls;  // Descomentar

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
                bub.destRec = destBub;
                bub.cambioMapa = 2;
                bub.muerto = false;
                bub.muriendo = false;
                if (admin.scores.hayP2) {
                    admin.scores.hayP2 = false;
                    contadorVidas.hayP2 = false;
                    bob.numVidas = 2;
                    bob.destRec = destBub;
                    bob.cambioMapa = 2;
                    bob.muerto = false;
                    bob.muriendo = false;
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
                
                destRob = { (float)GetScreenWidth() / 2, 30, 32, 32 };
                fantasma = std::make_shared<Fantasma>(Fantasma("resources/enemyFantasma/fantasmaBasic.png", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(fantasma);
                
                destRob = { (float)GetScreenWidth() / 2, 70, 32, 32 };
                rosa = std::make_shared<Rosa>(Rosa("USELESS", 2.0f, 40.0f, 1.0f, 1.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(rosa);
                
                destRob = { (float)GetScreenWidth() / 2, 80, 32, 32 };
                robot = std::make_shared<Robot>(Robot("USELESS", 2.0f, 80.0f, 2.0f, 2.0f, TARGET_FPS, destRob, admin));
                admin.enemigos.push_back(robot);
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

        } break;
        case NIVEL_3:
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

        } break;
        case NIVEL_4:
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

        } break;
        case NIVEL_5:
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