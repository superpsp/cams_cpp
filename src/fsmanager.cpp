#include "fsmanager.h"

FileSystemManager* fileSystemManagerInstance = 0;

FileSystemManagerDestructor::~FileSystemManagerDestructor() {
	delete fileSystemManagerInstance;
}

void FileSystemManagerDestructor::initialize(FileSystemManager* p) {
	fileSystemManagerInstance = p;
	if (!fileSystemManagerInstance->files.empty()) {
		for (std::map<std::string, std::fstream>::iterator it = fileSystemManagerInstance->files.begin(); it != fileSystemManagerInstance->files.end(); it++) {
			fileSystemManagerInstance->closeFile(it->second);
		}
	}
}

FileSystemManager& FileSystemManager::getInstance() {
	if (!fileSystemManagerInstance) {
		fileSystemManagerInstance = new FileSystemManager();
	}
	return *fileSystemManagerInstance;
}

void FileSystemManager::createFile(std::string fileName) {
	files[fileName] = std::fstream(fileName);
}

bool FileSystemManager::openFile(std::fstream &file, std::string fileName, short ioType) {
	if (ioType = FILE_MANAGER_FILE_OUT) {
		file.open(fileName, std::ios::out);
	}
	else {
		file.open(fileName, std::ios::out);
	}
	return file.is_open();
}

short FileSystemManager::writeToFile(std::string fileName, std::string line) {
	std::map<std::string, std::fstream>::iterator it = files.find(fileName);

	if (it == files.end()) {
		createFile(fileName);
		it = files.find(fileName);
	}
	if (!it->second.is_open()) {
		if (!openFile(it->second, it->first, FILE_MANAGER_FILE_OUT)) {
			return FILE_MANAGER_OPERATION_RESULT_NOT_OPENED;
		}
	}
	if (!it->second.good()) {
		return FILE_MANAGER_OPERATION_RESULT_NOT_GOOD;
	}
		
	it->second << line << std::endl;
	return FILE_MANAGER_OPERATION_RESULT_OK;
}

void FileSystemManager::closeFile(std::fstream& file) {
	if (file.is_open()) {
		file.close();
	}
}