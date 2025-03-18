#pragma once

#include <scene.hpp>

#include <raylib.h>
#include <map>
#include <memory>

#include <lufuGui.hpp>
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
            DataManager::getInstance().map = Grid(200,1000, 32);
            gameHandler.generateTerrain(DataManager::getInstance().map);

            // Load scene
            DataManager::getInstance().activeScene = std::make_unique<GameScene>();
            DataManager::getInstance().activeScene->initialize();
        }
        if(resumeTextButton.mIsPressed){
            // Load Map from file
            DataManager::getInstance().loadGameState("save.sv");

            // Load scene
            DataManager::getInstance().activeScene = std::make_unique<GameScene>();
            DataManager::getInstance().activeScene->initialize();
        }
        if(exitTextButton.mIsPressed){
            DataManager::getInstance().windowOpen = false;
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