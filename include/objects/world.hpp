#pragma once

#include <grid.hpp>

#include <vector>

#include <buildings.hpp>

class World
{
private:
    
public:
    Grid mGrid;
    size_t mBlockSize;
    std::vector<Building> buildings;

    World(){}
    World(size_t x, size_t y, size_t blockSize): mGrid(x,y), mBlockSize(blockSize){}
    ~World(){}


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
inline void to_json(nlohmann::json& j, const World& w) {
    j = {
        {"gridSizeX", w.mGrid.gridSizeX},
        {"gridSizeY", w.mGrid.gridSizeY},
        {"blockSize", w.mBlockSize},
        {"gridData", w.mGrid.gridData}
    };
}

// Convert JSON to Grid
inline void from_json(const nlohmann::json& j, World& w) {
    j.at("gridSizeX").get_to(w.mGrid.gridSizeX);
    j.at("gridSizeY").get_to(w.mGrid.gridSizeY);
    j.at("blockSize").get_to(w.mBlockSize);
    j.at("gridData").get_to(w.mGrid.gridData);
}