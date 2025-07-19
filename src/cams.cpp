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
		if (!DISPATCHER->run()) {
			LOGGER->logError("main: Error in Dispatcher");
			if (LOGGER->getLogDestination() == LOGGER->LOG_DEST_FILE) {
				LOGGER->setLogDestination(LOGGER->LOG_DEST_CONSOLE);
				LOGGER->logError("main: Error in Dispatcher");
				LOGGER->setLogDestination(LOGGER->LOG_DEST_FILE);
			}
		}
	}
	LOGGER->logDebug("main: deleting AppParameters");
	AppParametersDestructor* appParametersDestructor = new AppParametersDestructor();
	appParametersDestructor->initialize(APPPARAMETERS);
	delete appParametersDestructor;

	LOGGER->logDebug("main: deleting Dispatcher");
	DispatcherDestructor* dispatcherDestructor = new DispatcherDestructor();
	dispatcherDestructor->initialize(DISPATCHER);
	delete dispatcherDestructor;

	LOGGER->logDebug("main: deleting Tools");
	ToolsDestructor* toolsDestructor = new ToolsDestructor();
	toolsDestructor->initialize(TOOLS);
	delete toolsDestructor;

	LoggerDestructor* loggerDestructor = new LoggerDestructor();
	loggerDestructor->initialize(LOGGER);
	delete loggerDestructor;

	return 0;
}