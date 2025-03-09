#pragma once

#include <raylib.h>
#include <raymath.h>

#include <tools.hpp>
#include <aabb.hpp>

class Player : public AABB{
private:
    Drill drill;
    FuelTank fuelTank;
    Hull hull;
    CargoBay cargoBay;
    Engine engine;

public:
    Player() : AABB(){}
    Player(const Vector2 rPosition, const Vector2 rSize, const Vector2 rVelocity) : AABB(rPosition, rSize, rVelocity){}
    ~Player(){}
    
    void addForce(const Vector2& rVelocity){
        velocity += rVelocity;
    }

    void updatePosition(){
        position += velocity;
    }

    void render(){
        DrawRectangleV(position, size, RED);
    }
};