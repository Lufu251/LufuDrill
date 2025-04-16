#pragma once

#include <limits>
#include <vector>

#include <raylib.h>
#include <raymath.h>


class AABB{
private:
    /* data */
public:
    Vector2 mPosition;
    Vector2 mSize;
    Vector2 mVelocity = {0,0};
    float mMass;

    AABB() {}
    AABB(const Vector2 position, const Vector2 size) : mPosition(position), mSize(size){}
    AABB(const Vector2 position, const Vector2 size, const float mass) : mPosition(position), mSize(size), mMass(mass){}
    virtual ~AABB(){}

    // Divides by mass
    void addForce(const Vector2& force){
        mVelocity += force / mMass;
    }

    // Does not divied by mass
    void addAcceleration(const Vector2& acceleration){
        mVelocity += acceleration;
    }

    void updatePosition(){
        mPosition += mVelocity;
    }

    Vector2 getGridPosition(size_t& rBlockSize){
        return (mPosition + mSize /2) / rBlockSize;
   }
};

class Hit{
private:

public:
    Vector2 p;
    Vector2 n;
    float collisionTime;
    
    Hit(){}
    Hit(Vector2 position, float time, Vector2 normal) : p(position), n(normal), collisionTime(time) {}
};

// Returns a hit from two AABB's if below 1 a hit occured
inline Hit sweptAABB(const AABB& b1, const AABB& b2){
    Hit hit({0.0f, 0.0f}, 1.0f, {0.0f, 0.0f});

    float xInvEntry, yInvEntry;
    float xInvExit, yInvExit;
    
    // find the distance between the objects on the near and far sides for both x and y 
    if (b1.mVelocity.x > 0.0f){
        xInvEntry = b2.mPosition.x - (b1.mPosition.x + b1.mSize.x);
        xInvExit = (b2.mPosition.x + b2.mSize.x) - b1.mPosition.x;
    }
    else{
        xInvEntry = (b2.mPosition.x + b2.mSize.x) - b1.mPosition.x;
        xInvExit = b2.mPosition.x - (b1.mPosition.x + b1.mSize.x);
    }

    if (b1.mVelocity.y > 0.0f){
        yInvEntry = b2.mPosition.y - (b1.mPosition.y + b1.mSize.y);
        yInvExit = (b2.mPosition.y + b2.mSize.y) - b1.mPosition.y;
    }
    else{
        yInvEntry = (b2.mPosition.y + b2.mSize.y) - b1.mPosition.y;
        yInvExit = b2.mPosition.y - (b1.mPosition.y + b1.mSize.y);
    }
    
    // find time of collision and time of leaving for each axis (if statement is to prevent divide by zero)
    float xEntry, yEntry;
    float xExit, yExit;

    if (b1.mVelocity.x == 0.0f){ 
    xEntry = -std::numeric_limits<float>::infinity();
    xExit = std::numeric_limits<float>::infinity();
    }
    else{
        xEntry = xInvEntry / b1.mVelocity.x;
        xExit = xInvExit / b1.mVelocity.x;
    }

    if (b1.mVelocity.y == 0.0f) {
        yEntry = -std::numeric_limits<float>::infinity();
        yExit = std::numeric_limits<float>::infinity();
    }
    else{
        yEntry = yInvEntry / b1.mVelocity.y;
        yExit = yInvExit / b1.mVelocity.y;
    }

    // find the earliest/latest times of collision
    float entryTime = std::max(xEntry, yEntry);
    float exitTime = std::min(xExit, yExit);

    // if there was no collision
    if (entryTime > exitTime || (xEntry < 0.0f && yEntry < 0.0f) || xEntry > 1.0f || yEntry > 1.0f) { 
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
        hit.p = b1.mPosition + b1.mVelocity * entryTime;

        // return the time of collision
        return hit;
    }
}

// Checks for a regular AABB to AABB collision
inline bool AABBIntersection(AABB b1, AABB b2){
  return !(b1.mPosition.x + b1.mSize.x < b2.mPosition.x || b1.mPosition.x > b2.mPosition.x + b2.mSize.x || b1.mPosition.y + b1.mSize.y < b2.mPosition.y || b1.mPosition.y > b2.mPosition.y + b2.mSize.y); 
}

// Returns the broadphasebox from a AABB
inline AABB GetSweptBroadphaseBox(AABB b1){
  AABB broadphasebox;
  broadphasebox.mPosition.x = b1.mVelocity.x > 0 ? b1.mPosition.x : b1.mPosition.x + b1.mVelocity.x;
  broadphasebox.mPosition.y = b1.mVelocity.y > 0 ? b1.mPosition.y : b1.mPosition.y + b1.mVelocity.y;
  broadphasebox.mSize.x = b1.mVelocity.x > 0 ? b1.mVelocity.x + b1.mSize.x : b1.mSize.x - b1.mVelocity.x;
  broadphasebox.mSize.y = b1.mVelocity.y > 0 ? b1.mVelocity.y + b1.mSize.y : b1.mSize.y - b1.mVelocity.y;

  return broadphasebox;
}

// Returns the nearest collision from a list of AABB's
inline Hit GetNearestCollisionFromList(AABB& box, std::vector<AABB>& blocks){
    Hit hit({0.0f, 0.0f}, 1.0f, {0.0f, 0.0f}); // Set collision time to max

    // Iterate the list of possible vectors
    for(auto b : blocks){
        // Broad-Phasing with standard AABB check
        AABB broadphase = GetSweptBroadphaseBox(box); // create the broadphase AABB from the player position and velocity
        if(AABBIntersection(broadphase, b)){
            // Hit is very likely
            // Do a swept AABB test and get distance to collisionpoint
            Hit possibleHit = sweptAABB(box, b);

            // Check if calculated hit time is shorter and update if it is
            if(possibleHit.collisionTime < hit.collisionTime){
                // Collision and it is the nearest so far. Update values
                //std::cout << "Is nearest hit: " << iBlock << " " << jBlock << " " << collisionTime << std::endl;
                hit = possibleHit;
            }
        }
    }
    return hit;
}