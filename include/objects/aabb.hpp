#pragma once

#include <limits>
#include <vector>

#include <raylib.h>
#include <raymath.h>


class AABB{
private:
    /* data */
public:
    Vector2 p;
    Vector2 s;
    Vector2 v;

    // Zero means not touching. Any other numbers show for how many frames it is touching
    uint top, right, bottom, left;


    AABB(/* args */) {}
    AABB(const Vector2 rPosition, const Vector2 rSize) : p(rPosition), s(rSize){}
    AABB(const Vector2 rPosition, const Vector2 rSize, const Vector2 rVelocity) : p(rPosition), s(rSize), v(rVelocity){}
    ~AABB(){}

    Vector2 getGridPosition(size_t& rBlockSize){
         return (p + s /2) / rBlockSize;
    }
};

class Hit{
private:

public:
    Vector2 p;
    Vector2 n;
    float collisionTime;
    
    Hit(){}
    Hit(Vector2 position, float time, Vector2 normal) : p(position), collisionTime(time), n(normal) {}
};

// Returns a hit from two AABB's if below 1 a hit occured
inline Hit sweptAABB(const AABB& b1, const AABB& b2){
    Hit hit({0.0f, 0.0f}, 1.0f, {0.0f, 0.0f});

    float xInvEntry, yInvEntry;
    float xInvExit, yInvExit;
    
    // find the distance between the objects on the near and far sides for both x and y 
    if (b1.v.x > 0.0f){
        xInvEntry = b2.p.x - (b1.p.x + b1.s.x);
        xInvExit = (b2.p.x + b2.s.x) - b1.p.x;
    }
    else{
        xInvEntry = (b2.p.x + b2.s.x) - b1.p.x;
        xInvExit = b2.p.x - (b1.p.x + b1.s.x);
    }

    if (b1.v.y > 0.0f){
        yInvEntry = b2.p.y - (b1.p.y + b1.s.y);
        yInvExit = (b2.p.y + b2.s.y) - b1.p.y;
    }
    else{
        yInvEntry = (b2.p.y + b2.s.y) - b1.p.y;
        yInvExit = b2.p.y - (b1.p.y + b1.s.y);
    }
    
    // find time of collision and time of leaving for each axis (if statement is to prevent divide by zero)
    float xEntry, yEntry;
    float xExit, yExit;

    if (b1.v.x == 0.0f){ 
    xEntry = -std::numeric_limits<float>::infinity();
    xExit = std::numeric_limits<float>::infinity();
    }
    else{
        xEntry = xInvEntry / b1.v.x;
        xExit = xInvExit / b1.v.x;
    }

    if (b1.v.y == 0.0f) {
        yEntry = -std::numeric_limits<float>::infinity();
        yExit = std::numeric_limits<float>::infinity();
    }
    else{
        yEntry = yInvEntry / b1.v.y;
        yExit = yInvExit / b1.v.y;
    }

    // find the earliest/latest times of collision
    float entryTime = std::max(xEntry, yEntry);
    float exitTime = std::min(xExit, yExit);

    /*
    std::cout << std::endl;
    std::cout << "Player" << std::endl;
    std::cout << "Position X " << b1.p.x << std::endl;
    std::cout << "Position Y " << b1.p.y << std::endl;
    std::cout << "Velocity X  " << b1.v.x << std::endl;
    std::cout << "Velocity Y " << b1.v.y << std::endl;
    std::cout << std::endl;
    std::cout << "xInvEntry " << xInvEntry << std::endl;
    std::cout << "yInvEntry " << yInvEntry << std::endl;
    std::cout << "xInvExit " << xInvExit << std::endl;
    std::cout << "yInvExit " << yInvExit << std::endl;
    std::cout << "xEntry " << xEntry << std::endl;
    std::cout << "yEntry " << yEntry << std::endl;
    std::cout << "xExit " << xExit << std::endl;
    std::cout << "yExit " << yExit << std::endl;

    std::cout << "entryTime " << entryTime << std::endl;
    std::cout << "exitTime " << exitTime << std::endl;
    */

    // if there was no collision
    if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f || xEntry > 1.0f || yEntry > 1.0f) { 
        hit.n.x = 0.0f;
        hit.n.y = 0.0f;
        hit.collisionTime = 1.0f;
        return hit;
    }
    else{
        // if there was a collision
        // calculate normal of collided surface
        if (xEntry > yEntry){
            if (xInvEntry < 0.0f){
                // RIGHT
                hit.n.x = 1.0f;
                hit.n.y = 0.0f;
            }
            else{
                // LEFT
                hit.n.x = -1.0f;
                hit.n.y = 0.0f;
            }
        }
        else{
            if (yInvEntry < 0.0f){
                // UP
                hit.n.x = 0.0f;
                hit.n.y = 1.0f;
            } 
            else{
                // DOWN
                hit.n.x = 0.0f;
                hit.n.y = -1.0f;
            }
        }
        // Set hit to the correct values
        hit.collisionTime = entryTime;
        hit.p = b1.p + b1.v * entryTime;

        // return the time of collision
        return hit;
    }
}

// Checks for a regular AABB to AABB collision
inline bool AABBCheck(AABB b1, AABB b2){ 
  return !(b1.p.x + b1.s.x < b2.p.x || b1.p.x > b2.p.x + b2.s.x || b1.p.y + b1.s.y < b2.p.y || b1.p.y > b2.p.y + b2.s.y); 
}

// Returns the broadphasebox from a AABB
inline AABB GetSweptBroadphaseBox(AABB b1){ 
  AABB broadphasebox;
  broadphasebox.p.x = b1.v.x > 0 ? b1.p.x : b1.p.x + b1.v.x;
  broadphasebox.p.y = b1.v.y > 0 ? b1.p.y : b1.p.y + b1.v.y;
  broadphasebox.s.x = b1.v.x > 0 ? b1.v.x + b1.s.x : b1.s.x - b1.v.x;
  broadphasebox.s.y = b1.v.y > 0 ? b1.v.y + b1.s.y : b1.s.y - b1.v.y;

  return broadphasebox;
}

// Returns the nearest collision from a list of AABB's
inline Hit GetNearestCollisionFromList(AABB& player, std::vector<AABB>& blocks){
    Hit hit({0.0f, 0.0f}, 1.0f, {0.0f, 0.0f}); // Set collision time to max

    // Iterate the list of possible vectors
    for(auto b : blocks){
        // Broad-Phasing with standard AABB check
        AABB broadphase = GetSweptBroadphaseBox(player); // create the broadphase AABB from the player position and velocity
        if(AABBCheck(broadphase, b)){
            // Hit is very likely
            // Do a swept AABB test and get distance to collisionpoint
            Hit possibleHit = sweptAABB(player, b);

            // Check if calculated hit time is shorter and update if it is
            if(possibleHit.collisionTime < hit.collisionTime){
                // Collision and it is the nearest so far. Update values
                //std::cout << "Is nearest hit: " << iBlock << " " << jBlock << " " << collisionTime << std::endl;
                hit = possibleHit;
            }
            else{
                // Collision but it is not the nearest. No values updated
                //std::cout << "Is not nearest hit: " << iBlock << " " << jBlock << " " << collisionTime << std::endl;
            }
        }
    }
    return hit;
}