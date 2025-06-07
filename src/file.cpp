#include "file.h"

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
	short checkResult = checkFile(FILE_TXT, FILE_IO_OUT);
	if (checkResult != FILE_OK) {
		return checkResult;
	}
	this->file << line << std::endl;
	return FILE_OK;
}

std::string File::readLine() {
	short checkResult = checkFile(FILE_TXT, FILE_IO_IN);
	if (checkResult != FILE_OK) {
		return std::to_string(checkResult);
	}
	std::string line;
	std::getline(file, line);
	return line;
}

short File::checkFile(short checkType, short checkMode) {
	if (!file.is_open()) {
		return FILE_NOT_OPENED;
	}
	if (!file.good()) {
		return FILE_NOT_GOOD;
	}
	if (type != checkType) {
		return FILE_TYPE_NOT_CORRECT;
	}
	if (mode != checkMode) {
		return FILE_MODE_NOT_CORRECT;
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