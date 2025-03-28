#include <inputHandler.hpp>

// Access the singleton instance
InputHandler& InputHandler::getInstance() {
    static InputHandler instance;
    return instance;
}

void InputHandler::updateMovementInput(){
        movementInput = {0,0};
        // Player input
        if(IsKeyDown(KEY_A)) movementInput.x += -1.f;
        if(IsKeyDown(KEY_D)) movementInput.x += 1.f;
        if(IsKeyDown(KEY_W)) movementInput.y += -1.f;
        if(IsKeyDown(KEY_S)) movementInput.y += 1.f;
    }