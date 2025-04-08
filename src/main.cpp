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

int main(void){
    // Declaration
    // ----------------------------------------------------------------------------------
    // AssetManager and DataManager
    AssetManager::getInstance().searchAssetsDirectoryPath("assets", 3);

    DataManager::getInstance().searchDataDirectoryPath("data", 3);
    DataManager::getInstance().loadSettingConfig("settings.json");
    DataManager::getInstance().loadToolsConfig("tools.json");
    DataManager::getInstance().loadBlocksConfig("blocks.json");

    // Initialize audio device
    InitAudioDevice();
    SetMasterVolume(DataManager::getInstance().masterVolume);

    // Set window parameters
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(DataManager::getInstance().screenWidth, DataManager::getInstance().screenHeight, "LuFu_Drill");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);
    SetWindowMinSize(500, 500);
    //SetWindowIcon(Image image);
    //ToggleFullscreen(); // Toggle window state: fullscreen/windowed, resizes monitor to match window resolution
    //ToggleBorderlessWindowed(); // Toggle window state: borderless windowed, resizes window to match monitor resolution

    // Load fonts because they need to be loaded after window init
    AssetManager::getInstance().loadFont("thaleah_fat_20", "ThaleahFat.ttf", 20);
    AssetManager::getInstance().loadFont("thaleah_fat_48", "ThaleahFat.ttf", 48);

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
    //DataManager::getInstance().saveGameState("save.sv");
    // Cleanup resources and close raylib
    AssetManager::getInstance().cleanup();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}