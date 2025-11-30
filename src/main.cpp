#include <iostream>
#include <windows.h>
#include "view.hpp"
#include "inputHandler.hpp"
#include "Terminal.hpp"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::cerr << "\x1b[?1049h";

    View& view = View::getInstance();
    Terminal& terminal = Terminal::getInstance();
    InputHandler& ihandler = InputHandler::getInstance();

    view.display();
    view.highlightRow(terminal.getTerminalPosition().first);

    while(ihandler.handleInput()) {
    }

    std::cerr << "\x1b[?1049l";
    std::cerr << ESC_ENABLE_BLINKING;
    std::cerr << ESC_SHOW_CURSOR;
    std::cerr << ESC_CLEAR;
    terminal.moveCursorTo(1, 1);
}