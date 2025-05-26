#include "file.h"
#include "fsmanager.h"

#define FILE_SYSTEM_MANAGER FileSystemManager::getInstance()

File::File(std::string filePath, short filetype, short fileIOMode) {
	this->filePath = filePath;
	this->file = std::fstream(filePath);
	this->mode = fileIOMode;
	if (this->mode == FILE_IO_OUT) {
		this->file.open(this->filePath, std::ios::out);
	} else {
		this->file.open(this->filePath, std::ios::in);
	}
}

void File::writeLine(std::string line) {
	this->file << line << std::endl;
}

File::~File() {
	if (file.is_open()) {
		file.close();
	}
}