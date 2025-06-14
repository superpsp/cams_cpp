#include <filesystem>
#include "fsmanager.h"
#include <iostream>

namespace fs = std::filesystem;

FileSystemManager* fileSystemManagerInstance = 0;

FileSystemManagerDestructor::~FileSystemManagerDestructor() {
	delete fileSystemManagerInstance;
}

void FileSystemManagerDestructor::initialize(FileSystemManager* p) {
	fileSystemManagerInstance = p;
}

FileSystemManager& FileSystemManager::getInstance() {
	if (!fileSystemManagerInstance) {
		fileSystemManagerInstance = new FileSystemManager();
	}
	return *fileSystemManagerInstance;
}

bool FileSystemManager::isFileExisting(std::string filePath) {
	return fs::exists(filePath);
}

void FileSystemManager::copyFile(std::string source, std::string destination) {
	fs::copy_file(source, destination);
}

void FileSystemManager::deleteFile(std::string filePath) {
	fs::remove(filePath);
}

bool FileSystemManager::isFileEmpty(std::string filePath) {
	return (fs::is_empty(filePath));
}
