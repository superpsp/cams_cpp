#include "file.h"

namespace fs = std::filesystem;

void File::setPath(std::string path) {
	this->path = path;
}

std::string File::getPath() {
	return path;
}

void File::setMode(unsigned char mode) {
	this->mode = mode;
}

unsigned char File::getMode() {
	return mode;
}

void File::setType(unsigned char type) {
	this->type = type;
}

unsigned char File::getType() {
	return type;
}
unsigned char File::check() {
	unsigned char result = FILE_OK;

	if (fs::exists(path)) {
		if (mode == FILE_IO_OUT && mode != FILE_IO_APPEND) {
			result = FILE_ERROR_EXISTS;
		}
	}
	else {
		if (mode == FILE_IO_IN) {
			result = FILE_ERROR_NOT_EXISTS;
		}
	}
	return result;
}

std::string File::getErrorMessage(unsigned char code) {
	std::string result;
	switch (code) {
	case FILE_ERROR_EXISTS:
		result = " is existing";
		break;
	case FILE_ERROR_NOT_EXISTS:
		result = " is not existing";
		break;
	case FILE_ERROR_RENAME:
		result = " can't be renamed (either source file is not existing or taget file is existing)";
		break;
	default:
		result = FILE_ERROR_UNSUPPORTED;
	}
	return result;
}

uintmax_t File::getFileSize(std::string path) {
	if (fs::exists(path)) {
		return fs::file_size(path);
	}
	return 0;
}

unsigned char File::rename(std::string path) {
	unsigned char result = FILE_OK;
	if (this->path.compare(path) != 0) {
		if (!fs::exists(path) && fs::exists(this->path)) {
			fs::rename(this->path, path);
			this->path = path;
		}
		else {
			result = FILE_ERROR_RENAME;
		}
	}
	return result;
}

void File::deleteFile() {
	if (fs::exists(path)) {
		fs::remove(path);
	}
}

File::~File() {
}
