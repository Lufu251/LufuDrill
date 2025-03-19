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

public:
    GameHandler(/* args */);
    ~GameHandler();

    // Helper methods
    void handleInput(Vector2& rDirection);
    void generateTerrain(World& world);
    void clampToGrid(AABB& box, World& world);
    std::vector<AABB> getPossibleCollisionsFromGrid(AABB& box, World& world);
    void checkCollisionAndMove(AABB& box, World& world);
    void checkTouching(AABB& box, World& world);
    void checkBuildingTriggers(AABB& box, World& world);
};