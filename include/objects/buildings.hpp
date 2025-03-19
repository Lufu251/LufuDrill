#pragma once

#include <aabb.hpp>
#include <guiContext.hpp>

enum BUILDING_TYPE{GAS_STATION, TRADER, SHOP};

class Building : public AABB{
    private:
    GuiContext* menuToTrigger;
    

public:
    int mType;

    Building(/* args */){}
    Building(const int type, const Vector2 rPosition, const Vector2 rSize) : AABB(rPosition, rSize), mType(type){}
    ~Building(){}

    void setMenuToTrigger(GuiContext& rMenuToTrigger){
        menuToTrigger = &rMenuToTrigger;
    }

    void checkTrigger(AABB& aabb){
        if(AABBIntersection(aabb, *this)){
            menuToTrigger->enable();
        }
    }
};