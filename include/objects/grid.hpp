#pragma once

#include <vector>
#include <cassert>

#include <block.hpp>

class Grid
{
private:
    std::vector<Block> gridData;
public:
    size_t blockSize;
    size_t sizeX, sizeY;

    Grid(){}
    Grid(size_t x, size_t y, size_t bS) : sizeX(x), sizeY(y), gridData(x*y), blockSize(bS){}

    ~Grid(){}

    Block& operator() (size_t x, size_t y){
        assert(x < sizeX);
        assert(y < sizeY);
        return gridData[sizeX * y + x];
    }
};