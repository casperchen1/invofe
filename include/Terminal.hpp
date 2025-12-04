#pragma once
#include <vector>
#include <string>
#include "fileManager.hpp"

#define ESC_CLEAR "\x1b[2J"
#define ESC_CLEAR_LINE "\x1b[K"
#define ESC_HIGHLIGHT "\033[7m"
#define ESC_HIGHLIGHT_RESET "\033[0m"
#define ESC_CURSOR_UP "\x1b[1A";
#define ESC_CURSOR_DOWN "\x1b[1B";

class View;

class Terminal {
public:
    //Singleton instance
    static Terminal& getInstance();
    Terminal(const Terminal&) = delete;
    Terminal& operator=(const Terminal&) = delete;

    int getCursorRow() const;
    int getCursorCol() const;

    //<row, col>
    std::pair<int, int> getTerminalPosition() const;
    std::pair<int, int> getTerminalSize() const;
    size_t getFileNameBufferSize() const;

    void moveCursorTo(int row, int col);
    void moveCursorUp(int n);
    void moveCursorDown(int n);

    //Update the _filename_buffer
    void loadFileNameBuffer();
    void loadMetaDataBuffer();

    //Try to access the selected directory, takes in the index of the top element in view
    bool accessSelectedDir(int top_index);
private:
friend View;
    Terminal();

    FileManager& _f_manager;

    std::string _current_path = "";

    //Stores the filenames in the current directory
    std::vector<std::string> _filename_buffer;
    std::vector<std::vector<std::string>> _metadata_buffer;

    const int WIDTH = 60;
    const int HEIGHT = 20;
    const int START_ROWS = 4;
    const int END_ROWS = START_ROWS + HEIGHT - 1;
    const int START_COLS = 1;
    const int END_COLS = START_COLS + WIDTH - 1;
    const char ICON = '>';

    const int META_ATTRIBUTES = 4;
    const int META_WIDTH = 61;
    const int META_HEIGHT = META_ATTRIBUTES;
    const int META_START_ROWS = END_ROWS + 2;
    const int META_END_ROWS = META_START_ROWS + META_HEIGHT - 1;
    const int META_START_COLS = 1;
    const int META_END_COLS = META_START_COLS + META_WIDTH - 1;
    
    //Cursor position
    int _cursor_row;
    int _cursor_col;
};