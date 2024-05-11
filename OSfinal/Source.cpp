#include <SFML/Graphics.hpp>
#include "mainMenu.h"
#include "pacman.h"

bool leftKeyPressed = false; 
bool rightKeyPressed = false; 
bool upKeyPressed = false; 
bool downKeyPressed = false; 

void* movePacman(void* arg) {
    Pacman* pacman = static_cast<Pacman*>(arg);

    while (true) {
         if (leftKeyPressed) {
            pacman->MoveLeft(5.0f); // Move Pac-Man left
            pacman->wrapAround();
        }
        else if (rightKeyPressed) {
            pacman->MoveRight(5.0f); // Move Pac-Man right
            pacman->wrapAround();
        }
         else if (upKeyPressed) {
            pacman->MoveUp(5.0f); // Move Pac-Man up
            pacman->wrapAround();
        }
         else if (downKeyPressed) {
            pacman->MoveDown(5.0f); // Move Pac-Man down
            pacman->wrapAround();
        }

	    sleep(milliseconds(50));
    }

    return NULL;
}

int main()
{
    //for main menu front page
    RenderWindow mainmenu(VideoMode(800, 800), "main menu");
    Menu menu(800, 800);
    RectangleShape menubg;
    menubg.setSize(Vector2f(800, 800));
    Texture bg;
    bg.loadFromFile("img/bg1.png");
    menubg.setTexture(&bg); 

    //for instruction screen
    RectangleShape helpbg;
    helpbg.setSize(Vector2f(800, 800));
    Texture help_bg;
    help_bg.loadFromFile("img/help.png");
    helpbg.setTexture(&help_bg);

    //for scores
    RectangleShape scorebg;
    scorebg.setSize(Vector2f(800, 800));
    Texture sbg;
    sbg.loadFromFile("img/highscores.png");
    scorebg.setTexture(&sbg);

    //for levels
    RectangleShape levelbg;
    levelbg.setSize(Vector2f(800, 800));
    Texture lbg;
    lbg.loadFromFile("img/level.png");
    levelbg.setTexture(&lbg);

    while (mainmenu.isOpen())
    {
        Event e;
        while (mainmenu.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                mainmenu.close();
            }
            if (e.type == Event::KeyReleased)
            {
                if (e.key.code == Keyboard::Up)
                {
                    menu.MoveUp();
                    break;
                }
                if (e.key.code == Keyboard::Down)
                {
                    menu.MoveDown();
                    break;
                }
                if (e.key.code == Keyboard::Return)
                {
                    RenderWindow play(VideoMode(800, 800), "game screen");
                    RenderWindow help(VideoMode(800, 800), "help screen");
                    RenderWindow score(VideoMode(800, 800), "score screen");
                    RenderWindow level(VideoMode(800, 800), "level screen");
                    int x = menu.menuPressed();
                    if (x == 0) //play game
                    {
                        Pacman pacman;
                        pacman.SetRenderWindow(&play);
                        pthread_t t1;
                        pthread_create(&t1, NULL, movePacman, &pacman);//pacman creataed

                        while (play.isOpen())
                        {
                            Event a;
                            while (play.pollEvent(a)) 
                            {

                                if (a.type == Event::Closed) {
                                    play.close();
                                }

                                if (a.type == Event::KeyPressed) {
                                    if (a.key.code == Keyboard::Escape) {
                                        play.close();
                                    }
                                }

                                if (a.type == Event::KeyPressed)
                                {
                                    switch (a.key.code) 
                                    {
                                        case Keyboard::Down:

                                            downKeyPressed = true;
                                            leftKeyPressed = false;
                                            rightKeyPressed = false;  
                                            upKeyPressed = false;                                     
                                            break;

                                        case Keyboard::Left:
                                            leftKeyPressed = true; 					    
                                            downKeyPressed = false;
                                            rightKeyPressed = false; 
                                            upKeyPressed = false;  
                                            break;
                                        case Keyboard::Right:
                                            rightKeyPressed = true; 
                                            downKeyPressed = false;
                                            leftKeyPressed = false; 
                                            upKeyPressed = false; 
                                            break;
                                        case Keyboard::Up:
                                            upKeyPressed = true; 
                                            downKeyPressed = false;
                                            leftKeyPressed = false; 
                                            rightKeyPressed = false; 
                                            break;
                                            
                                        default:
                                        break;
                                    }
				               }
                            }

                            help.close();
                            score.close();
                            level.close();
                            play.clear();
                            pacman.Display();
                            //play.draw(menubg);
                            play.display();
                        }
                    }
                    if (x == 1) {  //instructions
                        while (help.isOpen())
                        {
                            Event b;
                            while (help.pollEvent(b)) {

                                if (b.type == Event::Closed) {
                                    help.close();
                                }

                                if (b.type == Event::KeyPressed) {
                                    if (b.key.code == Keyboard::Escape) {
                                        help.close();
                                    }
                                }
                            }
                            play.close();                           
                            score.close();
                            level.close();
                            help.clear();
                            help.draw(helpbg);
                            help.display();

                        }
                    }
                    if (x == 2)  //high scores
                    {
                        while (score.isOpen())
                        {
                            Event c;
                            while (score.pollEvent(c)) {

                                if (c.type == Event::Closed) {
                                    score.close();
                                }

                                if (c.type == Event::KeyPressed) {
                                    if (c.key.code == Keyboard::Escape) {
                                        score.close();
                                    }
                                }
                            }
                            play.close();
                            level.close();
                            help.close();
                            score.clear();
                            score.draw(scorebg);
                            score.display();
                        }
                    }
                    if (x == 3)    //levels
                    {
                        while (level.isOpen())
                        {
                            Event c;
                            while (level.pollEvent(c)) {

                                if (c.type == Event::Closed) {
                                    level.close();
                                }

                                if (c.type == Event::KeyPressed) {
                                    if (c.key.code == Keyboard::Escape) {
                                        level.close();
                                    }
                                }
                            }
                            play.close();
                            score.close();                      
                            help.close();
                            level.clear();
                            level.draw(levelbg);
                            level.display();
                        }
                    }
                    if (x == 4) {
                        mainmenu.close();
                    }
                }
            }
            mainmenu.clear();
            mainmenu.draw(menubg);
            menu.draw(mainmenu);
            mainmenu.display();

        }
    }
    return 0;
}
