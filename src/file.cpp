#include "file.h"
#include "fsmanager.h"

#define FSMANAGER FileSystemManager::getInstance()

File::File(std::string filePath, short filetype, short fileIOMode) {
	this->path = filePath;
	this->type = filetype;
	this->mode = fileIOMode;
}

bool File::fileOpen() {
	if (!FSMANAGER.registerFile(path)) {
		return false;
	}
	if (mode == FILE_IO_OUT) {
		file.open(path, std::ios::out);
	} else if (mode == FILE_IO_IN) {
		file.open(path, std::ios::in);
	} else if (mode == FILE_IO_APPEND) {
		file.open(path, std::ios::app);
	} else {
		return false;
	}
	return true;
}

short File::writeLine(std::string line) {
	short checkResult = checkFile(FILE_TXT, FILE_IO_OUT, false);
	if (checkResult != FILE_OK) {
		return checkResult;
	}
	this->file << line << std::endl;
	return FILE_OK;
}

std::string File::readLine() {
	short checkResult = checkFile(FILE_TXT, FILE_IO_IN, false);
	if (checkResult != FILE_OK) {
		return std::to_string(checkResult);
	}
	std::string line;
	std::getline(file, line);
	return line;
}

short File::getMode() {
	return mode;
}

short File::checkFile() {
	return checkFile(0, 0, true);
}

short File::checkFile(short checkType, short checkMode, bool isStatusOnly) {
	if (!file.is_open()) {
		return FILE_NOT_OPENED;
	}
	if (!file.good()) {
		return FILE_NOT_GOOD;
	}
	if (!isStatusOnly) {
		if (type != checkType) {
			return FILE_TYPE_NOT_CORRECT;
		}
		if (checkMode == FILE_IO_IN && mode != checkMode
			|| checkMode == FILE_IO_OUT && mode != FILE_IO_OUT && mode != FILE_IO_APPEND) {
			return FILE_MODE_NOT_CORRECT;
		}
	}
	return FILE_OK;
}

void File::fileClose() {
	FSMANAGER.unRegisterFile(path);
	file.close();
}

File::~File() {
	if (file.is_open()) {
		fileClose();
	}
}