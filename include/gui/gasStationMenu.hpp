#pragma once

#include <string>

#include <raylib.h>
#include <raymath.h>

#include <lufuGui.hpp>
#include <guiContext.hpp>
#include <globals.hpp>

#include <dataManager.hpp>
#include <assetManager.hpp>

class GasStationMenu : virtual public GuiContext{
private:
    lufu_gui::Text titelText;
    lufu_gui::Text fulltankText;
    lufu_gui::TextButton buyGasTextButton;
    
public:
    using GuiContext::GuiContext;

    void initialize() override{
        Font font = GetFontDefault();
        titelText = lufu_gui::Text(40, font);
        fulltankText = lufu_gui::Text(20, font);
        buyGasTextButton = lufu_gui::TextButton(Vector2{120,40}, 20, font);
    }

    void update() override {
        // Check if menu is active
        if(mIsActive){
            int gasToFill = gDM.player.gasTank.mGasMax - gDM.player.gasTank.mGas;
            // Update GuiElements
            titelText.update(mPosition + Vector2{15,10});
            titelText.setText("Gas Station");
            fulltankText.update(mPosition + Vector2{30,80});
            fulltankText.setText("Cost: " + std::to_string(gasToFill * gDM.gasCost));
            buyGasTextButton.update(mPosition + Vector2{20,100});
            buyGasTextButton.setText("Fill Tank");

            if(buyGasTextButton.mIsPressed){
                if(gDM.player.money >= gasToFill * gDM.gasCost){
                    gDM.player.money -= gasToFill * gDM.gasCost;
                    gDM.player.gasTank.mGas = gDM.player.gasTank.mGasMax;
                }
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
            fulltankText.render();
            buyGasTextButton.render();
        }
    }
};