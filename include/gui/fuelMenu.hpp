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
    lufu_gui::Text titelText;
    lufu_gui::Button closeButton;
    lufu_gui::TextButton buyFuelTextButton;
    
public:
    using GuiContext::GuiContext;

    void initialize() override{
        //AssetManager& assetManager = AssetManager::getInstance();
        Font font = GetFontDefault();
        titelText = lufu_gui::Text(mPosition + Vector2{5,5}, "Fuel Station", 40, font);
        closeButton = lufu_gui::Button(mPosition + Vector2{575,5}, Vector2{20,20});
        buyFuelTextButton = lufu_gui::TextButton(mPosition + Vector2{20,100}, Vector2{100,30}, "Buy Fuel", 20, font);
    }

    void update() override {
        // Check if menu is active
        if(mIsActive){
            // Update GuiElements
            titelText.update();
            closeButton.update();
            buyFuelTextButton.update();

            if(closeButton.mIsPressed){
                disable();
            }
        }
    }

    void render() override {
        // Check if menu is activ
        if(mIsActive){
            DrawRectangleV(mPosition, mSize, GRAY);

            // Draw GuiElements
            titelText.render();
            closeButton.render();
            buyFuelTextButton.render();
        }
    }
};