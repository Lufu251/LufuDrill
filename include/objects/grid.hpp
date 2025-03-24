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

// Convert Grid to JSON
inline void to_json(nlohmann::json& j, const Grid& g) {
    j = {
        {"gridSizeX", g.gridSizeX},
        {"gridSizeY", g.gridSizeY},
        {"gridData", g.gridData}
    };
}

// Convert JSON to Grid
inline void from_json(const nlohmann::json& j, Grid& g) {
    j.at("gridSizeX").get_to(g.gridSizeX);
    j.at("gridSizeY").get_to(g.gridSizeY);
    j.at("gridData").get_to(g.gridData);
}