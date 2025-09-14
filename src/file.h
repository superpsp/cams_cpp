#pragma once
#include <string>
#include <filesystem>

class File {
public:
	static const unsigned char
		FILE_OK = 0
		, FILE_TXT = 0
		, FILE_JPG = 1
		, FILE_ZIP = 2
		, FILE_IO_OUT = 0
		, FILE_IO_IN = 1
		, FILE_IO_APPEND = 2
		//, FILE_ERROR_NOT_OPEN = 1
		//, FILE_ERROR_NOT_GOOD = 2
		//, FILE_ERROR_MODE_NOT_CORRECT = 3
		, FILE_TYPE_NOT_CORRECT = 4
		, FILE_ERROR_EXISTS = 5
		, FILE_ERROR_NOT_EXISTS = 6
		, FILE_ERROR_RENAME = 7;
	const std::string FILE_ERROR_UNSUPPORTED = " is unsupported error";

	uintmax_t getFileSize(std::string path);

	~File();

protected:
	void
		setPath(std::string path)
		, setMode(unsigned char mode)
		, setType(unsigned char type)
		, deleteFile();
	std::string getPath();
	unsigned char
		getMode()
		, getType();
	virtual unsigned char
		check()
		, rename(std::string path);
	virtual std::string getErrorMessage(unsigned char code);

private:
	std::string path; // TODO Slash for Windows an UNIX
	unsigned char
		type
		, mode;
};
