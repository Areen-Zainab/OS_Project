#pragma once
#ifndef PACMAN_H_
#define PACMAN_H_

#include <sstream>
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <semaphore.h>
using namespace sf;
using namespace std;

struct Position
{
    float x;
    float y;
};

class Pacman //:public Controllers
{
public:
    Sprite sprite;
    Position position;
    Texture texture;
    RenderWindow* renderWindow;
    sem_t* ghostMoveSemaphore;
    Pacman(){
        texture.loadFromFile("img/pacman.png");
        sprite = Sprite(texture);
        sprite.setScale(0.15, 0.15);
        position.x = 90;
        position.y = 100;
    }
    void SetPosition(Position position){this->position = position; }

    void MoveLeft(float delta_x){ position.x -= delta_x;}

    void MoveRight(float delta_x) {  position.x += delta_x;}

    void MoveUp(float delta_y){ position.y -= delta_y; }

    void MoveDown(float delta_y){ position.y += delta_y;}
    void Move(float x, float y)
    {
       // sem_wait(&ghostMoveSemaphore); // Acquire semaphore before updating board
        position.x +=x;
        position.y +=y;
      //  sem_post(&ghostMoveSemaphore); // Release semaphore after updates
    }
    void wrapAround()
    {
        if (position.y < 0){ position.y = 800;}
        if (position.y > 800)position.y = 0;
        if (position.x > 800) position.x = 0;
        if (position.x < 0) position.x = 800;
    }
    void Display(){
        sprite.setPosition(position.x, position.y);
        renderWindow->draw(sprite);
    }
    void SetRenderWindow(RenderWindow* renderWindow){  this->renderWindow = renderWindow;}

    ~Pacman(){}
};

#endif
