#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

struct FileContent {
    std::string name;
    std::string type; 
    fs::path absolute_path;
};

class FileManager {
public:
    //Singleton instance
    static FileManager& getInstance();
    FileManager& operator=(const FileManager&) = delete;

    //Access the directory inside _file_buffer
    bool accessDir(int index);
    //Returns the _flie_buffer
    std::vector<FileContent> loadFileBuffer();

    std::string getCurrentPath();
    //Fetch and update _file_buffer for current directory
    void fetchAllContentInPath(fs::path = fs::path());
private:
    FileManager();
    std::vector<FileContent> _file_buffer;
};