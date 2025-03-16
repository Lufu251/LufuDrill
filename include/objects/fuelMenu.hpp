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
    Text testText;
    Button closeButton;
    TextButton testTextButton;
    Button testButton;
    Toggle testToggle;
    ProgressBar testProgressBar;
    
public:
    using GuiContext::GuiContext;

    void initialize() override{
        //AssetManager& assetManager = AssetManager::getInstance();
        Font font = GetFontDefault();
        testText = Text(mPosition + Vector2{100,500}, Vector2{160,40}, "Test Text", 15, font);
        closeButton = Button(mPosition + Vector2{580,0}, Vector2{20,20});
        testTextButton = TextButton(mPosition + Vector2{100,100}, Vector2{160,40}, "Test Text", 15, font);
        testButton = Button(mPosition + Vector2{100,400}, Vector2{160,40});
        testToggle = Toggle(mPosition + Vector2{500,100}, Vector2{20,20});
        testProgressBar = ProgressBar(mPosition + Vector2{10,10}, Vector2{400,20});
    }

    void update() override {
        // Check if menu is active
            if(mIsActive){
            
            // Update GuiElements
            testText.update();
            closeButton.update();
            testTextButton.update();
            testButton.update();
            testToggle.update();
            testProgressBar.update(0.5f);

            if(closeButton.mIsPressed){
                disable();
            }
        }
    }

    void render() override {
        // Check if menu is activ
        if(mIsActive){
            DrawRectangleV(mPosition, mSize, GREEN);

            // Draw GuiElements
            testText.render();
            closeButton.render();
            testTextButton.render();
            testButton.render();
            testToggle.render();
            testProgressBar.render();
        }
    }
};