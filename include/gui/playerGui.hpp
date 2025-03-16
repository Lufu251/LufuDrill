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
    
public:
    using GuiContext::GuiContext;

    void initialize(){

    }

    void update() override {
        // Check if menu is active
        if(mIsActive){
        // Update GuiElements
            
        }
    }

    void render() override {
        // Check if menu is activ
        if(mIsActive){
            // Draw GuiElements
            
        }
    }
};