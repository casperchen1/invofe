#pragma once
#include <vector>
#include <string>
#include "Terminal.hpp"

#define BOX_TOP_LEFT "\xE2\x94\x8C"
#define BOX_TOP_RIGHT "\xE2\x94\x90"
#define BOX_BOTTOM_LEFT "\xE2\x94\x94"
#define BOX_BOTTOM_RIGHT "\xE2\x94\x98"
#define BOX_EDGE "\xE2\x94\x80"

#define ESC_ENABLE_BLINKING "\033[?12h";
#define ESC_DISABLE_BLINKING "\x1b[?12l";
#define ESC_HIDE_CURSOR "\033[?25l";
#define ESC_SHOW_CURSOR "\033[?25h";

#define ESC_FOLDER_COLOR "\033[31m"
#define ESC_COLOR_RESET "\033[0m"

class View {
public:
    //Singleton instance
    static View& getInstance();
    View(const View&) = delete;

    //Get the index of the top element in view
    int getTopIndex() const;

    //Draw the full app
    void display() const;
    //Reload _view_buffer, reposition the cursor, and redraw the full app
    void update();
    void drawHeader() const;
    void drawFileList() const;
    void drawOperations() const;
    //Scroll for n items, positive for scrolling down, negative for scrolling up
    void scroll(int n);
    void updateViewBuffer(int start, int end = -1);

    void highlightRow(int n);
    void updateRow(int n);
    void clearLine();
private:
    View();
    
    std::vector<std::string> _view_buffer;
    int _top_index;
    Terminal& _terminal;
};