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
        , FILE_MANAGER_OPERATION_RESULT_NOT_GOOD = 1
        , FILE_MANAGER_OPERATION_RESULT_FILE_IS_OPENED = 2;
    static FileSystemManager& getInstance();
    short writeToFile(std::string fileName, std::string line);
    std::string readFromFile(std::string fileName);
    short printFile(std::string fileName);
    //short changeFileModeForomOutToIn(std::string fileName);
    void closeFile(std::string fileName);
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
    std::string readFromFile(std::fstream& file);
    void closeFile(std::fstream& file);
};
