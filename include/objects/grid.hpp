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
    size_t blockSize;
    size_t gridSizeX, gridSizeY;

    Grid(){}
    Grid(size_t x, size_t y, size_t bS) : gridData(x*y), blockSize(bS), gridSizeX(x), gridSizeY(y){}

    ~Grid(){}

    Block& operator() (size_t x, size_t y){
        assert(x < gridSizeX);
        assert(y < gridSizeY);
        return gridData[gridSizeX * y + x];
    }
};

// Convert Block to JSON
inline void to_json(nlohmann::json& j, const Block& b) {
    j = {
        {"type", b.mType}
    };
}

// Convert JSON to Block
inline void from_json(const nlohmann::json& j, Block& b) {
    size_t type;
    j.at("type").get_to(type);
    b = Block(type);
}

// Convert Grid to JSON
inline void to_json(nlohmann::json& j, const Grid& g) {
    j = {
        {"gridSizeX", g.gridSizeX},
        {"gridSizeY", g.gridSizeY},
        {"blockSize", g.blockSize},
        {"gridData", g.gridData}
    };
}

// Convert JSON to Grid
inline void from_json(const nlohmann::json& j, Grid& g) {
    j.at("gridSizeX").get_to(g.gridSizeX);
    j.at("gridSizeY").get_to(g.gridSizeY);
    j.at("blockSize").get_to(g.blockSize);
    j.at("gridData").get_to(g.gridData);
}
