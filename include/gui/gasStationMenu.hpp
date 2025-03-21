#pragma once

#include <memory>
#include <map>
#include <string>

#include <raylib.h>
#include <raymath.h>

#include <lufuGui.hpp>
#include <guiContext.hpp>
#include <dataManager.hpp>

class GasStationMenu : virtual public GuiContext{
private:
    lufu_gui::Text titelText;
    lufu_gui::TextButton buyGasTextButton;
    
public:
    using GuiContext::GuiContext;

    void initialize() override{
        Font font = GetFontDefault();
        titelText = lufu_gui::Text("Gas Station", 40, font);
        buyGasTextButton = lufu_gui::TextButton(Vector2{120,40}, "Buy Gas", 20, font);
    }

    void update() override {
        // Check if menu is active
        if(mIsActive){
            // Update GuiElements
            titelText.setPosition(mPosition + Vector2{5,5});
            buyGasTextButton.setPosition(mPosition + Vector2{20,100});

            titelText.update();
            buyGasTextButton.update();

            if(buyGasTextButton.mIsPressed){
                DataManager::getInstance().player.gasTank.mGas = DataManager::getInstance().player.gasTank.mGasMax;
            }
        }
    }

    void render() override {
        // Check if menu is activ
        if(mIsActive){
            DrawRectangleV(mPosition, mSize, GRAY);

            // Draw GuiElements
            titelText.render();
            buyGasTextButton.render();
        }
    }
};