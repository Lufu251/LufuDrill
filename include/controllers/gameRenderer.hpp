#pragma once

#include <raylib.h>
#include <raymath.h>

#include <player.hpp>
#include <grid.hpp>
#include <assetManager.hpp>
#include <dataManager.hpp>

class GameRenderer{
private:

public:
    // View
    Camera2D camera;

    GameRenderer(/* args */);
    ~GameRenderer();

    void setCameraOffset(const Vector2& offset);
    void moveCameraToPosition(const Vector2& position);
    void renderMapGrid(World& world);
    void renderMapBuildings();
    void renderPlayer();
};