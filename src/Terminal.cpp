#include <iostream>
#include <string>
#include "Terminal.hpp"
#include "view.hpp"
#include "utils.hpp"

Terminal& Terminal::getInstance() {
    static Terminal t;
    return t;
}

Terminal::Terminal() : _cursor_row(START_ROWS), _cursor_col(START_COLS), _f_manager(FileManager::getInstance()) {
    _current_path = _f_manager.getCurrentPath();
    loadFileNameBuffer();
    loadMetaDataBuffer();
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

void Terminal::loadFileNameBuffer() {
    _filename_buffer.clear();
    auto files = _f_manager.getFileBuffer();
    
    for(const auto& file : files) {
        if(file.meta_data.type == "dir" || file.meta_data.type == "parent") {
            _filename_buffer.push_back("/" + file.name);
        }
        else {
            _filename_buffer.push_back(file.name);
        }
    }
}

void Terminal::loadMetaDataBuffer() {
    _metadata_buffer.clear();
    auto files = _f_manager.getFileBuffer();

    for(const auto& file : files) {
        if(file.meta_data.type == "parent") {
            _metadata_buffer.push_back({});
            continue;
        }
        std::vector<std::string> metaData;

        //File type
        std::string type = " Type: " + file.meta_data.type;

        //File size
        std::string fileSize = " File Size: " + std::to_string(file.meta_data.file_size) + " B";

        //Last write time
        std::string lastWrite = " Last Write: " + fileTimeToString(file.meta_data.last_write_time);

        //Perms
        std::string perms = " Perms: " + permsToString(file.meta_data.perms);

        if(file.meta_data.type == "dir") {
            metaData.push_back(type);
            metaData.push_back(lastWrite);
            metaData.push_back(perms);
        } else {
            metaData.push_back(type);
            metaData.push_back(fileSize);
            metaData.push_back(lastWrite);
            metaData.push_back(perms);
        }

        _metadata_buffer.push_back(metaData);
    }
}

bool Terminal::accessSelectedDir(int top_index) {
    int index = top_index + _cursor_row - START_ROWS;

    bool ok = _f_manager.accessDir(index);

    if(ok) {
        loadFileNameBuffer();
        loadMetaDataBuffer();
        _current_path = _f_manager.getCurrentPath();
    }
    return ok;
}