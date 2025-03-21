#pragma once

#include <memory>
#include <map>
#include <string>

#include <raylib.h>
#include <raymath.h>

#include <lufuGui.hpp>
#include <guiContext.hpp>
#include <dataManager.hpp>
#include <assetManager.hpp>

class PlayerGui : virtual public GuiContext{
private:
    Vector2 anchorLowerLeftCorner;

    lufu_gui::Text fuelBarText;
    lufu_gui::ProgressBar fuelBar;
    lufu_gui::Text hullBarText;
    lufu_gui::ProgressBar hullBar;
    
public:
    using GuiContext::GuiContext;

    void initialize(){
        Font font = GetFontDefault();
        anchorLowerLeftCorner = {0, static_cast<float>(GetScreenHeight() - 200)};

        fuelBarText = lufu_gui::Text("Fuel:", 20, font);
        fuelBar = lufu_gui::ProgressBar(Vector2{190,20});
        hullBarText = lufu_gui::Text("Hull:", 20, font);
        hullBar = lufu_gui::ProgressBar(Vector2{190,20});
    }

    void update() override {
        // Update anchor to be in the middle of the window
        if(IsWindowResized()){
            anchorLowerLeftCorner = {0, static_cast<float>(GetScreenHeight() - 200)};
        }
        // Check if menu is active
        if(mIsActive){
            fuelBarText.setPosition(anchorLowerLeftCorner + Vector2{5,0});
            fuelBar.setPosition(anchorLowerLeftCorner + Vector2{5,20});
            hullBarText.setPosition(anchorLowerLeftCorner + Vector2{5,45});
            hullBar.setPosition(anchorLowerLeftCorner + Vector2{5,65});

            fuelBarText.update();
            fuelBar.update();
            hullBarText.update();
            hullBar.update();

            fuelBar.setProgress(DataManager::getInstance().player.gasTank.mGas / DataManager::getInstance().player.gasTank.mGasMax);
            hullBar.setProgress(DataManager::getInstance().player.hull.mHealth / DataManager::getInstance().player.hull.mHealthMax);
        }
    }

    void render() override {
        // Check if menu is activ
        if(mIsActive){
            DrawRectangleV(anchorLowerLeftCorner,{200,200}, { 200, 200, 245, 230 });
            fuelBarText.render();
            fuelBar.render();
            hullBarText.render();
            hullBar.render();
        }
    }
};