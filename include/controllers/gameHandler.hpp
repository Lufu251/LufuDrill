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

    // Member variables for game state
    Player player;
    Grid mapGrid;

    // Game lifecycle methods
    void initialize();  // Initialize resources and game state
    void update(float deltaTime);  // Update game logic

private:
    // Helper methods
    void handleInput(Vector2& rDirection);
    void generateTerrain();
    std::vector<AABB> getPossibleCollisionsFromGrid(AABB& player, Grid& grid);
    void collisionResponse(AABB& player, Grid& grid);
};