#pragma once

#include <sstream>
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;
using namespace std;

#define max 5
class Menu
{
public:
	Menu(float x, float y)
	{
		if (!font.loadFromFile("fonts/arial.ttf")) {
			cout << "No font exits." << endl;
		}
		//play game option
		main[0].setFont(font);
		main[0].setFillColor(Color::White);
		main[0].setString("Play Game");
		main[0].setCharacterSize(40);
		main[0].setPosition(550, 250);

		//instructions
		main[1].setFont(font);
		main[1].setFillColor(Color::White);
		main[1].setString("Instructions");
		main[1].setCharacterSize(40);
		main[1].setPosition(550, 300);

		//high scores
		main[2].setFont(font);
		main[2].setFillColor(Color::White);
		main[2].setString("High Scores");
		main[2].setCharacterSize(40);
		main[2].setPosition(550,350);

		//levels
		main[3].setFont(font);
		main[3].setFillColor(Color::White);
		main[3].setString("Levels");
		main[3].setCharacterSize(40);
		main[3].setPosition(550, 400);

		//quit game
		main[4].setFont(font);
		main[4].setFillColor(Color::White);
		main[4].setString("Quit Game");
		main[4].setCharacterSize(40);
		main[4].setPosition(550, 450);
	}
	void draw(RenderWindow& window)
	{
		for(int i = 0; i < max; i++) {
			window.draw(main[i]);
		}
	}
	void MoveUp() {
		if (mainSelect - 1 >= 0) {
			main[mainSelect].setFillColor(Color::White);
			mainSelect--;
			if (mainSelect == -1) {
				mainSelect == 2;
			}
			main[mainSelect].setFillColor(Color::Red);

		}
	}
	void MoveDown() {
		if (mainSelect + 1 <= max) {
			main[mainSelect].setFillColor(Color::White);
			mainSelect+=1;
			if (mainSelect == 5) {
				mainSelect == 0;
			}
			main[mainSelect].setFillColor(Color::Red);
		}
	}
	int menuPressed()
	{
		return mainSelect;
	}

	~Menu()
	{

	}
private:
	int mainSelect=0;
	Font font;
	Text main[max];
};