#ifndef flyingObject_h
#define flyingObject_h


// Put your FlyingObject class here
#include <iostream>
#include "point.h"
#include "uiDraw.h"
#include "velocity.h"

class FlyingObject
{
protected:
    Point point;
    Velocity velocity;
    bool alive;
  
    
public:
    // Constructors
    FlyingObject() : point(Point()), velocity(Velocity()), alive(true) {}
    
    // Getters
    Point getPoint() const { return this->point; }
    Velocity getVelocity() const { return this->velocity; }
    bool isAlive() { return this->alive; }
    
    // Setters
    void setPoint(Point point) { this->point = point; }
    void setVelocity(Velocity velocity) { this->velocity = velocity; }
    
    // Other functions
    void kill() { alive = false; };
    void virtual advance();
    void virtual draw() { drawDot(this->getPoint()); };
    void virtual wrapAround();

};



#endif /* flyingObject_h */
