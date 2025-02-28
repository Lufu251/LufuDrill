#include <iostream>
#include <algorithm>

#include <raylib.h>


#include <assetManager.hpp>
#include <dataManager.hpp>
#include <gameHandler.hpp>
#include <gameRenderer.hpp>

#include <globalVariables.hpp>

// Initialize global variables
const float ext_velocityThreshold = 0.05f; // Threshold to stop completely
const float ext_bounceDamping = 0.1f; // Remaing velocity after collision
const float ext_onGroundSlow = 0.8f; // Slow when moving on ground

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

    // Preload textures
    assetManager.preloadTexture("tile.png");

    // Preload fonts
    assetManager.preloadFont("Roboto-Regular.ttf", 32);

    fuelMenu = FuelMenu({100,100}, {600,600});
    playerGui = PlayerGui({0,0}, {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())});
    fuelMenu.initialize();
    fuelMenu.disable();
    playerGui.initialize();
    playerGui.enable();

    player = Player({200,200}, {24,24}, {0,0});
    mapGrid = Grid(100,100, 32);
    gameHandler.generateTerrain(mapGrid);

    gameRenderer.setCameraTarget(player);

    while (!WindowShouldClose()){
        // Game Logic ------------------------------
        float deltaTime = GetFrameTime();

        // Update and smooth camera movement
        gameRenderer.updateCameraOnWindowResize(player);
        gameRenderer.updateCamera(player);

        // Movement input
        Vector2 movementInput{0,0};
        gameHandler.handleInput(movementInput);
        movementInput = Vector2Scale(movementInput, dataManager.movementSpeed); // Multiply by speed

        // Calculate air resistance
        Vector2 airResistance = player.v - player.v * dataManager.airResistance;

        player.addForce(movementInput); // Add movementInput to player velocity
        player.addForce(dataManager.gravity); // Add gravity to player velocity
        player.addForce(Vector2Negate(airResistance)); // Add airResistance to player velocity
        

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