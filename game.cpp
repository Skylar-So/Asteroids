/*********************************************************************
 * File: game.cpp
 * Description: Contains the implementaiton of the game class
 *  methods.
 *
 *********************************************************************/

#include "game.h"
#include "flyingObject.h"

// These are needed for the getClosestDistance function...
#include <limits>
#include <algorithm>
using namespace std;
#include <vector>
#define OFF_SCREEN_BORDER_AMOUNT 5

/******************************************
 * BULLET : Constructor
 * Takes the top left and bottom right to
 * construct the astroids game
 *****************************************/
Game::Game(Point tl, Point br) : topLeft(tl), bottomRight(br)
{
    //Set the conditions of the game.
    initializeVariables();
    // Create 5 large astroids, not within 25 px of center
    initializeRocks();
    
}

/******************************************
 * Function: initialize Rocks
 * Description: Creates 5 random big rocks
 *****************************************/
void Game::initializeRocks()
{
    int i = 0;
    while (i < 5)
    {
        Point p = Point(random(-200, 200), random(-200, 200));
        //cout << "Point " << i << " : " << p << endl;
        if ((p.getX() < -25 || p.getX() < 25) && (p.getY() < -25 || p.getY() > 25))
        {
            rocks.push_back(new BigRock(p));
            i++;
        }
    }
}

/******************************************
 * Function: initialize variables
 * Description: sets all vars to initial
 *               state
 *****************************************/
void Game::initializeVariables()
{
    score = 0;
    lives = AMOUNT_OF_LIVES;
    hasWon = false;
}

/******************************************
 * Function: Advance
 * Description: Advances the game by
 * a single frame
 *****************************************/
void Game::advance()
{
    if (!ship.isAlive() && !hasWon)
    {
        ship.reset(Point());
        lives--;
    }
    else if (ship.isAlive() && lives > 0 && !hasWon)
    {
        advanceRocks();
        advanceBullets();
        advanceShip();

        cleanUpZombies();
        handleCollisions();
    }
    
    if (rocks.size() <= 0)
        hasWon = true;
    
}

/******************************************
 * Function: Advance Bullets
 * Description: Advances each bullet by
 * a single frame
 *****************************************/
void Game::advanceBullets()
{
    for (int i = 0; i < bullets.size(); i++)
    {
        if (bullets[i].isAlive())
        {
            // this bullet is alive, so tell it to move forward
            bullets[i].advance();
            
            if (!isOnScreen(bullets[i].getPoint()))
            {
                // the bullet has left the screen
               // bullets[i].kill();
                bullets[i].wrapAround();
            }
            
        }
        
    }
}

/******************************************
 * Function: Advance Rocks
 * Description: Advances each rock by
 * a single frame
 *****************************************/
void Game::advanceRocks()
{
    for (vector<Rock*> ::iterator it = rocks.begin(); it != rocks.end(); it++)
    {
        (*it)->advance();
    
        if (!isOnScreen((*it)->getPoint()))
        {
          (*it)->wrapAround();
        }
    }
}

/******************************************
 * Function: Advance Ship
 * Description: Advances the ship by
 * a single frame
 *****************************************/
void Game::advanceShip()
{
    ship.advance();
    if (!isOnScreen(ship.getPoint()))
    {
        ship.wrapAround();
    }
}

/******************************************
 * Function: getClosestDistance
 * Description: Determine how close these
 * two objects will get in between the
 * frames.
 *****************************************/
float Game :: getClosestDistance(const FlyingObject &obj1, const FlyingObject &obj2) const
{
    // find the maximum distance traveled
    float dMax = max(abs(obj1.getVelocity().getDx()), abs(obj1.getVelocity().getDy()));
    dMax = max(dMax, abs(obj2.getVelocity().getDx()));
    dMax = max(dMax, abs(obj2.getVelocity().getDy()));
    dMax = max(dMax, 0.1f); // when dx and dy are 0.0. Go through the loop once.
    
    float distMin = std::numeric_limits<float>::max();
    for (float i = 0.0; i <= dMax; i++)
    {
        Point point1(obj1.getPoint().getX() + (obj1.getVelocity().getDx() * i / dMax),
                     obj1.getPoint().getY() + (obj1.getVelocity().getDy() * i / dMax));
        Point point2(obj2.getPoint().getX() + (obj2.getVelocity().getDx() * i / dMax),
                     obj2.getPoint().getY() + (obj2.getVelocity().getDy() * i / dMax));
        
        float xDiff = point1.getX() - point2.getX();
        float yDiff = point1.getY() - point2.getY();
        
        float distSquared = (xDiff * xDiff) +(yDiff * yDiff);
        
        distMin = min(distMin, distSquared);
    }
    
    return sqrt(distMin);
}

/******************************************
 * Function: Clean Up Zombies
 * Description: Cleans up all the dead
 * objects that are hanging around.
 *****************************************/
void Game::cleanUpZombies()
{
    cleanUpBullets(false);
    cleanUpRocks(false);
}


/******************************************
 * Function: Clean Up Bullets
 * Description: Cleans up each bullet
 * if dead
 *****************************************/
void Game::cleanUpBullets(bool deleteAll)
{
    vector<Bullet>::iterator bulletIt = bullets.begin();
    while (bulletIt != bullets.end())
    {
        Bullet bullet = *bulletIt;
        
        if (!bullet.isAlive() || deleteAll)
        {
    
            bulletIt = bullets.erase(bulletIt);
        }
        else
        {
            bulletIt++;
        }
    }
}

/******************************************
 * Function: Clean Up rocks
 * Description: Cleans up each rock
 * if dead
 *****************************************/
