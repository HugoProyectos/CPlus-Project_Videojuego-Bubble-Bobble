#pragma once
#include "raylib.h"
#include <string>

//Clase que se puede utilizar como clase padre de Personaje y de Suelo
class Sprite {
public:
    Texture2D sprite;
    int frameWidth = 0;
    int frameHeight = 0;

    Rectangle srcRec, destRec;
    Vector2 origin;
    float tamano;
};