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
    // AssetManager and DataManager search path
    assetManager.searchAssetsDirectoryPath("assets", 3);
    dataManager.searchDataDirectoryPath("data", 3);

    // Preload font
    assetManager.loadFont("roboto-regular", "Roboto-Regular.ttf", 16);

    // Set window parameters
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    
    InitWindow(dataManager.screenWidth, dataManager.screenHeight, "LuFu_Drill");
    SetTargetFPS(60);
    //SetWindowMinSize(500, 500);
    // SetWindowIcon(Image image);
    //ToggleFullscreen(); // Toggle window state: fullscreen/windowed, resizes monitor to match window resolution
    //ToggleBorderlessWindowed(); // Toggle window state: borderless windowed, resizes window to match monitor resolution

    // Declare scene
    std::unique_ptr<Scene> activeScene;
    activeScene = std::make_unique<GameScene>();

    // Initialization
    // ---------------------------------------------------------------------------------

    // initialize activeScene
    activeScene->initialize();

    while (!WindowShouldClose()){
        // Logic
        // ----------------------------------------------------------------------------------
        //float deltaTime = GetFrameTime();

        // Update activeScene
        activeScene->update();
        
        // Draw
        // ----------------------------------------------------------------------------------
        BeginDrawing();
            // Clear Screen for the new render cycle
            ClearBackground(PURPLE);

            // Render activeScene
            activeScene->render();
            
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