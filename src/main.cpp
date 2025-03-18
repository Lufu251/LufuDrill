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
    // AssetManager and DataManager
    AssetManager::getInstance().searchAssetsDirectoryPath("assets", 3);
    AssetManager::getInstance().loadFont("roboto-regular", "Roboto-Regular.ttf", 16);

    DataManager::getInstance().searchDataDirectoryPath("data", 3);
    DataManager::getInstance().loadSettingConfig("settings.json");

    // Set window parameters
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(DataManager::getInstance().screenWidth, DataManager::getInstance().screenHeight, "LuFu_Drill");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);
    SetWindowMinSize(800, 800);
    // SetWindowIcon(Image image);
    //ToggleFullscreen(); // Toggle window state: fullscreen/windowed, resizes monitor to match window resolution
    //ToggleBorderlessWindowed(); // Toggle window state: borderless windowed, resizes window to match monitor resolution

    // Declare scene
    DataManager::getInstance().activeScene = std::make_unique<MenuScene>();

    // Initialization
    // ---------------------------------------------------------------------------------
    DataManager::getInstance().activeScene->initialize();

    while (DataManager::getInstance().windowOpen){
        // Logic
        // ----------------------------------------------------------------------------------
        // Check window should close
        if(WindowShouldClose()) DataManager::getInstance().windowOpen = false;

        // Update activeScene
        DataManager::getInstance().activeScene->update();
        
        // Draw
        // ----------------------------------------------------------------------------------
        BeginDrawing();
            // Render activeScene
            DataManager::getInstance().activeScene->render();
        EndDrawing();
    }

    // Cleanup resources and close raylib
    AssetManager::getInstance().cleanup();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}