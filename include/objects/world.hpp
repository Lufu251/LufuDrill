#pragma once

#include <grid.hpp>

#include <vector>

#include <buildings.hpp>

class World {
private:
    
public:
    Grid mGrid;
    size_t mBlockSize;
    std::vector<Building> buildings;

    World(){}
    World(size_t x, size_t y, size_t blockSize): mGrid(x,y), mBlockSize(blockSize){}
    ~World(){}


};

// Convert World to JSON
inline void to_json(nlohmann::json& j, const World& w) {
    j = {
        {"blockSize", w.mBlockSize},
        {"buildings", w.buildings},
        {"grid", w.mGrid}
    };
}

// Convert World to Grid
inline void from_json(const nlohmann::json& j, World& w) {
    j.at("blockSize").get_to(w.mBlockSize);
    j.at("buildings").get_to(w.buildings);
    j.at("grid").get_to(w.mGrid);
}