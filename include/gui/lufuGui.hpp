#pragma once

#include <raylib.h>
#include <raymath.h>

// Define Style
// -------------------------------------------------------
// BASE
#define BASE_COLOR (Color){ 0, 121, 241, 255 }
#define BASE_BORDER_COLOR (Color){ 0, 0, 0, 255 }
#define BASE_TEXT_COLOR (Color){ 0, 0, 0, 255 }

// HOVER
#define HOVERED_COLOR (Color){ 0, 101, 221, 255 }
#define HOVERED_BORDER_COLOR (Color){ 0, 0, 0, 255 }
#define HOVERED_TEXT_COLOR (Color){ 0, 0, 0, 255 }

// ACTIVATED
#define ACTIVATED_COLOR (Color){ 0, 81, 101, 255 }
#define ACTIVATED_BORDER_COLOR (Color){ 0, 0, 0, 255 }
#define ACTIVATED_TEXT_COLOR (Color){ 0, 0, 0, 255 }

// GENERAL
#define SECONDARY_COLOR (Color){ 255, 0, 0, 255 }
#define BORDER_SIZE 2
#define TEXT_SPACING 2
// -------------------------------------------------------

class GuiElement{
    public:
    Vector2 mPosition;
    Vector2 mSize;

    bool mIsHovered = false;
    bool mIsPressed = false;
    bool mIsLongPressed = false;
    bool mIsSelected = false;
    bool mIsToggled = false;
    
    protected:
    GuiElement(){}
    GuiElement(const Vector2& rPosition, const Vector2& rSize):mPosition{rPosition}, mSize{rSize}{}
    
    virtual ~GuiElement(){}

    // Check if the element is hovered.
    void updateHovered(){
        Vector2 mouseP = GetMousePosition();
        if (mouseP.x >= mPosition.x && mouseP.x <= mPosition.x + mSize.x && mouseP.y >= mPosition.y && mouseP.y <= mPosition.y + mSize.y) {
            mIsHovered = true;
        }
        else{
            mIsHovered = false;
        }
    }

    // Check if element is pressed. Hovered must always be called before this function.
    void updatePressed(){
        if(mIsHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            mIsPressed = true;
            mIsSelected = true;
        }
        else{
            mIsPressed = false;
        }
    }

    // Check if element is long pressed. Hovered must always be called before this function.
    void updateDown(){
        if(mIsHovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            mIsLongPressed = true;
        }
        else{
            mIsLongPressed = false;
        }
    }

    // Check if the element is Selected. Pressed and Hovered must be called before this.
    void updateSelect(){
        if(!mIsHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            mIsSelected = false;
        }
    }

    // Check if the element is Selected. Pressed and Hovered must be called before this.
    void updateToggled(){
        if(mIsHovered && mIsPressed){
            mIsToggled = !mIsToggled;
        }
    }
};

class Text : public GuiElement{
private:
    std::string mText;
    float mTextSize;
    Font mFont;
    Vector2 mTextMeasure = MeasureTextEx(mFont, mText.c_str(), mTextSize, TEXT_SPACING);

public:
    Text(){}
    Text(const Vector2& rPosition, const Vector2& rSize, const std::string& rtext, const float& rtextSize, Font& rFont) : GuiElement(rPosition, rSize), mText(rtext), mTextSize(rtextSize), mFont(rFont){}

    void update(){
        // Do nothing only displays text
    }

    void render(){
        Vector2 textOffset = (mSize - mTextMeasure) /2;
        DrawTextEx(mFont, mText.c_str(), mPosition + textOffset , mTextSize, TEXT_SPACING, BASE_TEXT_COLOR);
    }
};

class ProgressBar : public GuiElement{
private:
    float mProgress = 0;

public:
    ProgressBar(){}
    ProgressBar(const Vector2& rPosition, const Vector2& rSize) : GuiElement(rPosition, rSize){}

    void update(const float& rProgress){
        mProgress = rProgress;
    }

    void render(){
        DrawRectangleV(mPosition, mSize, BASE_COLOR);
        DrawRectangleV(mPosition, Vector2Multiply(mSize, {mProgress, 1}), SECONDARY_COLOR);
        DrawRectangleLinesEx(Rectangle{mPosition.x, mPosition.y, mSize.x, mSize.y}, BORDER_SIZE, BASE_BORDER_COLOR);
    }
};

class TextButton : public GuiElement{
private:
    std::string mText;
    float mTextSize;
    Font mFont;
    Vector2 mTextMeasure = MeasureTextEx(mFont, mText.c_str(), mTextSize, TEXT_SPACING);

public:
    TextButton(){}
    TextButton(const Vector2& rPosition, const Vector2& rSize, const std::string& rtext, const float& rtextSize, Font& rFont) : GuiElement(rPosition, rSize), mText(rtext), mTextSize(rtextSize), mFont(rFont){}

