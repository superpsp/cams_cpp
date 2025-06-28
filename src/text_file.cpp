#include <iostream>
#include "text_file.h"

FileText::FileText(std::string path, unsigned char mode) {
	setType(FILE_TXT);
	setPath(path);
	setMode(mode);
}

unsigned char FileText::open() {
	unsigned char result;
	if (getMode() == FILE_IO_OUT) {
		deleteFile();
	}
	result = File::check();
	if (result == FILE_OK) {
		if (getMode() == FILE_IO_OUT) {
			file.open(getPath(), std::ios::out);
			file.close();
			setMode(FILE_IO_APPEND);
		} 
		if (getMode() == FILE_IO_IN) {
			file.open(getPath(), std::ios::in);
		}
		if (getMode() == FILE_IO_APPEND) {
			file.open(getPath(), std::ios::app);
		}
		result = check();
	}
	return result;
}

unsigned char FileText::check() {
	unsigned char result = File::check();
	if (result == FILE_OK) {
		if (!file.is_open()) {
			result = FILE_ERROR_NOT_OPEN;
		}
		else if (getMode() == FILE_IO_IN && !file.good()) {
			result = FILE_ERROR_NOT_GOOD;
		}
	}
	return result;
}

std::string FileText::getErrorMessage(unsigned char code) {
	std::string result = File::getErrorMessage(code);
	if (result.compare(FILE_ERROR_UNSUPPORTED) == 0) {
		switch (code) {
			case FILE_ERROR_NOT_OPEN:
				result = " is not opened";
				break;
			case FILE_ERROR_NOT_GOOD:
				result = " is not good";
				break;
			default:
				result = FILE_ERROR_UNSUPPORTED;
		}
	}
	return result;
}

unsigned char FileText::writeLine(std::string line) {
	if (getMode() == FILE_IO_IN) {
		return FILE_ERROR_MODE_NOT_CORRECT;
	}
	unsigned char result = check();
	if (result == FILE_OK) {
		file << line << std::endl;
	}
	result = check();
	return result;
}

std::string FileText::readLine() {
	if (getMode() == FILE_IO_OUT) {
		return FILE_READ_ERROR + std::to_string(FILE_ERROR_MODE_NOT_CORRECT);
	}
	std::string line;
	unsigned char result = check();
	if (result != FILE_OK) {
		return FILE_READ_ERROR + std::to_string(result);
	}
	std::getline(file, line);
	return line;
}

unsigned char FileText::rename(std::string path) {
	unsigned char result = check();
	if (result == FILE_OK) {
		file.close();
		File::rename(path);
		setPath(path);
		setMode(FILE_IO_APPEND);
		open();
	}
	return result;
}

unsigned char FileText::copyToConsole() {
	unsigned char result = check();
	if (result == FILE_OK) {
		unsigned char oldMode = getMode();
		if (file.is_open()) {
			file.close();
		}
		std::string line;
		setMode(FILE_IO_IN);
		open();
		while (result == FILE_OK) {
			std::getline(file, line);
			std::cout << line << std::endl;
			result = check();
		}
		file.close();
		setMode(FILE_IO_APPEND);
		open();
		result = check();
	}
	return result;
}

FileText::~FileText() {
	if (file.is_open()) {
		file.close();
	}
}
