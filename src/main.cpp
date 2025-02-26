#include <iostream>

#include <raylib.h>

#include <gameHandler.hpp>
#include <resourceManager.hpp>
#include <gameRenderer.hpp>

#include <globalVariables.hpp>

// Initialize global variables
const float ext_speed = 1.5f; // Movement speed
const float ext_friction = 0.93f; // Deceleration factor
const float ext_velocityThreshold = 0.1f; // Threshold to stop completely
const float ext_bounceDamping = 0.1f; // Remaing velocity after collision
const float ext_onGroundSlow = 0.8f; // Slow when moving on ground
const Vector2 ext_gravity{0.0f,0.98f}; // Gravity

const int startScreenWidth = 1200;
const int startScreenHeight = 900;

int main(){
    // Set configuration flags before initializing the window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    
    InitWindow(startScreenWidth, startScreenHeight, "LuFu_Drill");
    SetTargetFPS(60);

    // Access the ResourceManager instance
    ResourceManager& resourceManager = ResourceManager::getInstance();
    GameHandler gameHandler;
    GameRenderer gameRenderer;

    // Gui Elements
    PlayerGui playerGui;
    FuelMenu fuelMenu;
    
    // Initialization ----------------------------------------------------------------------------------
    resourceManager.setAssetDirectoryPath("assets", 3);
    //resourceManager.preloadTexture("/Roboto-Regular.ttf");

    fuelMenu = FuelMenu({100,100}, {600,600});
    playerGui = PlayerGui({0,0}, {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())});
    fuelMenu.initialize();
    fuelMenu.enable();
    playerGui.initialize();
    playerGui.enable();

    // Preload textures
    resourceManager.preloadTexture("tile.png");

    gameHandler.initialize();

    gameRenderer.setCameraTarget(gameHandler.player);

    while (!WindowShouldClose()){
        // Logic ----------------------------------------------------------------------------------

        // Update and smooth camera movement
        gameRenderer.updateCameraOnWindowResize(gameHandler.player);
        gameRenderer.updateCamera(gameHandler.player);


        // Game Logic ------------------------------
        float deltaTime = GetFrameTime();
        gameHandler.update(deltaTime);

        playerGui.update();
        fuelMenu.update();
        
        // Draw ----------------------------------------------------------------------------------
        BeginDrawing();
            // Clear Screen for the new render cycle
            ClearBackground(BLACK);
            gameRenderer.renderGrid(gameHandler.mapGrid);
            gameRenderer.renderPlayer(gameHandler.player);

            fuelMenu.render();
            playerGui.render();
            
        EndDrawing();
    }

    // Cleanup resources and close raylib
    resourceManager.cleanup();
    CloseWindow();
    return 0;
}