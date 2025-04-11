#pragma once

#include <string>

#include <raylib.h>
#include <raymath.h>

#include <lufuGui.hpp>
#include <guiContext.hpp>
#include <globals.hpp>

class TraderMenu : virtual public GuiContext{
private:
    lufu_gui::Text titelText;
    lufu_gui::TextButton sellAllTextButton;
    
public:
    using GuiContext::GuiContext;

    void initialize() override{
        Font font = GetFontDefault();
        titelText = lufu_gui::Text(40, font);
        sellAllTextButton = lufu_gui::TextButton(Vector2{120,40}, 20, font);
    }

    void update() override {
        // Check if menu is active
        if(mIsActive){
            // Update GuiElements
            titelText.update(mPosition + Vector2{15,10});
            sellAllTextButton.update(mPosition + Vector2{20,100});
            titelText.setText("Trader");
            sellAllTextButton.setText("Sell All");

            if(sellAllTextButton.mIsPressed){
                // Sell All
                gDM.player.money += gDM.player.getCargoValue();
                gDM.player.clearCargo();
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
            sellAllTextButton.render();
        }
    }
};