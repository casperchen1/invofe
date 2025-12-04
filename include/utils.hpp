#pragma once
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

std::string padString(std::string str, int size, std::string padChar = " ", bool alignLeft = true);

std::string fileTimeToString(std::filesystem::file_time_type ftime);

std::string permsToString(fs::perms p);