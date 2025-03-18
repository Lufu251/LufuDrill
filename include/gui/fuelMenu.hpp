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
        Font font = GetFontDefault();
        titelText = lufu_gui::Text("Gas Station", 40, font);
        closeButton = lufu_gui::Button(Vector2{20,20});
        buyFuelTextButton = lufu_gui::TextButton(Vector2{100,30}, "Buy Fuel", 20, font);
    }

    void update() override {
        // Check if menu is active
        if(mIsActive){
            // Update GuiElements
            titelText.setPosition(mPosition + Vector2{5,5});
            closeButton.setPosition(mPosition + Vector2{575,5});
            buyFuelTextButton.setPosition(mPosition + Vector2{20,100});

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