#pragma once

#include <scene.hpp>

#include <raygui.h>
#include <raylib.h>

#include <map>
#include <memory>
#include <lufuGui.hpp>
#include <assetManager.hpp>
#include <dataManager.hpp>

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
        anchor = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

        Font font = GetFontDefault();
        titelText = lufu_gui::Text(anchor - Vector2{200,100}, Vector2{200,50},"LufuDrill", 32, font);
        startTextButton = lufu_gui::TextButton(anchor + Vector2{200,300}, Vector2{100,20},"Start", 16, font);
        resumeTextButton = lufu_gui::TextButton(anchor + Vector2{200,500}, Vector2{100,20},"Resume", 16, font);
        exitTextButton = lufu_gui::TextButton(anchor + Vector2{200,700}, Vector2{100,20},"Exit", 16, font);
    }

    void update() override {
        // Update anchor to be in the middle of the window
        if(IsWindowResized()){
            anchor = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
        }

    }

    void render() override {
    // Draw GuiElements

    }
};