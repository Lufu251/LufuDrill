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
public:
    GameHandler(/* args */);
    ~GameHandler();

    // Helper methods
    void handleInput(Vector2& rDirection);
    void generateTerrain(Grid& grid);
    std::vector<AABB> getPossibleCollisionsFromGrid(AABB& player, Grid& grid);
    void collisionResponse(AABB& player, Grid& grid);
    void checkTouchingSides(AABB& player, Grid& grid);
};