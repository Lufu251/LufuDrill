#include <iostream>
#include <algorithm>

#include <raylib.h>


#include <assetManager.hpp>
#include <dataManager.hpp>
#include <gameHandler.hpp>
#include <gameRenderer.hpp>

#include <fstream>

int main(){
    // Declaration ----------------------------------------------------------------------------------

    // Set window parameters
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    
    InitWindow(500, 500, "LuFu_Drill");
    SetTargetFPS(60);
    //SetWindowMinSize(500, 500);
    // SetWindowIcon(Image image);
    //ToggleFullscreen(); // Toggle window state: fullscreen/windowed, resizes monitor to match window resolution
    //ToggleBorderlessWindowed(); // Toggle window state: borderless windowed, resizes window to match monitor resolution

    // Load DataManager and AssetManager befor anything else!
    // Access singleton classes
    AssetManager& assetManager = AssetManager::getInstance();
    DataManager& dataManager = DataManager::getInstance();

    GameHandler gameHandler;
    GameRenderer gameRenderer;

    // Gui Elements
    PlayerGui playerGui;
    FuelMenu fuelMenu;

    // Initialization ---------------------------------------------------------------------------------
    // AssetManager
    assetManager.searchAssetsDirectoryPath("assets", 3);
    // Preload textureAtlas
    assetManager.loadTextureAtlas("tileset");
    assetManager.loadTextureAtlas("particleset");
    // Preload font
    assetManager.loadFont("roboto-regular", "Roboto-Regular.ttf", 32);

    // DataManager
    dataManager.searchDataDirectoryPath("data", 3);
    // Load tools configuration
    dataManager.loadToolConfig("tools.json");
    dataManager.loadSettingConfig("settings.json");

    SetWindowSize(dataManager.screenWidth, dataManager.screenHeight);

    playerGui = PlayerGui({0,0}, {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())});
    playerGui.initialize();
    playerGui.enable();

    fuelMenu = FuelMenu({100,100}, {600,600});
    fuelMenu.initialize();
    fuelMenu.enable();

    dataManager.player = Player({200,200}, {24,24}, {0,0});
    dataManager.map = Grid(200,1000, 32);
    gameHandler.generateTerrain(dataManager.map);

    // Load Map from file
    dataManager.loadGameState("test.save");
    //dataManager.saveGameState("test.save");

    // Camera initialise
    gameRenderer.camera.zoom = 2.0f;
    gameRenderer.camera.rotation = 0.0f;
    gameRenderer.camera.target = dataManager.player.position;
    gameRenderer.setCameraOffset({GetScreenWidth() / 2.0f - dataManager.player.size.x, GetScreenHeight() / 2.0f - dataManager.player.size.y});

    while (!WindowShouldClose()){
        // Logic ----------------------------------------------------------------------------------
        //float deltaTime = GetFrameTime();

        // Do updates on screen resize
        if(IsWindowResized()){
            gameRenderer.setCameraOffset({GetScreenWidth() / 2.0f - dataManager.player.size.x, GetScreenHeight() / 2.0f - dataManager.player.size.y});
        }

        // Update and pan camera in the direction of the player
        gameRenderer.moveCameraToPlayer(dataManager.player);

        // Movement input
        Vector2 movementInput{0,0};
        gameHandler.handleInput(movementInput);
        movementInput = Vector2Scale(movementInput, dataManager.movementSpeed); // Multiply by speed

        // Calculate air resistance
        Vector2 airResistance = dataManager.player.velocity - dataManager.player.velocity * dataManager.airResistance;
        dataManager.player.addForce(movementInput); // Add movementInput to player velocity
        dataManager.player.addForce(dataManager.gravity); // Add gravity to player velocity
        dataManager.player.addForce(Vector2Negate(airResistance)); // Add airResistance to player velocity

        // Stop completely if below the threshold
        if (Vector2Length(dataManager.player.velocity) < dataManager.velocityThreshhold) dataManager.player.velocity = { 0.0f, 0.0f };

        // Physics collision response and move player
        gameHandler.checkCollisionAndMove(dataManager.player, dataManager.map);

        //Clamp player to grid
        gameHandler.clampToGrid(dataManager.player, dataManager.map);

        // Check if player is touching a block on any side and count for how long it is touching
        gameHandler.checkTouching(dataManager.player, dataManager.map);

        // Print touching sides
        if(dataManager.player.left) std::cout << "Player LEFT " << dataManager.player.left << std::endl;
        if(dataManager.player.right) std::cout << "Player RIGHT " << dataManager.player.right << std::endl;
        if(dataManager.player.top) std::cout << "Player TOP " << dataManager.player.top << std::endl;
        if(dataManager.player.bottom) std::cout << "Player BOTTOM " << dataManager.player.bottom << std::endl;

        // Update Gui
        playerGui.update();
        fuelMenu.update();
        
        // Draw ----------------------------------------------------------------------------------
        BeginDrawing();
            // Clear Screen for the new render cycle
            ClearBackground(PURPLE);
            gameRenderer.renderGrid(dataManager.map);
            gameRenderer.renderPlayer(dataManager.player);

            fuelMenu.render();
            playerGui.render();
            
        EndDrawing();
    }

    // Cleanup resources and close raylib
    assetManager.cleanup();
    CloseWindow();
    return 0;
}