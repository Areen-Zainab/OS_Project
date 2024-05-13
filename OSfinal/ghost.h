#pragma once
#ifndef GHOST_H_
#define GHOST_H_
#include <sstream>
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <pthread.h>
using namespace sf;
using namespace std;
struct gPosition
{
    float x;
    float y;
};
class Ghost {
public:
    RenderWindow* renderWindow;
    Sprite sprite;
    Texture texture;
    gPosition position;

public:

    Ghost() {
        //texture.loadFromFile(path);
       // sprite.setTexture(texture);
        //sprite.setScale(1, 1);
       /// position.x=250;
        //position.y=295;
    }
    void SetPosition(gPosition position){this->position = position; }
    void Move(float x, float y) {
        position.x +=(x*2);
        position.y +=(y*2);
    }

    void Display() {
        sprite.setPosition(position.x, position.y);
        renderWindow->draw(sprite);
    }
       void SetRenderWindow(RenderWindow* renderWindow){  this->renderWindow = renderWindow;}


};

#endif    