void Game::cleanUpRocks(bool deleteAll)
{
    vector<Rock*>::iterator rockIt = rocks.begin();
    while (rockIt != rocks.end())
    {
        Rock * rock = *rockIt;
        
        if (!rock->isAlive() || deleteAll)
        {
            delete (*rockIt);
            rockIt = rocks.erase(rockIt);
        }
        else
        {
            rockIt++;
        }
    }
}

/**************************************************************************
 * GAME :: IS ON SCREEN
 * Determines if a given point is on the screen.
 **************************************************************************/
bool Game :: isOnScreen(const Point & point)
{
    return (point.getX() >= topLeft.getX() - OFF_SCREEN_BORDER_AMOUNT
            && point.getX() <= bottomRight.getX() + OFF_SCREEN_BORDER_AMOUNT
            && point.getY() >= bottomRight.getY() - OFF_SCREEN_BORDER_AMOUNT
            && point.getY() <= topLeft.getY() + OFF_SCREEN_BORDER_AMOUNT);
}

/******************************************
 * Function: Handle Collisions
 * Description: Handles Object collisions
 *****************************************/
void Game::handleCollisions()
{
    handleBulletCollisions();
    handleShipCollisions();
}
/******************************************
 * Function: Handle Bullet Collisions
 * Description: Handles Bullet collisions
 *****************************************/
void Game::handleBulletCollisions()
{
    for (int i = 0; i < bullets.size(); i++)
    {
      //  Bullet * bullet = bullets[i];
        
        if (bullets[i].isAlive())
        {
            for (int j = 0; j < rocks.size(); j++)
            {
                Rock * rock = rocks[j];
                if (rock->isAlive())
                {
                    float rockRange = CLOSE_ENOUGH + rock->getSize();
                    
                    if (fabs(getClosestDistance(bullets[i], *rock)) < rockRange || bullets[i].getPoint().isClose(rock->getPoint(), rockRange))
                    {
                        vector<Rock*> newRocks = rock->destory();
                        rocks.insert(rocks.end(), newRocks.begin(), newRocks.end());
                        score += rock->getScore();
                        rock->kill();
                        bullets[i].kill();
                    }
                }
            }
        }
    }
}

void Game::handleShipCollisions()
{
    for (int i = 0; i < rocks.size(); i++)
    {
        Rock * rock = rocks[i];
        if (rock->isAlive())
        {
            float rockRange = CLOSE_ENOUGH + rock->getSize();
            
            if (fabs(getClosestDistance(ship, *rock)) < rockRange || ship.getPoint().isClose(rock->getPoint(), rockRange))
            {
                vector<Rock*> newRocks = rock->destory();
                rocks.insert(rocks.end(), newRocks.begin(), newRocks.end());
                rock->kill();
                ship.kill();
            }
        }
    }
}

/******************************************
 * Function: Handle Input
 * Description: Handles all input from the
 * user
 *****************************************/
void Game::handleInput(const Interface & ui)
{
    if (ship.isAlive() && lives > 0 && !hasWon)
    {
        if (ui.isUp())
        {
            ship.applyThrust(true);
        }
        if (ui.isDown())
        {
            ship.applyThrust(false);
        }
        if (ui.isLeft())
        {
            ship.applyRotation(false);
        }
        
        if (ui.isRight())
        {
            ship.applyRotation(true);
        }
        
        if (ui.isSpace())
        {
            Bullet newBullet;
            newBullet.fire(ship.getPoint(), ship.getRotation());
            
            bullets.push_back(newBullet);
        }
    }
    else
    {
        if (ui.isSpace())
        {
            this->reset();
        }
    }
    
}

/******************************************
 * Function: draw
 * Description: draws each object
 *****************************************/
void Game::draw(const Interface & ui)
{
    for (vector<Rock*> ::iterator it = rocks.begin(); it != rocks.end(); it++)
    {
        (*it)->draw();
    }
    
    // draw the Ship
    ship.draw();
    
    // draw the bullets, if they are alive
    for (int i = 0; i < bullets.size(); i++)
    {
        if (bullets[i].isAlive())
        {
            bullets[i].draw();
        }
    }
    
    drawScore();
    drawLives();
    
    if (lives < 1)
    {
        drawText(Point(-50, 25), "You Crashed!");
        drawText(Point(-45, -25), "Game Over!");
        drawText(Point(-65, -50), "Press Space to Reset.");
    }
    if (hasWon)
    {
        drawText(Point(-40, 25), "You Won!");
        drawText(Point(-65, -25), "Press Space to Reset.");
    }
    
    
}

/******************************************
 * Function: draw Score
 * Description: draws the score
 *****************************************/
void Game::drawScore()
{
    Point scoreLocation, scoreTextLocation;
    scoreLocation.setX(topLeft.getX() + 50);
    scoreLocation.setY(topLeft.getY() - 5);
    scoreTextLocation.setX(topLeft.getX() + 5);
    scoreTextLocation.setY(topLeft.getY() - 15);
    
    drawText(scoreTextLocation, "Score: ");
    drawNumber(scoreLocation, score);
}

/******************************************
 * Function: draw Lives
 * Description: draws the lives remaining
 *****************************************/
void Game::drawLives()
{
    Point scoreLocation, scoreTextLocation;
    scoreLocation.setX(bottomRight.getX() - 10);
    scoreLocation.setY(topLeft.getY() - 5);
    scoreTextLocation.setX(bottomRight.getX() - 55);
    scoreTextLocation.setY(topLeft.getY() - 15);
    
    drawText(scoreTextLocation, "Lives: ");
    drawNumber(scoreLocation, lives);
}

/******************************************
 * Function: Reset
 * Description: Resets the game
 *****************************************/
void Game::reset()
{
    initializeRocks();
    initializeVariables();
    cleanUpZombies();
    handleCollisions();
}
