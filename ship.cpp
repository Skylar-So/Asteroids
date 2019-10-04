#include "ship.h"

/******************************************
 * SHIP : Apply Thrust
 * applys the thrust in the current
 * directon of the ship
 *****************************************/
void Ship::applyThrust(bool isForward)
{
    float thrust = (isForward) ? THRUST_AMOUNT : -THRUST_AMOUNT;
    
    Velocity v = this->getVelocity();
    v.setDx(this->getAngle().getXDir() * thrust);
    v.setDy(this->getAngle().getYDir() * thrust);
    this->setVelocity(v);
}

/******************************************
 * SHIP : Apply Rocation (public)
 * applys applys rotation to the ship
 * in either the right or left direction
 *****************************************/
void Ship::applyRotation(bool isRight)
{
    if (isRight)
    {
        applyRotation(-ROTATE_AMOUNT);
    }
    else
    {
        applyRotation(ROTATE_AMOUNT);
    }
}

/******************************************
 * SHIP : Apply Rocation 
 * sets the new angle of the ship, based
 * on the rotation amount
 *****************************************/
void Ship::applyRotation(int rotation)
{
    angle.setAngle(rotation + angle.getAngle());
}
