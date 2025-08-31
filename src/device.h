#pragma once
#include "login.h"
class Device {
	public:
		void brute();
	protected:
		unsigned long ip = 0;
	private:
		Login* login;
		bool
			deviceFromQueueFlag = false
			, knownLoginFlag = false;
		void
			getIp()
			, createConnection();
};

