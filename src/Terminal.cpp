#include <iostream>
#include "Terminal.hpp"
#include "view.hpp"
#include "utils.hpp"

Terminal& Terminal::getInstance() {
    static Terminal t;
    return t;
}

Terminal::Terminal() : _cursor_row(START_ROWS), _cursor_col(START_COLS), _f_manager(FileManager::getInstance()) {
    loadFileBuffer();
}

std::pair<int, int> Terminal::getTerminalPosition() const {
    return {START_ROWS, START_COLS};
}

std::pair<int, int> Terminal::getTerminalSize() const {
    return {HEIGHT, WIDTH};
}

int Terminal::getCursorRow() const {
    return _cursor_row;
}

int Terminal::getCursorCol() const {
    return _cursor_col;
}

size_t Terminal::getFileNameBufferSize() const {
    return _filename_buffer.size();
}

void Terminal::moveCursorTo(int row, int col) {
    //if(row > END_ROWS || row < START_ROWS || col > END_COLS || col < START_COLS) return;
    _cursor_row = row;
    _cursor_col = col;

    std::cerr << "\x1b[" << _cursor_row << ";" << _cursor_col << "H";
}

void Terminal::moveCursorUp(int n) {
    if(_cursor_row - n < START_ROWS) return;
    std::cerr << "\x1b[" << n << "A";
    _cursor_row -= n;
}

void Terminal::moveCursorDown(int n) {
    if(_cursor_row + n > END_ROWS) return;
    std::cerr << "\x1b[" << n << "B";
    _cursor_row += n;
}

void Terminal::loadFileBuffer() {
    _filename_buffer.clear();
    auto files = _f_manager.loadFileBuffer();
    _current_path = _f_manager.getCurrentPath();
    
    for(const auto& file : files) {
        if(file.type == "dir") {
            _filename_buffer.push_back("/" + file.name);
        }
        else {
            _filename_buffer.push_back(file.name);
        }
    }
}

bool Terminal::accessSelectedDir(int top_index) {
    int index = top_index + _cursor_row - START_ROWS;

    bool ok = _f_manager.accessDir(index);

    if(ok) loadFileBuffer();
    return ok;
}