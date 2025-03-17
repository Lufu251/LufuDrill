#pragma once

#include <aabb.hpp>

class GasBuilding : AABB{
private:
    /* data */
public:
    GasBuilding(/* args */){}
    ~GasBuilding(){}

    void render(){
        DrawRectangleV(position, size, RED);
    }
};
