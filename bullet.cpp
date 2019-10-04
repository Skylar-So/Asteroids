#include "bullet.h"
#define BULLET_SPEED 10.0

/**********************************************
 * Function: fire()
 * Purpose: Takes and sets the bullet point and angle
 *********************************************/
void Bullet::fire(Point point, float angle)
{
    this->angle = angle;
    this->point = point;
}

/************************************************
 * Function: draw()
 * Purpose: draws the bullet
 **********************************************/
void Bullet::draw()
{
    drawDot(getPoint());
}


/******************************************
 * Function: Advance()
 * Purpose: advances the bullet on the screen
 ***************a******************************/
void Bullet::advance()
{
    
    velocity.setDx(BULLET_SPEED * (cos(M_PI / 180.0 * angle)));
    velocity.setDy(BULLET_SPEED * (sin(M_PI / 180.0 * angle)));
    
    FlyingObject::advance();
}
