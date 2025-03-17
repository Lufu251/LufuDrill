#pragma once

#include <aabb.hpp>
#include <guiContext.hpp>

class Building : public AABB{
    private:
    GuiContext* menuToTrigger;

public:
    Building(/* args */){}
    Building(const Vector2 rPosition, const Vector2 rSize, GuiContext& rMenuToTrigger) : AABB(rPosition, rSize), menuToTrigger(&rMenuToTrigger){}
    ~Building(){}

    void render(){
        DrawRectangleV(position, size, BLUE);
    }

    void checkTrigger(AABB& aabb){
        if(AABBIntersection(aabb, *this)){
            menuToTrigger->enable();
        }
    }
};