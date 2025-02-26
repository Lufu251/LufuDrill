#pragma once

#include <memory>
#include <map>
#include <string>

#include <raylib.h>
#include <raymath.h>

#include <lufuGui.hpp>
#include <guiContext.hpp>

class FuelMenu : virtual public GuiContext{
private:
    std::map<std::string, std::shared_ptr<GuiElement>> guiElements;
    
public:
    using GuiContext::GuiContext;

    void initialize(){
        mFont = LoadFontEx("../../data/Roboto-Regular.ttf", 64, NULL, 0);
        guiElements["closeButton"] = std::make_shared<Button>(mPosition + Vector2{580,0}, Vector2{20,20});
        guiElements["testButton"] = std::make_shared<Button>(mPosition + Vector2{100,100}, Vector2{80,20});
        guiElements["testTextBox"] = std::make_shared<TextBox>(mPosition + Vector2{200,100}, Vector2{100,20}, 18, mFont);
        guiElements["testToggle"] = std::make_shared<Toggle>(mPosition + Vector2{500,100}, Vector2{20,20});
    }

    void update() override {
        // Check if menu is active
            if(mIsActive){
            
            // Update GuiElements
            for(auto & [key, guiElement] : guiElements){
                guiElement->update();
            }

            if(guiElements["closeButton"]->mIsPressed){
                disable();
            }
        }
    }

    void render() override {
        // Check if menu is activ
        if(mIsActive){
            DrawRectangleV(mPosition, mSize, GREEN);

            // Draw GuiElements
            for(auto [key, guiElement] : guiElements){
                guiElement->render();
            }
        }
    }
};