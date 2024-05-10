#include <SFML/Graphics.hpp>
#include "mainMenu.h"
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
                        while (play.isOpen())
                        {
                            Event a;
                            while (play.pollEvent(a)) {

                                if (a.type == Event::Closed) {
                                    play.close();
                                }

                                if (a.type == Event::KeyPressed) {
                                    if (a.key.code == Keyboard::Escape) {
                                        play.close();
                                    }
                                }
                            }

                            help.close();
                            score.close();
                            level.close();
                            play.clear();
                            play.draw(menubg);
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
