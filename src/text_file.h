#pragma once
#include <fstream>
#include <mutex>
#include "file.h"

class FileText : public File {
public:
	 static const unsigned char
		FILE_ERROR_NOT_OPEN = 1
		, FILE_ERROR_NOT_GOOD = 2
		, FILE_ERROR_MODE_NOT_CORRECT = 3
		, FILE_ERROR_EXISTS = 5;
	const std::string FILE_READ_ERROR = "!!!!!!! 777 File read error: ";

	FileText(std::string path, unsigned char mode);
	~FileText();
	unsigned char writeLine(std::string line);
	std::string readLine();
	unsigned char rename(std::string path) override;
	unsigned char
		open()
		, copyToConsole();
	std::string getErrorMessage(unsigned char code) override;

private:
	std::fstream file;
	std::mutex fileMutex;

	unsigned char check() override;
};

