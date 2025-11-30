#include "utils.hpp"

std::string padString(std::string str, int size, std::string padChar, bool alignLeft) {
    std::string safeStr = str.substr(0, size);

    std::string pad = "";

    for(int i = 0; i < size - safeStr.length(); i++) {
        pad += padChar;
    }

    if(alignLeft) {
        return safeStr + pad;
    }
    else {
        return pad + safeStr;
    }
}