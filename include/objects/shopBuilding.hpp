#pragma once

#include <aabb.hpp>

class ShopBuilding : AABB{
private:
    /* data */
public:
    ShopBuilding(/* args */){}
    ~ShopBuilding(){}

    void render(){
        DrawRectangleV(position, size, GREEN);
    }
};
