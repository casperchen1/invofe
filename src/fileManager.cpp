#include <iostream>
#include "fileManager.hpp"

FileManager& FileManager::getInstance() {
    static FileManager f;
    return f;
}

FileManager::FileManager() {
    fetchAllContentInPath();
}

std::string FileManager::getCurrentPath() {
    return fs::current_path().generic_string();
}

void FileManager::fetchAllContentInPath(fs::path path) {
    if(path.empty()) path = fs::current_path();
    _file_buffer.clear();

    if(path.has_parent_path()) _file_buffer.push_back({"..", FileMetaData{"parent"}});

    for(const auto& entry : fs::directory_iterator(path)) {
        std::string type = "";

        if(entry.is_directory()) type = "dir";
        else if(entry.is_regular_file()) type = "file";
        
        _file_buffer.push_back({
            entry.path().filename().generic_string(), 
            FileMetaData{
                type, 
                entry.file_size(), 
                entry.last_write_time(), 
                entry.status().permissions()
            }});
    }
}

std::vector<FileContent> FileManager::getFileBuffer() {
    return _file_buffer;
}

bool FileManager::accessDir(int index) {
    if(index >= _file_buffer.size()) return false;

    FileContent content = _file_buffer[index];
    if(content.meta_data.type == "dir" || content.meta_data.type == "parent") {
        fs::current_path(content.name);
        fetchAllContentInPath();
        return true;
    } 
    return false;
}