#include "logger.h"
#include "parameters.h"
#include "tools.h"
#include "dispatcher.h"
#include "fsmanager.h"

#define LOGGER Logger::getInstance()
#define APPPARAMETERS AppParameters::getInstance()

int main(int argc, char* argv[]) {
	// TODO Add to AppParameters
	const std::string IP_FILE_NAME = "../ips.txt";
	const unsigned int NUMBER_OF_DEVICES = 200;
	const unsigned int DEVICE_LIFE_TIME_SECONDS = 10;
	const unsigned int DEVICE_LIFE_TIME_CHECK_SECONDS = 1;

	if (APPPARAMETERS.parseParameters(argc, argv)) {
		int a;
		a = 1;
	}
	AppParametersDestructor();

	//if (DISPATCHER.initialize(IP_FILE_NAME, NUMBER_OF_DEVICES, DEVICE_LIFE_TIME_SECONDS, DEVICE_LIFE_TIME_CHECK_SECONDS)) {
	//	LOGGER.logDebug("main: dispatcher wasn't initialised");
	//}

	ToolsDestructor();
	//DispatcherDestructor();
	LOGGER.logDebug("main: deleting Logger");
	LoggerDestructor();
	//FileSystemManagerDestructor();
}