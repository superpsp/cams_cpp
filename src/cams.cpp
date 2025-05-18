#include "loggerpsp.h"
#include "parameters.h"
#include "tools.h"

#define LOGGER LoggerPSP::getInstance()
#define APPPARAMETERS AppParameters::getInstance()

int main(int argc, char* argv[]) {
	LOGGER.setLogLevel(LoggerPSP::LOG_LEVEL_INFO);
	LOGGER.logInfo("cams is started");

	//AppParametersDestructor();
	LOGGER.logInfo("cams is completed");
}