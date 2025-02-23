#pragma once

#include <vector>

#include <block.hpp>

class Grid
{
private:
    std::vector<Block> gridData;
public:
    size_t s;
    size_t sizeX, sizeY;

    Grid(){}
    Grid(size_t x, size_t y, size_t blockSize) : sizeX(x), sizeY(y), gridData(x*y), s(blockSize){}

    ~Grid(){}

    Block& operator() (size_t x, size_t y){
        return gridData[sizeX * y + x];
    }
};