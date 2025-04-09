#include <iostream>
#include <algorithm>

#include <raylib.h>

#include <scene.hpp>
#include <gameScene.hpp>
#include <menuScene.hpp>
#include <globals.hpp>
#include <gameHandler.hpp>
#include <gameRenderer.hpp> 

int main(void){
    // Declaration
    // ----------------------------------------------------------------------------------
    // AssetManager and DataManager
    gAM.searchAssetsDirectoryPath("assets", 3);

    gDM.searchDataDirectoryPath("data", 3);
    gDM.loadSettingConfig("settings.json");
    gDM.loadToolsConfig("tools.json");
    gDM.loadBlocksConfig("blocks.json");
    gDM.loadOresConfig("ores.json");

    // Initialize audio device
    InitAudioDevice();
    SetMasterVolume(gDM.masterVolume);

    // Set window parameters
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(gDM.screenWidth, gDM.screenHeight, "LuFu_Drill");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);
    SetWindowMinSize(500, 500);
    //SetWindowIcon(Image image);
    //ToggleFullscreen(); // Toggle window state: fullscreen/windowed, resizes monitor to match window resolution
    //ToggleBorderlessWindowed(); // Toggle window state: borderless windowed, resizes window to match monitor resolution

    // Load fonts because they need to be loaded after window init
    gAM.loadFont("thaleah_fat_20", "ThaleahFat.ttf", 20);
    gAM.loadFont("thaleah_fat_48", "ThaleahFat.ttf", 48);

    // Declare scene
    gDM.activeScene = std::make_unique<MenuScene>();

    // Initialization
    // ---------------------------------------------------------------------------------
    gDM.activeScene->initialize();

    while (gDM.windowOpen){
        // Logic
        // ----------------------------------------------------------------------------------
        // Check window should close
        if(WindowShouldClose()) gDM.windowOpen = false;

        // Update activeScene
        gDM.activeScene->update();
        
        // Draw
        // ----------------------------------------------------------------------------------
        BeginDrawing();
            // Render activeScene
            gDM.activeScene->render();
        EndDrawing();
    }
    //gDM.saveGameState("save.sv");
    // Cleanup resources and close raylib
    gAM.cleanup();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}