#include <iostream>

#include <raylib.h>

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <gameHandler.hpp> 
#include <resourceManager.hpp>
#include <globalVariables.hpp>

// Initialize global variables
const float ext_speed = 1.5f; // Movement speed
const float ext_friction = 0.92f; // Deceleration factor
const float ext_velocityThreshold = 0.1f; // Threshold to stop completely
const float ext_bounceDamping = 0.8f; // Remaing velocity after collision
const float ext_onGroundSlow = 0.8f; // Slow when moving on ground
const Vector2 ext_gravity{0.0f,1.0f}; // Gravity

const int startScreenWidth = 1200;
const int startScreenHeight = 900;

int main(){

    std::cout << "nlohmann -----------------------------" << std::endl;
    std::vector<int> test = {0,1,2,3,4,5,6,7,8,9};
    json data;
    data["map"] = test;
    std::ofstream o("../../pretty.json");
    o << data;
    std::cout << data["map"] << std::endl;

    // Set configuration flags before initializing the window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    
    InitWindow(startScreenWidth, startScreenHeight, "LuFu_Drill");
    SetTargetFPS(60);
    
    printf("Working directory: %s\n", GetWorkingDirectory());

    // Access the ResourceManager instance
    ResourceManager& resourceManager = ResourceManager::getInstance();
    resourceManager.setAssetDirectoryPath("assets", 3);

    GameHandler gameHandler;

    // Preload textures
    resourceManager.preloadTexture("tile.png");
    //resourceManager.preloadTexture("/Roboto-Regular.ttf");
    
    
    // Initialization ----------------------------------------------------------------------------------
    gameHandler.initialize();

    while (!WindowShouldClose()){
        // Logic ----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime();
        gameHandler.update(deltaTime);

        // Draw ----------------------------------------------------------------------------------
        BeginDrawing();
            // Clear Screen for the new render cycle
            ClearBackground(BLACK);
            gameHandler.render();
            
        EndDrawing();
    }

    // Cleanup resources and close raylib
    resourceManager.cleanup();
    CloseWindow();
    return 0;
}