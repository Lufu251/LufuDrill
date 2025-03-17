#pragma once

#include <vector>

#include <raylib.h>
#include <raymath.h>

#include <aabb.hpp>
#include <player.hpp>
#include <grid.hpp>
#include <fuelMenu.hpp>
#include <playerGui.hpp>

class GameHandler
{
private:
    // Access singleton classes
    AssetManager& assetManager = AssetManager::getInstance();
    DataManager& dataManager = DataManager::getInstance();

public:
    GameHandler(/* args */);
    ~GameHandler();

    // Helper methods
    void handleInput(Vector2& rDirection);
    void generateTerrain(Grid& grid);
    void clampToGrid(AABB& box, Grid& grid);
    std::vector<AABB> getPossibleCollisionsFromGrid(AABB& box, Grid& grid);
    void checkCollisionAndMove(AABB& box, Grid& grid);
    void checkTouching(AABB& box, Grid& grid);
    void checkBuildingTriggers();
};