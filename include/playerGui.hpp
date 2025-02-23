#pragma once

#include <memory>
#include <map>
#include <string>

#include <raylib.h>
#include <raymath.h>

#include <lufuGui.hpp>
#include <guiContext.hpp>

class PlayerGui : virtual public GuiContext{
private:
    std::map<std::string, std::shared_ptr<GuiElement>> guiElements;
    
public:
    using GuiContext::GuiContext;

    void initialize(){
        mFont = LoadFontEx("../../data/Roboto-Regular.ttf", 64, NULL, 0);
        //guiElements["fuelText"] = std::make_shared<TextBox>(mPosition + Vector2{200,100}, Vector2{100,20}, 18, mFont);
    }

    void update() override {
        // Check if menu is active
            if(mIsActive){
            
            // Update GuiElements
            for(auto & [key, guiElement] : guiElements){
                guiElement->update();
            }
        }
    }

    void render() override {
        // Check if menu is activ
        if(mIsActive){

            DrawTextEx(mFont, "Fuel:", mPosition + Vector2{20,20}, 20, 1, GREEN);

            // Draw GuiElements
            for(auto [key, guiElement] : guiElements){
                guiElement->render();
            }
        }
    }
};