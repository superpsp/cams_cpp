#include <string>
#include <fstream>

class File {
public:
	const short
		FILE_TXT = 0
		, FILE_JPG = 1
		, FILE_ZIP = 2
		, FILE_IO_OUT = 0
		, FILE_IO_IN = 1;

	File(std::string filePath, short filetype, short fileIOMode);
	void writeLine(std::string line); // Writes a line to FILE_TXT
	~File();

private:
	std::string path; // TODO Slash for Windows an UNIX
	std::fstream file;
	short
		mode
		, type;
};
