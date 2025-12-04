#include <conio.h>
#include <iostream>
#include "view.hpp"
#include "inputHandler.hpp"

InputHandler::InputHandler() : _terminal(Terminal::getInstance()), _view(View::getInstance()) {}

InputHandler& InputHandler::getInstance() {
    static InputHandler i;
    return i;
}

bool InputHandler::handleInput() {
    if(!_kbhit()) return true;
    
    int input = _getch();

    if(input == INPUT_ESC) {
        //TODO: execute shell script to actually move to that directory
        return false;
    }

    if(input == 0 || input == 224) { 
        input = _getch();          
        auto t_pos = _terminal.getTerminalPosition();  

        if(input == INPUT_UP) {
            int row = _terminal.getCursorRow();
            if(row == t_pos.first) _view.scroll(-1);

            _terminal.moveCursorUp(1);
            _view.display();
            _view.highlightRow(_terminal.getCursorRow());
        }
        else if(input == INPUT_DOWN) { 
            int row = _terminal.getCursorRow();
            int endRow = t_pos.first + _terminal.getTerminalSize().first - 1;

            if(row == endRow) {
                _view.scroll(1);
            } 
            else if(row == t_pos.first + _terminal.getFileNameBufferSize() - 1) {
                return true;
            };

            _terminal.moveCursorDown(1);
            //std::cerr << _terminal.getCursorRow();
            _view.display();
            _view.highlightRow(_terminal.getCursorRow());
        }
    }
    else if(input == INPUT_SPACE) {
        bool ok = _terminal.accessSelectedDir(_view.getTopIndex());
        if(!ok) return true;

        _view.update();
    }

    return true;
}