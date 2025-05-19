#include "parameters.h"
#include "loggerpsp.h"

#define LOGGER LoggerPSP::getInstance()

AppParameters* appParametersInstance = 0;

AppParametersDestructor::~AppParametersDestructor() {
	LOGGER.logDebug("AppParametersDestructor: Instance deleted");
	delete appParametersInstance;
}

void AppParametersDestructor::initialize(AppParameters* p) {
	appParametersInstance = p;
}

AppParameters& AppParameters::getInstance() {
	if (!appParametersInstance) {
		appParametersInstance = new AppParameters();
		LOGGER.logDebug("AppParameters::getInstance: Instance created");
	}
	return *appParametersInstance;
}

void AppParameters::setParameters(int argc, char* argv[]) {
	std::string argument;

	if (argc && argc > 1) {
		for (int i = 1; i < argc; i++) {
			argument = argv[i];
			if (argument.find("-d") != std::string::npos) {
				LOGGER.setLogLevel(LOGGER.LOG_LEVEL_DEBUG);
				LOGGER.logDebug("AppParameters::setParameters: Setting level to [DEBUG]");
				LOGGER.logDebug("AppParameters::setParameters: argc = " + std::to_string(argc));
			} else {
				printHelp();
				break;
			}
		}
	}
	LOGGER.logDebug("AppParameters::setParameters: End");
}

void AppParameters::printHelp() {
	LOGGER.logText("Usage: cams <options>");
	LOGGER.logText("<options>:");
	LOGGER.logText("\t -h --help\tShow help");
	LOGGER.logText("\t -d\t\tDebug mode");
}