#include <iostream>
#include <algorithm>

#include <raylib.h>


#include <assetManager.hpp>
#include <dataManager.hpp>
#include <gameHandler.hpp>
#include <gameRenderer.hpp>

#include <globalVariables.hpp>

// Initialize global variables
const float ext_speed = 0.6f; // Movement speed
const float ext_friction = 0.985f; // Deceleration factor
const float ext_velocityThreshold = 0.05f; // Threshold to stop completely
const float ext_bounceDamping = 0.1f; // Remaing velocity after collision
const float ext_onGroundSlow = 0.8f; // Slow when moving on ground
const Vector2 ext_gravity{0.0f,0.4f}; // Gravity

const int startScreenWidth = 1200;
const int startScreenHeight = 900;

int main(){
    // Declaration ----------------------------------------------------------------------------------
    // Set configuration flags before initializing the window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    
    InitWindow(startScreenWidth, startScreenHeight, "LuFu_Drill");
    SetTargetFPS(60);

    // Access singleton classes
    AssetManager& assetManager = AssetManager::getInstance();
    DataManager& dataManager = DataManager::getInstance();


    GameHandler gameHandler;
    GameRenderer gameRenderer;

    // Gui Elements
    PlayerGui playerGui;
    FuelMenu fuelMenu;

    // Member variables for game state
    Player player;
    Grid mapGrid;
    
    // Initialization ----------------------------------------------------------------------------------
    assetManager.searchAssetsDirectoryPath("assets", 3);
    dataManager.searchDataDirectoryPath("data", 3);

    fuelMenu = FuelMenu({100,100}, {600,600});
    playerGui = PlayerGui({0,0}, {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())});
    fuelMenu.initialize();
    fuelMenu.enable();
    playerGui.initialize();
    playerGui.enable();

    player = Player({200,200}, {24,24}, {0,0});
    mapGrid = Grid(100,100, 32);
    gameHandler.generateTerrain(mapGrid);

    // Preload textures
    assetManager.preloadTexture("tile.png");

    gameRenderer.setCameraTarget(player);

    while (!WindowShouldClose()){
        // Logic ----------------------------------------------------------------------------------

        // Update and smooth camera movement
        gameRenderer.updateCameraOnWindowResize(player);
        gameRenderer.updateCamera(player);


        // Game Logic ------------------------------
        float deltaTime = GetFrameTime();

        // Movement input
        Vector2 direction{0,0};
        gameHandler.handleInput(direction);
        direction = Vector2Scale(direction, ext_speed); // Multiply by speed

        Vector2 frameVelocityChange{0,0};
        frameVelocityChange = direction + ext_gravity; // Update frames velocity change that will be added to the players velocity

        player.addVelocity(frameVelocityChange); // Add gravity and player input to the players velocity
        
        player.v = Vector2Scale(player.v, ext_friction); // Apply air friction

        // Stop completely if below the threshold
        if (Vector2Length(player.v) < ext_velocityThreshold) {
                player.v = { 0.0f, 0.0f };
        }

        // Physics collision response
        gameHandler.collisionResponse(player, mapGrid);

        // Clamp player to gridMap
        player.p.x = std::clamp(player.p.x, 0.f, static_cast<float>(mapGrid.sizeX * mapGrid.s - player.s.x));
        player.p.y = std::clamp(player.p.y, 0.f, static_cast<float>(mapGrid.sizeY * mapGrid.s - player.s.y));
        

        // Update Gui
        playerGui.update();
        fuelMenu.update();
        
        // Draw ----------------------------------------------------------------------------------
        BeginDrawing();
            // Clear Screen for the new render cycle
            ClearBackground(LIGHTGRAY);
            gameRenderer.renderGrid(mapGrid);
            gameRenderer.renderPlayer(player);

            fuelMenu.render();
            playerGui.render();
            
        EndDrawing();
    }

    // Cleanup resources and close raylib
    assetManager.cleanup();
    CloseWindow();
    return 0;
}