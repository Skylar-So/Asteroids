#ifndef bullet_h
#define bullet_h

#define BULLET_LIFE 40



// Put your Bullet class here
#include "flyingObject.h"
#include "point.h"
#include "uiDraw.h"


class Bullet : public FlyingObject
{
public:
    Bullet() {};
    
    void fire(Point point, float angle);
    void draw();
    void advance();
    
    
    
private:
    float angle;
    
    
};

#endif /* bullet_h */
