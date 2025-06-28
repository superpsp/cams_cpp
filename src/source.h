#pragma once
class Source {
	public:
		static const unsigned char
			SOURCE_FILE = 0
			, SOURCE_DB = 1;
		void setSource(unsigned char source);
	private:
		unsigned char source;
};

