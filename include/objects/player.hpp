#pragma once

#include <raylib.h>
#include <raymath.h>

#include <tools.hpp>
#include <aabb.hpp>

enum FACING{LEFT, RIGHT, DOWN};

class DrillUnit : public AABB{
public:
    Drill drill;
    GasTank gasTank;
    Hull hull;
    CargoBay cargoBay;
    Engine engine;

    // Booleans to track players state
    bool drilling = false;
    int facing = RIGHT;

    // Values to track touching sides
    int top, right, bottom, left;

    DrillUnit() : AABB(){}
    DrillUnit(const Vector2 rPosition, const Vector2 rSize, const Vector2 rVelocity) : AABB(rPosition, rSize, rVelocity){}
    ~DrillUnit(){}
    
    void addForce(const Vector2& rVelocity){
        velocity += rVelocity;
    }

    void updatePosition(){
        position += velocity;
    }
};