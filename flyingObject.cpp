#include "flyingObject.h"
#include <iostream>

// Put your FlyingObject method bodies here
void FlyingObject::advance()
{

    point.addX(getVelocity().getDx());
    point.addY(getVelocity().getDy());
    
}

void FlyingObject::wrapAround()
{
    
    if (getPoint().getX() > 200 || getPoint().getX() < 200)
    {
        setPoint(Point(-getPoint().getX(), getPoint().getY()));
    }
    
    if (getPoint().getY() > 200 || getPoint().getY() < 200)
    {
        setPoint(Point(getPoint().getX(), -getPoint().getY()));
    }
}

