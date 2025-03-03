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

    // Camera
    gameRenderer.camera.zoom = 2.0f;
    gameRenderer.camera.rotation = 0.0f;
    gameRenderer.camera.target = player.position;
    gameRenderer.setCameraOffset({GetScreenWidth() / 2.0f - player.size.x, GetScreenHeight() / 2.0f - player.size.y});

    while (!WindowShouldClose()){
        // Logic ----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime();

        // Update camera offset when window is resized
        if(IsWindowResized()){
            gameRenderer.setCameraOffset({GetScreenWidth() / 2.0f - player.size.x, GetScreenHeight() / 2.0f - player.size.y});
        }

        // Update and smooth pan camera to the player position
        gameRenderer.moveCameraToPlayer(player);

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

        // Physics collision response and move player
        gameHandler.checkCollisionAndMove(player, mapGrid);

        //Clamp player to grid
        gameHandler.clampToGrid(player, mapGrid);

        // Check if player is touching a block on any side and count for how long it is touching
        gameHandler.checkTouching(player, mapGrid);

        // Print touching sides
        if(player.left) std::cout << "Player LEFT " << player.left << std::endl;
        if(player.right) std::cout << "Player RIGHT " << player.right << std::endl;
        if(player.top) std::cout << "Player TOP " << player.top << std::endl;
        if(player.bottom) std::cout << "Player BOTTOM " << player.bottom << std::endl;

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