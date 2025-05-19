#include "loggerpsp.h"
#include "parameters.h"
#include "tools.h"
#include "dispatcher.h"

#define LOGGER LoggerPSP::getInstance()
#define APPPARAMETERS AppParameters::getInstance()
#define DISPATCHER Dispatcher::getInstance()

int main(int argc, char* argv[]) {
	const std::string IP_FILE_NAME = "ips.txt";

	LOGGER.logText("main: cams is started");

	APPPARAMETERS.setParameters(argc, argv);
	AppParametersDestructor();

	DISPATCHER.setIpFile(IP_FILE_NAME);

	ToolsDestructor();
	DispatcherDestructor();
	LoggerPSPDestructor();
}