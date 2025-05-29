#include "file.h"
#include "fsmanager.h"

#define FILE_SYSTEM_MANAGER FileSystemManager::getInstance()

File::File(std::string filePath, short filetype, short fileIOMode) {
	this->path = filePath;
	this->type = filetype;
	this->mode = fileIOMode;
	// TODO Register in FileSystemManager to avoid duplicates
	if (this->mode == FILE_IO_OUT) {
		this->file.open(this->path, std::ios::out);
	} else {
		this->file.open(this->path, std::ios::in);
	}
}

short File::writeLine(std::string line) {
	short checkResult = checkFile(FILE_CHECK_FOR_WRITE);
	if (checkResult != FILE_OK) {
		return checkResult;
	}
	this->file << line << std::endl;
	return FILE_OK;
}

short File::checkFile(short checkType) {
	if (!file.is_open()) {
		return FILE_NOT_OPENED;
	}
	if (!file.good()) {
		return FILE_NOT_GOOD;
	}
	if (checkType == FILE_CHECK_FOR_WRITE) {
		if (mode != FILE_IO_OUT) {
			return FILE_NOT_OUT;
		}
		if (type != FILE_TXT) {
			return FILE_NOT_TXT;
		}
	}
	return FILE_OK;
}

void File::fileClose() {
	file.close();
}

File::~File() {
	if (file.is_open()) {
		std::cout << "File " << path << " closed" << std::endl;
		fileClose();
	}
}