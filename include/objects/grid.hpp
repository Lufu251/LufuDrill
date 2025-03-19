#pragma once

#include <vector>
#include <cassert>

#include <nlohmann/json.hpp>

#include <block.hpp>
#include <buildings.hpp>

class Grid
{
public:
    std::vector<Block> gridData;
public:
    size_t gridSizeX, gridSizeY;

    Grid(){}
    Grid(size_t x, size_t y) : gridData(x*y), gridSizeX(x), gridSizeY(y){}

    ~Grid(){}

    Block& operator() (size_t x, size_t y){
        assert(x < gridSizeX);
        assert(y < gridSizeY);
        return gridData[gridSizeX * y + x];
    }
};