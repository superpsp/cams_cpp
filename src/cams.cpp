#include "loggerpsp.h"

int main(int argc, char* argv[]) {
	LoggerPSP& log = LoggerPSP::getInstance();
	log.setLogLevel(log.LOG_LEVEL_INFO);
	log.logInfo("cams is started");
	log.logInfo("cams is completed");
}