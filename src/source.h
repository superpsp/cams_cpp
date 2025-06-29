#pragma once
#include <string>
#include "text_file.h"

class Source {
	public:
		static const unsigned char
			SOURCE_FILE = 0
			, SOURCE_DB = 1
			, SOURCE_FILE_IP = 0;
		Source();
		~Source();
		void
			setSource(unsigned char source)
			, setFileName(unsigned char target, std::string name);
	private:
		unsigned char source;
		const std::string IP_FILE_NAME = "ips.txt";
		std::string ipFileName;
		FileText* ipFile = 0;
		void
			setDefaultParameters()
			, openFile(unsigned char target);
		bool checkFile(unsigned char target, unsigned char openResult);
};

