#pragma once
#include "Terminal.hpp"

#define INPUT_LEFT 37
#define INPUT_UP 72
#define INPUT_RIGHT 39
#define INPUT_DOWN 80
#define INPUT_ESC 27
#define INPUT_SPACE 32

class InputHandler {
public:
    static InputHandler& getInstance();
    InputHandler(const InputHandler&) = delete;
    InputHandler& operator=(const InputHandler&) = delete;

    bool handleInput();
private:
    InputHandler();
    
    View& _view;
    Terminal& _terminal;
};
