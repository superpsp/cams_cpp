#include <string>

class FileSystemManager;

class FileSystemManagerDestructor {
public:
    ~FileSystemManagerDestructor();
    void initialize(FileSystemManager* p);
private:
    FileSystemManager* fileSystemManagerInstance;
};

class FileSystemManager {
public:
    static FileSystemManager& getInstance();
    bool
        isFileExisting(std::string filePath)
        , isFileEmpty(std::string filePath);
    void
        copyFile(std::string source, std::string destination)
        , deleteFile(std::string filePath);
protected:
    FileSystemManager() {}
    FileSystemManager(const FileSystemManager&);
    FileSystemManager& operator = (FileSystemManager&);
    ~FileSystemManager() {}
    friend class FileSystemManagerDestructor;
private:
    static FileSystemManagerDestructor destructor;
};
