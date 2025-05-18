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
		LOGGER.logDebug("AppParameters: Instance created");
	}
	return *appParametersInstance;
}

void AppParameters::setParameters() {
	LOGGER.logDebug("AppParameters::setParameters: Start");
	LOGGER.logDebug("AppParameters::setParameters: End");
}