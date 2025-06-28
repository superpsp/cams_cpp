#include "logger.h"
#include "parameters.h"
#include "tools.h"
#include "dispatcher.h"

#define LOGGER Logger::getInstance()
#define APPPARAMETERS AppParameters::getInstance()
#define DISPATCHER Dispatcher::getInstance()

int main(int argc, char* argv[]) {
	if (APPPARAMETERS.parseParameters(argc, argv)) {
		LOGGER.logDebug("main: deleting AppParameters");
		AppParametersDestructor();
		if (DISPATCHER.run()) {
			LOGGER.logError("main: Error in Dispatcher");
		}
	}
	LOGGER.logDebug("main: deleting Dispatcher");
	DispatcherDestructor();
	LOGGER.logDebug("main: deleting Tools");
	ToolsDestructor();
	LOGGER.logDebug("main: deleting Logger");
	LoggerDestructor();
}