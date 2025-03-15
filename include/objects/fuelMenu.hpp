#pragma once

#include <memory>
#include <map>
#include <string>

#include <raylib.h>
#include <raymath.h>

#include <lufuGui.hpp>
#include <guiContext.hpp>
#include <assetManager.hpp>

class FuelMenu : virtual public GuiContext{
private:
    std::map<std::string, std::shared_ptr<GuiElement>> guiElements;
    
public:
    using GuiContext::GuiContext;

    void initialize() override{
        //AssetManager& assetManager = AssetManager::getInstance();
        Font font = GetFontDefault();
        guiElements["testText"] = std::make_shared<Text>(mPosition + Vector2{100,500}, Vector2{160,40}, "Test Text", 15, font);
        guiElements["closeButton"] = std::make_shared<Button>(mPosition + Vector2{580,0}, Vector2{20,20});
        guiElements["testTextButton"] = std::make_shared<TextButton>(mPosition + Vector2{100,100}, Vector2{160,40}, "Test Text", 15, font);
        guiElements["testButton"] = std::make_shared<Button>(mPosition + Vector2{100,400}, Vector2{160,40});
        guiElements["testToggle"] = std::make_shared<Toggle>(mPosition + Vector2{500,100}, Vector2{20,20});
    }

    void update() override {
        // Check if menu is active
            if(mIsActive){
            
            // Update GuiElements
            for(auto & [key, guiElement] : guiElements){
                guiElement->update();
            }

            if(guiElements["closeButton"]->mIsPressed){
                disable();
            }
        }
    }

    void render() override {
        // Check if menu is activ
        if(mIsActive){
            DrawRectangleV(mPosition, mSize, GREEN);

            // Draw GuiElements
            for(auto [key, guiElement] : guiElements){
                guiElement->render();
            }
        }
    }
};