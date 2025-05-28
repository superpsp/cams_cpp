#include <string>
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
		, FILE_NOT_OUT = 3
		, FILE_NOT_TXT = 4;

	File(std::string filePath, short filetype, short fileIOMode);
	short writeLine(std::string line); // Writes a line to FILE_TXT
	~File();

private:
	std::string path; // TODO Slash for Windows an UNIX
	std::fstream file;
	static const short
		FILE_CHECK_FOR_WRITE = 0;
	short
		mode
		, type;
	short checkFile(short checkType);
};
