#pragma once

#include <raylib.h>
#include <raymath.h>

#define LUFUGUI_MAX_CHARS 64
#define BASE_COLOR BLUE

class GuiElement{
    public:
    Vector2 mPosition;
    Vector2 mSize;

    bool mIsHovered = false;
    bool mIsPressed = false;
    bool mIsLongPressed = false;
    bool mIsSelected = false;

    GuiElement(const Vector2& rPosition, const Vector2& rSize):mPosition{rPosition}, mSize{rSize}{}
    virtual ~GuiElement(){}

    virtual void render() = 0;

    virtual void update(){
        updateHovered();
        updatePressed();
        updateDown();
        updateSelect();
    }

    private:
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
};

class Button : public GuiElement{
private:
    /* data */
public:
    using GuiElement::GuiElement;

    void update(){
        GuiElement::update();
        // TODO add functions for this specific element
    }

    void render(){
        Color color = BASE_COLOR;
        if(mIsHovered){
            color = ColorBrightness(color, -0.2f);
        }
        if(mIsPressed){
            color = ColorBrightness(color, -0.4f);
        }
        DrawRectangleV(mPosition, mSize, color);
        DrawRectangleLinesEx(Rectangle{mPosition.x, mPosition.y, mSize.x, mSize.y}, 2, BLACK);
    }
};

class Toggle : public GuiElement{
private:

public:
    bool mIsToggled = false;

    using GuiElement::GuiElement;

    void update(){
        GuiElement::update();
        updateToggled();
        // TODO add functions for this specific element
    }

    void render(){
        Color color = BASE_COLOR;
        DrawRectangleV(mPosition, mSize, color);

        if(mIsToggled){
            DrawLineEx(mPosition, mPosition + mSize, 2, BLACK);
            DrawLineEx(mPosition + Vector2{mSize.x, 0.f}, mPosition + Vector2{0.f, mSize.y}, 2, BLACK);
            
        }
        DrawRectangleLinesEx(Rectangle{mPosition.x, mPosition.y, mSize.x, mSize.y}, 2, BLACK);
    }

    // Check if the element is Selected. Pressed and Hovered must be called before this.
    void updateToggled(){
        if(mIsHovered && mIsPressed){
            mIsToggled = !mIsToggled;
        }
    }

};

class TextBox : public GuiElement{
private:
    char text[LUFUGUI_MAX_CHARS + 1] = "\0";
    int keyCount = 0;
    float mTextSize = 20;
    Font& mFont;

public:
    TextBox(const Vector2& rPosition, const Vector2& rSize, const float& rtextSize, Font& rFont) : GuiElement(rPosition, rSize), mTextSize(rtextSize), mFont(rFont) {}
    //using GuiElement::GuiElement;

    void update(){
        GuiElement::update();
        updateText();
        // TODO add functions for this specific element
    }

    void render(){
        DrawRectangleV(mPosition, mSize, BASE_COLOR);
        DrawTextEx(mFont, text, mPosition + Vector2{2,0}, mTextSize, 1, BLACK);
        DrawRectangleLinesEx(Rectangle{mPosition.x, mPosition.y, mSize.x, mSize.y}, 2, BLACK);
    }

    void updateText(){
        int key = GetCharPressed();
        // Only type when element is selected
        if(mIsSelected){
            // Check if max chars is not reached
            if(keyCount < LUFUGUI_MAX_CHARS){
                // Filter which keys can be typed
                if ((key >= 48) && (key <= 57) || (key >= 65) && (key <= 90) || (key >= 97) && (key <= 122) || (key == 45) || (key == 95)){ // Allow key 0-9 || A-Z || a-z || - || _
                    // Check if any key is pressed
                    if(key != 0){
                        text[keyCount] = (char)key;
                        keyCount ++;
                    }
                }
            }
        }

        if(IsKeyPressed(KEY_BACKSPACE)){
            // Check if text is already empty and nothing can be erased
            if(keyCount > 0){
                keyCount --;
                text[keyCount] = '\0';
            }
        }
    }
};