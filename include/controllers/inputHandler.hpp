#pragma once

#include <raylib.h>

class InputHandler{
private:
        // Private constructor
    InputHandler() = default;

    // Disable copy and assignment
    InputHandler(const InputHandler&) = delete;
    InputHandler& operator=(const InputHandler&) = delete;
public:
    Vector2 movementInput;

    void updateMovementInput();

    // Access the singleton instance
    static InputHandler& getInstance();
};