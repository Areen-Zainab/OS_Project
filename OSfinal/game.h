#pragma once
#ifndef GAME_H_
#define GAME_H_
#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include "pacman.h"

using namespace sf;
using namespace std;

class Game{
public:
    RenderWindow* renderWindow;
    Sprite background; //Game background sprite
    Texture bg_texture;
    Sprite bar; //the scores bar
    Texture bar_texture;
    Font font; //the font used
    Pacman pacman;

    void SetRenderWindow(RenderWindow* renderWindow)
    {
        this->renderWindow = renderWindow;
        pacman.SetRenderWindow(renderWindow);
    }

    void Display()
    {
        pacman.Display();
    }


};

#endif


