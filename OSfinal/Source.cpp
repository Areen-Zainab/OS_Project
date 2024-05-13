#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>
#include <fstream>
#include <time.h>
#include "mainMenu.h"
#include "pacman.h"
#include "ghost.h"
#include <semaphore.h>
#include <pthread.h>
#include <vector>
#include <mutex>
using namespace sf;
using namespace std;
#include "score.cpp"

Pacman pacman;
bool leftKeyPressed = false; 
bool rightKeyPressed = false; 
bool upKeyPressed = false; 
bool downKeyPressed = false; 
int topScore=0, scoreCount=0, lives=3;

const int NUM_GHOSTS = 4;
const int WIDTH = 22;
const int HEIGHT = 20;
const int gsize = 35;
sem_t ghostMoveSemaphore;

int maze[HEIGHT][WIDTH] = { {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                            {1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1},
                            {1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                            {1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1},
                            {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},//6
                            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                            {1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
                            {1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1},
                            {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},//10
                            {1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1},
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

bool CheckPelletCollision(const Pacman& pacman) { //we have 256 pellets
    int pacmanX = static_cast<int>(pacman.position.x) / gsize;
    int pacmanY = static_cast<int>(pacman.position.y) / gsize-1;
    if (maze[pacmanY][pacmanX] == 0) {
        maze[pacmanY][pacmanX] = -1;
        return true;
    }
    return false;
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

bool gCheckWallCollision(const Ghost& ghost, float x, float y) {
  FloatRect ghostBox = ghost.sprite.getGlobalBounds();

  float newX = ghostBox.left + x;
  float newY = ghostBox.top + y;

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {

      if (maze[i][j] == 1) 
      { // shit yaar wall agai hai
        RectangleShape currentWall(Vector2f(30, 30));
        currentWall.setPosition(j * gsize + 30, i * gsize + 50);
        FloatRect wallBox = currentWall.getGlobalBounds();
        if (FloatRect(newX, newY, ghostBox.width, ghostBox.height).intersects(wallBox)) {
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
            dx = -5.0f;
        }
        else if (rightKeyPressed) {
           dx = 5.0f;
        }
         else if (upKeyPressed) {
            dy = -5.0f;
        }
         else if (downKeyPressed) {
             dy = 5.0f;
        }
         if (!CheckWallCollision(*pacman, dx, dy)) {
            // If not, move Pacman
            pacman->Move(dx, dy);
            pacman->wrapAround();
            if (CheckPelletCollision(*pacman)) {
                scoreCount++;
            }
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

bool CheckGhostCollision(const Pacman& pacman, const Ghost& ghost) {
    FloatRect pacmanBox = pacman.sprite.getGlobalBounds();
    FloatRect ghostBox = ghost.sprite.getGlobalBounds();
    if (pacmanBox.intersects(ghostBox)) {
        return true; // Collision occurred
    }
    return false; // No collision wow yayyy
}

bool CheckWallCollision(const Ghost& ghost, float x, float y) {
    FloatRect ghostBox = ghost.sprite.getGlobalBounds();

    float newX = ghostBox.left + x;
    float newY = ghostBox.top + y;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (maze[i][j] == 1) {
                RectangleShape currentWall(sf::Vector2f(30, 30));
                currentWall.setPosition(j * gsize + 30, i * gsize + 50);
                FloatRect wallBox = currentWall.getGlobalBounds();
                if (FloatRect(newX, newY, ghostBox.width, ghostBox.height).intersects(wallBox)) {
                    return true; // Collision detected
                }
            }
        }
    }

    return false; // No collision detected
}
#include <cmath>

// Function to calculate Euclidean distance between two points
float distance(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

pair<float, float> findClosestDirection(const Ghost& ghost, const Pacman& pacman) {
    float ghostX = ghost.position.x;
    float ghostY = ghost.position.y;
    float pacmanX = pacman.position.x;
    float pacmanY = pacman.position.y;

    // Calculate distances in each direction
    float distanceUp = distance(ghostX, ghostY - 1, pacmanX, pacmanY);
    float distanceDown = distance(ghostX, ghostY + 1, pacmanX, pacmanY);
    float distanceLeft = distance(ghostX - 1, ghostY, pacmanX, pacmanY);
    float distanceRight = distance(ghostX + 1, ghostY, pacmanX, pacmanY);

    // Find the minimum distance and return the corresponding direction
    if (distanceUp <= distanceDown && distanceUp <= distanceLeft && distanceUp <= distanceRight) {
        return make_pair(0, -1); // Move up
    } else if (distanceDown <= distanceLeft && distanceDown <= distanceRight) {
        return make_pair(0, 1); // Move down
    } else if (distanceLeft <= distanceRight) {
        return make_pair(-1, 0); // Move left
    } else {
        return make_pair(1, 0); // Move right
    }
}

void* moveGhost(void* arg) {
    Ghost* ghost = static_cast<Ghost*>(arg);
    while (true) {
        pair<float, float> direction = findClosestDirection(*ghost, pacman);
        float dx = direction.first;
        float dy = direction.second;
        if (!CheckWallCollision(*ghost, dx, dy))
        ghost->Move(dx,dy); // Move ghost randomly
        // Sleep for a short time to avoid busy waiting
        sleep(milliseconds(100));
    }
    return NULL;
}

/**
void* moveGhost(void* arg) {
  Ghost* ghost = static_cast<Ghost*>(arg);
  //Pacman* pacman = // Get Pacman object ;

  while (true) {
    sem_wait(&ghostMoveSemaphore); // Acquire semaphore before accessing board

    float x = pacman->position.x - ghost->position.x;
    float y = pacman->position.y - ghost->position.y;

    // Determine the direction to move closer to Pacman (avoiding large jumps)
    x = (abs(x) > gsize) ? (x > 0 ? -gsize : gsize) : x;
    y = (abs(y) > gsize) ? (y > 0 ? -gsize : gsize) : y;

    // Check for collision with walls (considering gameBoard)
    if (!gCheckWallCollision(*ghost, x, y)) {
             ghost->Move(x,y);  
    }

    sem_post(&ghostMoveSemaphore); // Release semaphore after calculations/movement

    sleep(milliseconds(100)); // Adjust delay as needed
  }

  return NULL;
}*/

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
    Font font;
    if (!font.loadFromFile("fonts/arial.ttf"))
    {
        cout << "error loading font" << endl;
    }

    Text scoress,topScoreText, scoreText,liveText, livee;
    scoress.setFont(font);
    topScoreText.setFont(font);
    scoreText.setFont(font);
    scoreText.setString("Your score: ");
    liveText.setFont(font);
    liveText.setString("Your lives: ");
    livee.setFont(font);

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
                        
                        pacman.SetRenderWindow(&play);
                        sem_init(&ghostMoveSemaphore, 0, 1);
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
                            // Check for collisions with each ghost
                            for (const auto& ghost : ghosts) {
                                if (CheckGhostCollision(pacman, ghost)) {
                                    lives--;
                                    pacman.position.x =90;
                                    pacman.position.y= 100;
                                }
                            }
                            //play.draw(menubg);
                            scoress.setString(int2Str(scoreCount));
                            scoress.setCharacterSize(20);
                            scoress.setFillColor(Color::Yellow);
                            scoress.setPosition(160, 20);
                            play.draw(scoress);
                            scoreText.setCharacterSize(20);
                            scoreText.setFillColor(Color::White);
                            scoreText.setPosition(50,20);
                            play.draw(scoreText);
                            liveText.setCharacterSize(20);
                            liveText.setPosition(600, 20);
                            liveText.setFillColor(Color::White);
                            play.draw(liveText);
                            livee.setString(int2Str(lives));
                            livee.setCharacterSize(20);
                            livee.setFillColor(Color::Red);
                            livee.setPosition(700,20);
                            play.draw(livee);
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
                    if (x == 2)    //high scores
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
                            topScoreText.setString(int2Str(topScore));
                            topScoreText.setCharacterSize(70);
                            topScoreText.setFillColor(Color::Red);
                            topScoreText.setPosition(400,400);
                            score.draw(topScoreText);
                            
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
