#pragma once

#include <raylib.h>
#include <raymath.h>

#include <player.hpp>
#include <grid.hpp>

class GameRenderer
{
private:

public:
    // View
    Camera2D camera;

    GameRenderer(/* args */);
    ~GameRenderer();

    void setCameraOffset(Vector2 offset);
    void moveCameraToPlayer(Player& player);
    void renderGrid(Grid& mapGrid);
    void renderPlayer(Player& player);

};