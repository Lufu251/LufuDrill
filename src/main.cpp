#include <iostream>
#include <algorithm>

#include <raylib.h>

#include <scene.hpp>
#include <gameScene.hpp>
#include <menuScene.hpp>
#include <assetManager.hpp>
#include <dataManager.hpp>
#include <gameHandler.hpp>
#include <gameRenderer.hpp> 

#include <fstream>

int main(void){
    // Declaration
    // ----------------------------------------------------------------------------------
    // Access singleton classes
    AssetManager& assetManager = AssetManager::getInstance();
    DataManager& dataManager = DataManager::getInstance();
    // AssetManager and DataManager
    assetManager.searchAssetsDirectoryPath("assets", 3);
    assetManager.loadFont("roboto-regular", "Roboto-Regular.ttf", 16);

    dataManager.searchDataDirectoryPath("data", 3);
    dataManager.loadSettingConfig("settings.json");

    

    // Set window parameters
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(dataManager.screenWidth, dataManager.screenHeight, "LuFu_Drill");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);
    SetWindowMinSize(500, 500);
    // SetWindowIcon(Image image);
    //ToggleFullscreen(); // Toggle window state: fullscreen/windowed, resizes monitor to match window resolution
    //ToggleBorderlessWindowed(); // Toggle window state: borderless windowed, resizes window to match monitor resolution

    // Declare scene
    dataManager.activeScene = std::make_unique<MenuScene>();

    // Initialization
    // ---------------------------------------------------------------------------------
    dataManager.activeScene->initialize();

    while (dataManager.windowOpen){
        // Logic
        // ----------------------------------------------------------------------------------
        // Check window should close
        if(WindowShouldClose()) dataManager.windowOpen = false;

        // Update activeScene
        dataManager.activeScene->update();
        
        // Draw
        // ----------------------------------------------------------------------------------
        BeginDrawing();
            // Render activeScene
            dataManager.activeScene->render();
        EndDrawing();
    }

    // Cleanup resources and close raylib
    assetManager.cleanup();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}