#pragma once

#include <raylib.h>
#include <raymath.h>

#include <equipment.hpp>
#include <aabb.hpp>

class Player : public AABB{
private:
    Drill drill;
    FuelTank fuelTank;
    Hull hull;
    Storage storage;
    Engine engine;

public:
    // If player is touching a block on any side
    bool top;
    bool right;
    bool bottom;
    bool left;

    Player() : AABB(){}
    Player(const Vector2 rPosition, const Vector2 rSize, const Vector2 rVelocity) : AABB(rPosition, rSize, rVelocity){}
    ~Player(){}

    void move(const Vector2& direction){
        p.x += direction.x;
        p.y += direction.y;
    }
    
    void addVelocity(const Vector2& rVelocity){
        v += rVelocity;
    }

    void updatePosition(){
        p += v;
    }

    void render(){
        DrawRectangleV(p, s, RED);
    }
};