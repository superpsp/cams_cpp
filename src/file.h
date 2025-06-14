#include <string>
#include <iostream>
#include <fstream>

class File {
public:
	static const short
		FILE_TXT = 0
		, FILE_JPG = 1
		, FILE_ZIP = 2
		, FILE_IO_OUT = 0
		, FILE_IO_IN = 1
		, FILE_OK = 0
		, FILE_NOT_OPENED = 1
		, FILE_NOT_GOOD = 2
		, FILE_MODE_NOT_CORRECT = 3
		, FILE_TYPE_NOT_CORRECT = 4;

	File(std::string filePath, short filetype, short fileIOMode);
	short writeLine(std::string line);
	std::string readLine();
	void fileClose();
	~File();
	short checkFile();
	short checkFile(short checkType, short checkMode);

private:
	std::string path; // TODO Slash for Windows an UNIX
	std::fstream file;
	static const short
		FILE_CHECK_FOR_WRITE = 0;
	short
		mode
		, type;
};
