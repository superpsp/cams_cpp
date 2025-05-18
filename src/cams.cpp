#include "loggerpsp.h"
#include "parameters.h"
#include "tools.h"

#define LOGGER LoggerPSP::getInstance()
#define APPPARAMETERS AppParameters::getInstance()

int main(int argc, char* argv[]) {
	LOGGER.setLogLevel(LoggerPSP::LOG_LEVEL_DEBUG);
	LOGGER.logInfo("cams is started");

	APPPARAMETERS.setParameters();

	AppParametersDestructor();
	ToolsDestructor();
	LoggerPSPDestructor();
}