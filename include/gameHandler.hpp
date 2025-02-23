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

    // Game lifecycle methods
    void initialize();  // Initialize resources and game state
    void update(float deltaTime);  // Update game logic
    void render();  // Render game elements

private:
    // Member variables for game state
    // View
    Camera2D camera;
    Vector2 cameraTarget;
    float zoom;

    Player player;
    Grid mapGrid;
    PlayerGui playerGui;
    FuelMenu fuelMenu;

    // Helper methods
    void handleInput(Vector2& rDirection);
    void generateTerrain();
    std::vector<AABB> getPossibleCollisionsFromGrid(AABB& player, Grid& grid);
    int collisionResponse(AABB& player, Grid& grid);
};