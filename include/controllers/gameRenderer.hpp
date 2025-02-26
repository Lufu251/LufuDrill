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
    Vector2 cameraTarget;
    float zoom;

    GameRenderer(/* args */);
    ~GameRenderer();

    void setCameraTarget(Player& player);
    void updateCamera(Player& player);
    void updateCameraOnWindowResize(Player& player);
    void renderGrid(Grid& mapGrid);
    void renderPlayer(Player& player);

};