    void update(){
        updateHovered();
        updatePressed();
        updateDown();
    }

    void render(){
        Vector2 textOffset = (mSize - mTextMeasure) /2;
        if(mIsPressed){
            DrawRectangleV(mPosition, mSize, ACTIVATED_COLOR);
            DrawRectangleLinesEx(Rectangle{mPosition.x, mPosition.y, mSize.x, mSize.y}, BORDER_SIZE, ACTIVATED_BORDER_COLOR);
            DrawTextEx(mFont, mText.c_str(), mPosition + textOffset , mTextSize, TEXT_SPACING, ACTIVATED_TEXT_COLOR);
        }
        else if(mIsLongPressed){
            DrawRectangleV(mPosition, mSize, ACTIVATED_COLOR);
            DrawRectangleLinesEx(Rectangle{mPosition.x, mPosition.y, mSize.x, mSize.y}, BORDER_SIZE, ACTIVATED_BORDER_COLOR);
            DrawTextEx(mFont, mText.c_str(), mPosition + textOffset , mTextSize, TEXT_SPACING, ACTIVATED_TEXT_COLOR);
        }
        else if(mIsHovered){
            DrawRectangleV(mPosition, mSize, HOVERED_COLOR);
            DrawRectangleLinesEx(Rectangle{mPosition.x, mPosition.y, mSize.x, mSize.y}, BORDER_SIZE, HOVERED_BORDER_COLOR);
            DrawTextEx(mFont, mText.c_str(), mPosition + textOffset , mTextSize, TEXT_SPACING, HOVERED_TEXT_COLOR);
        }
        else{
            DrawRectangleV(mPosition, mSize, BASE_COLOR);
            DrawRectangleLinesEx(Rectangle{mPosition.x, mPosition.y, mSize.x, mSize.y}, BORDER_SIZE, BASE_BORDER_COLOR);
            DrawTextEx(mFont, mText.c_str(), mPosition + textOffset , mTextSize, TEXT_SPACING, BASE_TEXT_COLOR);
        }
    }
};

class Button : public GuiElement{
private:

public:
    Button(){}
    Button(const Vector2& rPosition, const Vector2& rSize) : GuiElement(rPosition, rSize){}

    void update(){
        updateHovered();
        updatePressed();
        updateDown();
    }

    void render(){
        if(mIsPressed){
            DrawRectangleV(mPosition, mSize, ACTIVATED_COLOR);
            DrawRectangleLinesEx(Rectangle{mPosition.x, mPosition.y, mSize.x, mSize.y}, BORDER_SIZE, ACTIVATED_BORDER_COLOR);
        }
        else if(mIsLongPressed){
            DrawRectangleV(mPosition, mSize, ACTIVATED_COLOR);
            DrawRectangleLinesEx(Rectangle{mPosition.x, mPosition.y, mSize.x, mSize.y}, BORDER_SIZE, ACTIVATED_BORDER_COLOR);
        }
        else if(mIsHovered){
            DrawRectangleV(mPosition, mSize, HOVERED_COLOR);
            DrawRectangleLinesEx(Rectangle{mPosition.x, mPosition.y, mSize.x, mSize.y}, BORDER_SIZE, HOVERED_BORDER_COLOR);
        }
        else{
            DrawRectangleV(mPosition, mSize, BASE_COLOR);
            DrawRectangleLinesEx(Rectangle{mPosition.x, mPosition.y, mSize.x, mSize.y}, BORDER_SIZE, BASE_BORDER_COLOR);
        }
    }
};

class Toggle : public GuiElement{
private:

public:
    Toggle(){}
    Toggle(const Vector2& rPosition, const Vector2& rSize) : GuiElement(rPosition, rSize){}

    void update(){
        updateHovered();
        updatePressed();
        updateDown();
        updateToggled();
        // TODO add functions for this specific element
    }

    void render(){
        if(mIsToggled){
            DrawRectangleV(mPosition, mSize, BASE_COLOR);
            DrawRectangleLinesEx(Rectangle{mPosition.x, mPosition.y, mSize.x, mSize.y}, 2, BASE_BORDER_COLOR);
            DrawLineEx(mPosition + Vector2{1,1}, mPosition + mSize, 2, BASE_BORDER_COLOR);
            DrawLineEx(mPosition + Vector2{mSize.x, 0.f}, mPosition + Vector2{0.f, mSize.y} + Vector2{1,-1}, 2, BASE_BORDER_COLOR);
        }
        else{
            DrawRectangleV(mPosition, mSize, BASE_COLOR);
            DrawRectangleLinesEx(Rectangle{mPosition.x, mPosition.y, mSize.x, mSize.y}, 2, BASE_BORDER_COLOR);
        }
    }
};



/*
TODO
ProgressBar
*/