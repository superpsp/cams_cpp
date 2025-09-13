#include "logger.h"
#include "parameters.h"
#include "tools.h"
#include "dispatcher.h"

#define TOOLS Tools::getInstance()
#define LOGGER Logger::getInstance()
#define APPPARAMETERS AppParameters::getInstance()
#define DISPATCHER Dispatcher::getInstance()

int main(int argc, char* argv[]) {
	if (APPPARAMETERS->parseParameters(argc, argv)) {
		DISPATCHER->bruteDevices();
	}
	LOGGER->logDebug("main: deleting AppParameters");
	AppParametersDestructor* appParametersDestructor = new AppParametersDestructor();
	appParametersDestructor->initialize(APPPARAMETERS);
	delete appParametersDestructor;

	LOGGER->logDebug("main: deleting Dispatcher");
	DispatcherDestructor* dispatcherDestructor = new DispatcherDestructor();
	dispatcherDestructor->initialize(DISPATCHER);
	delete dispatcherDestructor;

	LOGGER->logDebug("main: deleting Logger");
	LoggerDestructor* loggerDestructor = new LoggerDestructor();
	loggerDestructor->initialize(LOGGER);
	delete loggerDestructor;

	ToolsDestructor* toolsDestructor = new ToolsDestructor();
	toolsDestructor->initialize(TOOLS);
	delete toolsDestructor;

	return 0;
}