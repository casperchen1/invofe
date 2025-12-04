#include <sstream>
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

std::string fileTimeToString(fs::file_time_type ftime) {
    using namespace std::chrono;

    // convert to system_clock
    auto sctp = time_point_cast<system_clock::duration>(
        ftime - std::filesystem::file_time_type::clock::now()
        + system_clock::now()
    );

    // to time_t
    std::time_t tt = system_clock::to_time_t(sctp);

    // format
    std::stringstream ss;
    ss << std::put_time(std::localtime(&tt), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string permsToString(fs::perms p) {
    auto check = [&](fs::perms bit, char c) {
        return (p & bit) != fs::perms::none ? c : '-';
    };

    std::string s;
    s += check(fs::perms::owner_read,  'r');
    s += check(fs::perms::owner_write, 'w');
    s += check(fs::perms::owner_exec,  'x');

    s += check(fs::perms::group_read,  'r');
    s += check(fs::perms::group_write, 'w');
    s += check(fs::perms::group_exec,  'x');

    s += check(fs::perms::others_read,  'r');
    s += check(fs::perms::others_write, 'w');
    s += check(fs::perms::others_exec,  'x');

    return s;
}
