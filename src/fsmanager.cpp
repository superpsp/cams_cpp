#include <filesystem>
#include <iostream>
#include <set>
#include "fsmanager.h"

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

bool FileSystemManager::registerFile(std::string filePath) {
	if (!this->fileSet.empty()) {
		this->fileSetIterator = this->fileSet.find(filePath);
		if (this->fileSetIterator != this->fileSet.end()) {
			return false;
		}
	}
	fileSet.insert(filePath);
	return true;
}

void FileSystemManager::unRegisterFile(std::string filePath) {
	if (!this->fileSet.empty()) {
		this->fileSetIterator = this->fileSet.find(filePath);
		if (this->fileSetIterator != this->fileSet.end()) {
			this->fileSet.erase(this->fileSetIterator);
		}
	}
}

void FileSystemManager::renameFile(std::string source, std::string destination) {
	fs::rename(source, destination);
}

void FileSystemManager::deleteFile(std::string filePath) {
	fs::remove(filePath);
}

bool FileSystemManager::isFileEmpty(std::string filePath) {
	return (fs::is_empty(filePath));
}
