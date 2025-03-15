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
    std::map<std::string, std::shared_ptr<GuiElement>> guiElements;

    Vector2 anchor;

    // Access singleton classes
    AssetManager& assetManager = AssetManager::getInstance();
    DataManager& dataManager = DataManager::getInstance();

public:
    MenuScene(/* args */){}
    ~MenuScene(){}

    void initialize() override{
        // Set anchor to the middle of the screen
        anchor = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

        guiElements["titelTextBox"] = std::make_shared<TextBox>(anchor - Vector2{200,100}, Vector2{200,50}, 60, assetManager.getFont("roboto-regular"));
        guiElements["startTextBox"] = std::make_shared<TextBox>(anchor + Vector2{200,300}, Vector2{100,20}, 18, assetManager.getFont("roboto-regular"));
        guiElements["resumeTextBox"] = std::make_shared<TextBox>(anchor + Vector2{200,500}, Vector2{100,20}, 18, assetManager.getFont("roboto-regular"));
        guiElements["exitTextBox"] = std::make_shared<TextBox>(anchor + Vector2{200,700}, Vector2{100,20}, 18, assetManager.getFont("roboto-regular"));
    }

    void update() override {
        // Update anchor to be in the middle of the window
        if(IsWindowResized()){
            anchor = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
        }

        for(auto [key, guiElement] : guiElements){
            guiElement->update();
        }
    }

    void render() override {
    // Draw GuiElements
        for(auto [key, guiElement] : guiElements){
            guiElement->render();
        }
    }
};