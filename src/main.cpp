#include <iostream>
#include <algorithm>

#include <raylib.h>


#include <assetManager.hpp>
#include <dataManager.hpp>
#include <gameHandler.hpp>
#include <gameRenderer.hpp>


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
        // Logic ----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime();

        // Update and smooth camera movement
        gameRenderer.updateCameraOnWindowResize(player);
        gameRenderer.updateCamera(player);

        // Movement input
        Vector2 movementInput{0,0};
        gameHandler.handleInput(movementInput);
        movementInput = Vector2Scale(movementInput, dataManager.movementSpeed); // Multiply by speed

        // Calculate air resistance
        Vector2 airResistance = player.velocity - player.velocity * dataManager.airResistance;

        player.addForce(movementInput); // Add movementInput to player velocity
        player.addForce(dataManager.gravity); // Add gravity to player velocity
        player.addForce(Vector2Negate(airResistance)); // Add airResistance to player velocity
        

        // Stop completely if below the threshold
        if (Vector2Length(player.velocity) < dataManager.velocityThreshhold) player.velocity = { 0.0f, 0.0f };

        // Physics collision response
        gameHandler.collisionResponse(player, mapGrid);

        // Clamp player to gridMap
        Vector2 positionBeforeClamp = player.position;
        player.position.x = std::clamp(player.position.x, 0.f, static_cast<float>(mapGrid.sizeX * mapGrid.blockSize - player.size.x));
        player.position.y = std::clamp(player.position.y, 0.f, static_cast<float>(mapGrid.sizeY * mapGrid.blockSize - player.size.y));
        // If position was clamped set velocity to 0 in this axis
        if(positionBeforeClamp.x != player.position.x) player.velocity.x = 0;
        if(positionBeforeClamp.y != player.position.y) player.velocity.y = 0;

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