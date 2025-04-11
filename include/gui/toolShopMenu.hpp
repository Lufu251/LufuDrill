#pragma once

#include <string>

#include <raylib.h>
#include <raymath.h>

#include <lufuGui.hpp>
#include <guiContext.hpp>
#include <globals.hpp>

class ToolShopMenu : virtual public GuiContext{
private:
    lufu_gui::Text titelText;
    lufu_gui::TextButton repairHullTextButton;
    
public:
    using GuiContext::GuiContext;

    void initialize() override{
        Font font = GetFontDefault();
        titelText = lufu_gui::Text(40, font);
        repairHullTextButton = lufu_gui::TextButton(Vector2{120,40}, 20, font);
    }

    void update() override {
        // Check if menu is active
        if(mIsActive){
            // Update GuiElements
            titelText.update(mPosition + Vector2{15,10});
            repairHullTextButton.update(mPosition + Vector2{20,100});

            titelText.setText("Tool Shop");
            repairHullTextButton.setText("Repair Hull");

            if(repairHullTextButton.mIsPressed){
                gDM.player.hull.mHealth = gDM.player.hull.mHealthMax;
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
            repairHullTextButton.render();
        }
    }
};