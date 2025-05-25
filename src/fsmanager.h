#include <iostream>
#include <fstream>
#include <map>

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
    const short
        FILE_MANAGER_OPERATION_RESULT_OK = 0
        , FILE_MANAGER_OPERATION_RESULT_NOT_OPENED = 0
        , FILE_MANAGER_OPERATION_RESULT_NOT_GOOD = 0;
    static FileSystemManager& getInstance();
    short writeToFile(std::string fileName, std::string line);
protected:
    FileSystemManager() {}
    FileSystemManager(const FileSystemManager&);
    FileSystemManager& operator = (FileSystemManager&);
    ~FileSystemManager() {}
    friend class FileSystemManagerDestructor;
private:
    const short
        FILE_MANAGER_FILE_OUT = 0
        , FILE_MANAGER_FILE_IN = 1;
        static FileSystemManagerDestructor destructor;
    std::map<std::string, std::fstream> files;
    void createFile(std::string fileName);
    bool openFile(std::fstream& file, std::string fileName, short ioType);
};
