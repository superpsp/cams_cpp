#include "file.h"
#include "fsmanager.h"

#define FILE_SYSTEM_MANAGER FileSystemManager::getInstance()

File::File(std::string filePath, short filetype, short fileIOMode) {
	this->path = filePath;
	this->type = filetype;
	this->file = std::fstream(filePath);
	this->mode = fileIOMode;
	// TODO Register in FileSystemManager to avoid duplicates
	if (this->mode == FILE_IO_OUT) {
		this->file.open(this->path, std::ios::out);
	} else {
		this->file.open(this->path, std::ios::in);
	}
}

void File::writeLine(std::string line) {
	if (this->mode == FILE_IO_OUT && this->type == FILE_TXT) {
		this->file << line << std::endl;
	} else {
		throw "[ERROR] File::writeLine: Either " + this->path + " is opened for input or it is not a .txt file";
	}
}

File::~File() {
	if (file.is_open()) {
		file.close();
	}
}