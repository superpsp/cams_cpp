#include "parameters.h"
#define LOGGER LoggerPSP::getInstance()

AppParameters* instance = 0;

AppParametersDestructor::~AppParametersDestructor() {
	//LOGGER.logDebug("AppParametersDestructor: Instance deleted");
	delete instance;
}

void AppParametersDestructor::initialize(AppParameters* p) {
	instance = p;
}

AppParameters& AppParameters::getInstance() {
	if (!instance) {
		instance = new AppParameters();
		//LOGGER.logDebug("AppParameters: Instance created");
	}
	return *instance;
}

void AppParameters::setParameters() {
	//LOGGER.logDebug("AppParameters::setParameters: Start");
	//LOGGER.logDebug("AppParameters::setParameters: End");
}