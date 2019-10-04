#ifndef VELOCITY_H
#define VELOCITY_H


// Put your velocity class here, or overwrite this file with your own
// velocity class from the previous project
class Velocity
{
private:
    
    float dx;
    float dy;
    
public:
    
    //CONSTRUCTORS
    Velocity() {}
    Velocity(float dx, float dy) {};
    
    //SETTERS
    void setDx(float dx) { this->dx = dx; }
    void setDy(float dy) { this->dy = dy; }
    void setV(float newDx, float newDy);
    
    //GETTERS
    float getDx() const { return dx; }
    float getDy() const { return dy; }
    
};

#endif /* velocity_h */
