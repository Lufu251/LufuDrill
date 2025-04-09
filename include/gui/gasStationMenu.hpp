#pragma once

#include <memory>
#include <map>
#include <string>

#include <raylib.h>
#include <raymath.h>

#include <lufuGui.hpp>
#include <guiContext.hpp>
#include <globals.hpp>

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
            titelText.setPosition(mPosition + Vector2{15,10});
            buyGasTextButton.setPosition(mPosition + Vector2{20,100});

            titelText.update();
            buyGasTextButton.update();

            if(buyGasTextButton.mIsPressed){
                gDM.player.gasTank.mGas = gDM.player.gasTank.mGasMax;
            }
        }
    }

    void render() override {
        // Check if menu is activ
        if(mIsActive){
            Texture2D texture = gAM.getTexture("menuTemplate");
            DrawTextureEx(texture, mPosition, 0, 4, {255,255,255,250});

            // Draw GuiElements
            titelText.render();
            buyGasTextButton.render();
        }
    }
};