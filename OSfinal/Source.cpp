#include <SFML/Graphics.hpp>
#include "mainMenu.h"
#include "pacman.h"
#include "ghost.h"
#include <semaphore.h>
#include <pthread.h>
using namespace sf;
using namespace std;

bool leftKeyPressed = false; 
bool rightKeyPressed = false; 
bool upKeyPressed = false; 
bool downKeyPressed = false; 

void* moveGhost(void* arg) {
    Ghost* ghost = static_cast<Ghost*>(arg);
    while (true) {
        ghost->MoveRandomly(5.0f); // Move ghost randomly

        // Sleep for a short time to avoid busy waiting
        sleep(milliseconds(100));
    }
    return NULL;
}

const int NUM_GHOSTS = 4;
const int WIDTH = 22;
const int HEIGHT = 20;
const int gsize = 35;

int maze[HEIGHT][WIDTH] = { {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                            {1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1},
                            {1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                            {1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1},
                            {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},//6
                            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                            {1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
                            {1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1},
                            {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},//10
                            {1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1},
                            {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
                            {1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
                            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                            {1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
                            {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                            {1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
                            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

//maze bnane wala function
void drawmaze(RenderWindow &ok){
   for (int i = 0; i < HEIGHT; i++) 
   {
      for (int j = 0; j < WIDTH; j++) 
      {
          if(maze[i][j]==0)
          {
             CircleShape golu(5,5);
             golu.setPosition(j*gsize+40, i*gsize+60);
             golu.setFillColor(Color::Yellow);
              ok.draw(golu);
          }
          else if(maze[i][j]==1)
          {
             RectangleShape dabba(Vector2f(30,30));
            dabba.setPosition(j*gsize+30, i*gsize+50);
            dabba.setFillColor(Color::Blue);
            ok.draw(dabba);
          }
        }
                         
   }
}

//wall ki collision check krne wala beautiful function
bool CheckWallCollision(const Pacman& pacman, float x, float y) {
  FloatRect pacmanBox = pacman.sprite.getGlobalBounds();

  float newX = pacmanBox.left + x;
  float newY = pacmanBox.top + y;

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {

      if (maze[i][j] == 1) 
      { // shit yaar wall agai hai
        RectangleShape currentWall(Vector2f(30, 30));
        currentWall.setPosition(j * gsize + 30, i * gsize + 50);
        FloatRect wallBox = currentWall.getGlobalBounds();
        if (FloatRect(newX, newY, pacmanBox.width, pacmanBox.height).intersects(wallBox)) {
          return true; // Collision ho rahi ha bhaiii
        }
      }
    }
  }

  return false; // yayy no collision
}

void* movePacman(void* arg) {
    Pacman* pacman = static_cast<Pacman*>(arg);

    while (true) {
        float dx = 0.0f;
        float dy = 0.0f;
         if (leftKeyPressed) {
            //pacman->MoveLeft(5.0f); // Move Pac-Man left
            dx = -5.0f;
            //pacman->wrapAround();
        }
        else if (rightKeyPressed) {
           // pacman->MoveRight(5.0f); // Move Pac-Man right
           // pacman->wrapAround();
           dx = 5.0f;
        }
         else if (upKeyPressed) {
            //pacman->MoveUp(5.0f); // Move Pac-Man up
            //pacman->wrapAround();
            dy = -5.0f;
        }
         else if (downKeyPressed) {
           // pacman->MoveDown(5.0f); // Move Pac-Man down
           // pacman->wrapAround();
           dy = 5.0f;
        }
         if (!CheckWallCollision(*pacman, dx, dy)) {
            // If not, move Pacman
            pacman->Move(dx, dy);
            pacman->wrapAround();

        }else if(CheckWallCollision(*pacman, dx, dy))
        {
            if (leftKeyPressed) {
            rightKeyPressed=true;
            leftKeyPressed = false;
            dx = 5.0f;
            pacman->Move(dx, dy);
            pacman->wrapAround();
        }
        else if (rightKeyPressed) {
           leftKeyPressed = true;
           rightKeyPressed= false;
           dx = -5.0f;
           pacman->Move(dx, dy);
           pacman->wrapAround();
        }
         else if (upKeyPressed) {
            upKeyPressed = false;
            downKeyPressed = true;
            dy = 5.0f;
            pacman->Move(dx, dy);
            pacman->wrapAround();
        }
         else if (downKeyPressed) {
            downKeyPressed = false;
            upKeyPressed = true;
           dy = -5.0f;
           pacman->Move(dx, dy);
           pacman->wrapAround();
        }

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

                         Ghost ghosts[NUM_GHOSTS]; 
                        ghosts[0].texture.loadFromFile("img/ghost1.png");
                        ghosts[1].texture.loadFromFile("img/ghost2.png");
                        ghosts[2].texture.loadFromFile("img/ghost3.png");
                        ghosts[3].texture.loadFromFile("img/ghost4.png");

                        ghosts[0].sprite.setTexture(ghosts[0].texture);
                        ghosts[1].sprite.setTexture(ghosts[1].texture);
                        ghosts[2].sprite.setTexture(ghosts[2].texture);
                        ghosts[3].sprite.setTexture(ghosts[3].texture);

                        ghosts[0].sprite.setScale(0.07,0.07);
                        ghosts[1].sprite.setScale(0.07,0.07);
                        ghosts[2].sprite.setScale(0.07,0.07);
                        ghosts[3].sprite.setScale(0.07,0.07);

                        ghosts[0].position.x=370; ghosts[0].position.y=370;
                        ghosts[1].position.x=420; ghosts[1].position.y=420;
                        ghosts[2].position.x=470; ghosts[2].position.y=370;
                        ghosts[3].position.x=300; ghosts[3].position.y=420;

                        ghosts[0].SetRenderWindow(&play);
                        ghosts[1].SetRenderWindow(&play);
                        ghosts[2].SetRenderWindow(&play);
                        ghosts[3].SetRenderWindow(&play);

                        pthread_t threads[NUM_GHOSTS];
                         for (int i = 0; i < NUM_GHOSTS; ++i) {
                            //ghosts[i].position.x= 250+10*(i+1);
                           pthread_create(&threads[i], nullptr, moveGhost, &ghosts[i]);
                        }

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
                            drawmaze(play);
                            pacman.Display();
                            ghosts[0].Display();
                            ghosts[1].Display();
                            ghosts[2].Display();
                            ghosts[3].Display();
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
