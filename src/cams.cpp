#include "loggerpsp.h"
#include "parameters.h"
#include "tools.h"

#define LOGGER LoggerPSP::getInstance()
#define APPPARAMETERS AppParameters::getInstance()

int main(int argc, char* argv[]) {
	LOGGER.logText("main: cams is started");

	APPPARAMETERS.setParameters(argc, argv);
	AppParametersDestructor();

	ToolsDestructor();
	LoggerPSPDestructor();
}