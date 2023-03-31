#include "Agua.hpp"

Agua::Agua() = default; //Debe llamarse a Inicializador

Agua::Agua(float tamano, float saltoMax, float velSalto, float velLateral, int _targetFrames, Rectangle destino, AdministradorPompas& adm) {
    Inicializador(tamano, saltoMax, velSalto, velLateral, _targetFrames, adm);
    destRec = destino;
    //inicio = destino;
};
void Agua::Inicializador(float tamano, float saltoMax, float velSalto, float velLateral, int _targetFrames, AdministradorPompas& adm)
{
    //
    admin = &adm;

    sprite = LoadTexture("resources/Players/Bobblun/animation_set.png");
    //widthAnimation = standingAnimation.width / fStandingAnimation;
    //heightAnimation = standingAnimation.height;
    //targetFrames = _targetFrames;

    std::cout << "Anchura y altura de un fotograma" << std::endl;
    std::cout << widthAnimation << std::endl;
    std::cout << heightAnimation << std::endl;

    //distanciaSaltoMax = saltoMax;//60.0f;
    //velocidadSalto = velSalto;
    //velocidadLateral = velLateral;

    // Source rectangle (part of the texture to use for drawing)
    srcRec = { 0.0f, 0.0f, (float)heightAnimation, (float)widthAnimation };
    this->tamano = tamano;

    // Destination rectangle (screen rectangle where drawing part of texture)
    destRec = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f, (float)widthAnimation * tamano, (float)heightAnimation * tamano }; //Dos primeros, ubicacion. Dos ultimos, dimensiones
    //inicio = destRec;

    // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
    origin = { (float)widthAnimation * tamano / 2, (float)heightAnimation * tamano / 2 }; //En principio no lo necesitamos
}