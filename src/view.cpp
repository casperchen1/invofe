#include <iostream>
#include "view.hpp"
#include "utils.hpp"

View::View() : _terminal(Terminal::getInstance()), _top_index(0) {
    _view_buffer.reserve(_terminal.HEIGHT);
    updateViewBuffer(0, _terminal.HEIGHT);
    std::cerr << ESC_CLEAR;
    std::cerr << ESC_DISABLE_BLINKING;
    std::cerr << ESC_HIDE_CURSOR;
}

View& View::getInstance() {
    static View v;
    return v;
}

int View::getTopIndex() const {
    return _top_index;
}

void View::updateViewBuffer(int start, int end) {
    if(end == -1) end = _terminal.HEIGHT;

    _view_buffer.clear();
    _view_buffer.reserve(_terminal.HEIGHT);
    int bufferSize = (int)_terminal.getFileNameBufferSize();

    for(int i = start; i < end && i < bufferSize; i++) {
        _view_buffer.push_back(_terminal._filename_buffer[i]);
    }
}

void View::clearLine() {
    int row = _terminal.getCursorRow();
    int col = _terminal.getCursorCol();

    std::cerr << ESC_CLEAR_LINE;

    _terminal.moveCursorTo(row, col);
}

void View::highlightRow(int n) {
    if(n >= _terminal.START_ROWS + _view_buffer.size() || n < _terminal.START_ROWS) return;

    int row = _terminal.getCursorRow();
    int col = _terminal.getCursorCol();

    _terminal.moveCursorTo(n, _terminal.START_COLS);
    std::string line = _view_buffer[n - _terminal.START_ROWS];
    clearLine();

    std::cerr << '|' << _terminal.ICON;
    std::cerr << ESC_HIGHLIGHT;
    std::string padded = padString(line, _terminal.WIDTH - 2 - 2);
    std::cerr << " " << padded;
    std::cerr << ESC_HIGHLIGHT_RESET;
    std::cerr << "|\n";

    _terminal.moveCursorTo(row, col);
}

void View::updateRow(int n) {
    if(n >= _terminal.START_ROWS + _view_buffer.size()) return;

    int row = _terminal.getCursorRow();
    int col = _terminal.getCursorCol();

    _terminal.moveCursorTo(n, _terminal.START_COLS);
    std::string line = _view_buffer[n - _terminal.START_ROWS];
    clearLine();

    std::string padded = padString(line, _terminal.WIDTH - 4);
    std::cerr << "|  " << padded << "|\n";

    _terminal.moveCursorTo(row, col);
}

void View::drawHeader() const {
    std::cerr << BOX_TOP_LEFT + padString("", _terminal.WIDTH - 2, BOX_EDGE) + BOX_TOP_RIGHT + "\n";
    std::string path = _terminal._current_path;
    std::string paddedPath = padString(path, _terminal.WIDTH - 2);
    std::cerr << '|' << paddedPath << "|\n";
    std::cerr << BOX_BOTTOM_LEFT + padString("", _terminal.WIDTH - 2, BOX_EDGE) + BOX_BOTTOM_RIGHT + "\n";
}

void View::drawFileList() const {
    //
    //TODO: change color for folders
    //
    int viewBufferSize = (int)_view_buffer.size();
    auto [row, col] = _terminal.getTerminalPosition();
    for(int i = 0; i < _terminal.HEIGHT; i++) {
        _terminal.moveCursorTo(row + i, col);
        std::string line = "";
        if(i < viewBufferSize) {
            line = _view_buffer[i];
        }
        std::string paddedLine = padString(line, _terminal.WIDTH - 4);
        std::cerr << "|  " << paddedLine << "|";
    }
    _terminal.moveCursorTo(_terminal.END_ROWS + 1, col);
    std::cerr << BOX_BOTTOM_LEFT + padString("", _terminal.WIDTH - 2, BOX_EDGE) + BOX_BOTTOM_RIGHT + "\n";
}

void View::drawOperations() const {
    int row = _terminal.getCursorRow();
    int col = _terminal.getCursorRow();
    _terminal.moveCursorTo(_terminal.END_ROWS + 2, _terminal.START_COLS);

    std::cerr << "[SPACE] SELECT  [UP] MOVE UP  [DOWN] MOVE DOWN  [ESC] EXIT";

    _terminal.moveCursorTo(row, col);
}

void View::display() const {
    _terminal.moveCursorTo(1, 1);
    drawHeader();

    _terminal.moveCursorTo(_terminal.START_ROWS, _terminal.START_COLS);
    drawFileList();
    _terminal.moveCursorTo(_terminal.START_ROWS, _terminal.START_COLS);
    drawOperations();
}

void View::update() {
    updateViewBuffer(0, -1);
    _top_index = 0;

    display();

    highlightRow(_terminal.START_ROWS);
    _terminal.moveCursorTo(_terminal.START_ROWS, _terminal.START_COLS);
}

void View::scroll(int n) {
    int bufferSize = (int)_terminal.getFileNameBufferSize();
    int row = _terminal.getCursorRow();
    int col = _terminal.getCursorCol();

    _top_index = std::min(std::max(0, _top_index + n), std::max(bufferSize - _terminal.HEIGHT, 0));
    updateViewBuffer(_top_index, _top_index + _terminal.HEIGHT);
    display();
    _terminal.moveCursorTo(row, col);
}
