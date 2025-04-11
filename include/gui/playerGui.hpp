#pragma once

#include <string>

#include <raylib.h>
#include <raymath.h>

#include <lufuGui.hpp>
#include <guiContext.hpp>
#include <globals.hpp>

class PlayerGui : virtual public GuiContext{
private:
    Vector2 anchorUpperLeftCorner;
    lufu_gui::Text moneyText;    

    Vector2 anchorLowerLeftCorner;
    lufu_gui::Text fuelBarText;
    lufu_gui::ProgressBar fuelBar;
    lufu_gui::Text hullBarText;
    lufu_gui::ProgressBar hullBar;
    lufu_gui::Text cargoBarText;
    lufu_gui::ProgressBar cargoBar;
    
public:
    using GuiContext::GuiContext;

    void initialize() override{
        Font font = GetFontDefault();
        anchorUpperLeftCorner = {0, 0};
        moneyText = lufu_gui::Text(20, font);

        anchorLowerLeftCorner = {0, static_cast<float>(GetScreenHeight() - 200)};
        fuelBarText = lufu_gui::Text(20, font);
        fuelBar = lufu_gui::ProgressBar(Vector2{190,20});
        hullBarText = lufu_gui::Text(20, font);
        hullBar = lufu_gui::ProgressBar(Vector2{190,20});
        cargoBarText = lufu_gui::Text(20, font);
        cargoBar = lufu_gui::ProgressBar(Vector2{190,20});
    }

    void update() override {
        // Update anchor to be in the middle of the window
        if(IsWindowResized()){
            anchorLowerLeftCorner = {0, static_cast<float>(GetScreenHeight() - 200)};
            anchorUpperLeftCorner = {0, 0};
        }
        // Check if menu is active
        if(mIsActive){
            moneyText.update(anchorUpperLeftCorner + Vector2{5,0});

            moneyText.setText("Money: " + std::to_string(gDM.player.money));

            fuelBarText.update(anchorLowerLeftCorner + Vector2{5,0});
            fuelBar.update(anchorLowerLeftCorner + Vector2{5,20});
            hullBarText.update(anchorLowerLeftCorner + Vector2{5,45});
            hullBar.update(anchorLowerLeftCorner + Vector2{5,65});
            cargoBarText.update(anchorLowerLeftCorner + Vector2{5,90});
            cargoBar.update(anchorLowerLeftCorner + Vector2{5,110});
            
            fuelBarText.setText("Fuel:");
            hullBarText.setText("Hull:");
            cargoBarText.setText("CargoBay:");
            fuelBar.setProgress(gDM.player.gasTank.mGas / gDM.player.gasTank.mGasMax);
            hullBar.setProgress(gDM.player.hull.mHealth / gDM.player.hull.mHealthMax);
            cargoBar.setProgress(static_cast<float>(gDM.player.cargoBay.mStored) / static_cast<float>(gDM.player.cargoBay.mMaxStored));
        }
    }

    void render() override {
        // Check if menu is activ
        if(mIsActive){
            // UpperLeftCorner
            DrawRectangleV(anchorUpperLeftCorner,{200,50}, { 200, 200, 245, 230 });
            moneyText.render();
            

            // LowerLeftConrner
            DrawRectangleV(anchorLowerLeftCorner,{200,200}, { 200, 200, 245, 230 });
            fuelBarText.render();
            fuelBar.render();
            hullBarText.render();
            hullBar.render();
            cargoBarText.render();
            cargoBar.render();
        }
    }
};