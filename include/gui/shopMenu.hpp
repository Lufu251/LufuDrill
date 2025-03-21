#pragma once

#include <memory>
#include <map>
#include <string>

#include <raylib.h>
#include <raymath.h>

#include <lufuGui.hpp>
#include <guiContext.hpp>
#include <assetManager.hpp>

class ShopMenu : virtual public GuiContext{
private:
    lufu_gui::Text titelText;
    lufu_gui::TextButton repairHullTextButton;
    
public:
    using GuiContext::GuiContext;

    void initialize() override{
        Font font = GetFontDefault();
        titelText = lufu_gui::Text("Shop", 40, font);
        repairHullTextButton = lufu_gui::TextButton(Vector2{120,40}, "Repair Hull", 20, font);
    }

    void update() override {
        // Check if menu is active
        if(mIsActive){
            // Update GuiElements
            titelText.setPosition(mPosition + Vector2{5,5});
            repairHullTextButton.setPosition(mPosition + Vector2{20,100});

            titelText.update();
            repairHullTextButton.update();

            if(repairHullTextButton.mIsPressed){
                DataManager::getInstance().player.hull.mHealth = DataManager::getInstance().player.hull.mHealthMax;
            }
        }
    }

    void render() override {
        // Check if menu is activ
        if(mIsActive){
            DrawRectangleV(mPosition, mSize, GRAY);

            // Draw GuiElements
            titelText.render();
            repairHullTextButton.render();
        }
    }
};