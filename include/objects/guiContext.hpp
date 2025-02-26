#pragma once

#include <raylib.h>

class GuiContext{
public:
    Vector2 mPosition;
    Vector2 mSize;

    bool mIsActive = false;
    Font mFont;

    GuiContext(){

    }
    
    GuiContext(const Vector2& rPosition, const Vector2& rSize):mPosition{rPosition}, mSize{rSize}{}
    ~GuiContext(){}
    
    virtual void initialize() = 0;
    virtual void update() = 0;
    virtual void render() = 0;

    void setPosition(const Vector2& rPosition){
        mPosition = rPosition;
    }

    void setSize(const Vector2& rSize){
        mSize = rSize;
    }

    void enable(){
        mIsActive = true;
    }

    void disable(){
        mIsActive = false;
    }
};