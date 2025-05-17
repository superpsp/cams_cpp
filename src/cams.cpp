#include <iostream>
#include "loggerpsp.h"

int main(int argc, char* argv[]) {
	LoggerPSP& log = LoggerPSP::getInstance();
	log.setLogLevel(log.LOG_LEVEL_INFO);
	log.logInfo("Project is started");
	log.logInfo("Project is completed");
}