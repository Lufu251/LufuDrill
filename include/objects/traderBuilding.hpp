#pragma once

#include <aabb.hpp>

class TraderBuilding : AABB{
private:
    /* data */
public:
    TraderBuilding(/* args */){}
    ~TraderBuilding(){}

    void render(){
        DrawRectangleV(position, size, BLUE);
    }
};