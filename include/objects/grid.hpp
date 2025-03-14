#pragma once

#include <vector>
#include <cassert>

#include <block.hpp>

class Grid
{
public:
    std::vector<Block> gridData;
public:
    size_t blockSize;
    size_t gridSizeX, gridSizeY;

    Grid(){}
    Grid(size_t x, size_t y, size_t bS) : gridSizeX(x), gridSizeY(y), gridData(x*y), blockSize(bS){}

    ~Grid(){}

    Block& operator() (size_t x, size_t y){
        assert(x < gridSizeX);
        assert(y < gridSizeY);
        return gridData[gridSizeX * y + x];
    }
};