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

    float mAirDensity;
    Vector2 mGravity;

    World(){}
    World(size_t x, size_t y, size_t blockSize): mGrid(x,y), mBlockSize(blockSize){}
    ~World(){}


};

// Convert World to JSON
inline void to_json(nlohmann::json& j, const World& w) {
    j = {
        {"blockSize", w.mBlockSize},
        {"airDensity", w.mAirDensity},
        {"gravity", w.mGravity},
        {"buildings", w.buildings},
        {"grid", w.mGrid}
    };
}

// Convert World to Grid
inline void from_json(const nlohmann::json& j, World& w) {
    j.at("blockSize").get_to(w.mBlockSize);
    j.at("airDensity").get_to(w.mAirDensity);
    j.at("gravity").get_to(w.mGravity);
    j.at("buildings").get_to(w.buildings);
    j.at("grid").get_to(w.mGrid);
}