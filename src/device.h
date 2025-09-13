#pragma once
#include "login.h"
class Device {
	public:
		Device(unsigned char mode);
		void stop();
		unsigned long getIp() const;
		unsigned char getMode();
		~Device();
		static const unsigned char MODE_BRUT = 0;
	protected:
			unsigned long ip = 0;
	private:
		Login* login;
		bool
			deviceFromQueueFlag = false
			, knownLoginFlag = false;
		void
			setIp()
			, brute()
			, openConnection();
		unsigned int connection;
		bool isAlive = true;
		unsigned char mode;
};

