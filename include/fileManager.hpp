#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

struct FileMetaData {
    std::string type;
    uintmax_t file_size;
    fs::file_time_type last_write_time;
    fs::perms perms;
};

struct FileContent {
    std::string name;
    FileMetaData meta_data;
};

class FileManager {
public:
    //Singleton instance
    static FileManager& getInstance();
    FileManager& operator=(const FileManager&) = delete;

    //Access the directory inside _file_buffer
    bool accessDir(int index);
    //Returns the _flie_buffer
    std::vector<FileContent> getFileBuffer();

    std::string getCurrentPath();
    //Fetch and update _file_buffer for current directory
    void fetchAllContentInPath(fs::path = fs::path());
private:
    FileManager();
    std::vector<FileContent> _file_buffer;
};