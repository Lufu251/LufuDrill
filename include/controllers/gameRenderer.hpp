#pragma once

#include <raylib.h>
#include <raymath.h>

#include <player.hpp>
#include <grid.hpp>
#include <assetManager.hpp>
#include <dataManager.hpp>

class GameRenderer
{
private:
    // Access singleton classes
    AssetManager& assetManager = AssetManager::getInstance();
    DataManager& dataManager = DataManager::getInstance();

public:
    // View
    Camera2D camera;

    GameRenderer(/* args */);
    ~GameRenderer();

    void setCameraOffset(const Vector2& offset);
    void moveCameraToPlayer();
    void renderGrid();
    void renderBuildings();
    void renderPlayer();
};