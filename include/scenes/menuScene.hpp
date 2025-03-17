#pragma once

#include <scene.hpp>

#include <raygui.h>
#include <raylib.h>

#include <map>
#include <memory>
#include <lufuGui.hpp>
#include <assetManager.hpp>
#include <dataManager.hpp>
#include <gameHandler.hpp>
#include <gameScene.hpp>

class MenuScene : public Scene{
private:
    Vector2 anchor;
    lufu_gui::Text titelText;
    lufu_gui::TextButton startTextButton;
    lufu_gui::TextButton resumeTextButton;
    lufu_gui::TextButton exitTextButton;

    // Access singleton classes
    AssetManager& assetManager = AssetManager::getInstance();
    DataManager& dataManager = DataManager::getInstance();

public:
    MenuScene(/* args */){}
    ~MenuScene(){}

    void initialize() override{
        // Set anchor to the middle of the screen
        anchor = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f - 200};

        Font font = GetFontDefault();
        titelText = lufu_gui::Text("LufuDrill", 50, font);
        startTextButton = lufu_gui::TextButton({200,40},"Start New Game", 20, font);
        resumeTextButton = lufu_gui::TextButton({200,40},"Resume", 20, font);
        exitTextButton = lufu_gui::TextButton({200,40},"Exit", 20, font);
    }

    void update() override {
        GameHandler gameHandler;
        
        // Update anchor to be in the middle of the window
        if(IsWindowResized()){
            anchor = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f - 200};
        }

        titelText.setPosition(anchor + Vector2{0,0} - titelText.mSize /2);
        startTextButton.setPosition(anchor + Vector2{0,200} - startTextButton.mSize /2);
        resumeTextButton.setPosition(anchor + Vector2{0,250} - resumeTextButton.mSize /2);
        exitTextButton.setPosition(anchor + Vector2{0,400} - exitTextButton.mSize /2);

        titelText.update();
        startTextButton.update();
        resumeTextButton.update();
        exitTextButton.update();

        if(startTextButton.mIsPressed){
            // Generate map
            dataManager.map = Grid(200,1000, 32);
            gameHandler.generateTerrain(dataManager.map);

            // Load scene
            dataManager.activeScene = std::make_unique<GameScene>();
            dataManager.activeScene->initialize();
        }
        if(resumeTextButton.mIsPressed){
            // Load Map from file
            dataManager.loadGameState("save.sv");

            // Load scene
            dataManager.activeScene = std::make_unique<GameScene>();
            dataManager.activeScene->initialize();
        }
        if(exitTextButton.mIsPressed){
            dataManager.windowOpen = false;
        }
    }

    void render() override {
        // Clear Screen for the new render cycle
        ClearBackground(RAYWHITE);

        // Draw GuiElements
        titelText.render();
        startTextButton.render();
        resumeTextButton.render();
        exitTextButton.render();
    }
};