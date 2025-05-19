#include "dispatcher.h"
#include "loggerpsp.h"

#define LOGGER LoggerPSP::getInstance()

Dispatcher* dispatcherInstance = 0;


DispatcherDestructor::~DispatcherDestructor() {
	LOGGER.logDebug("DispatcherDestructor::~DispatcherDestructor: Instance deleted");
	delete dispatcherInstance;
}

void DispatcherDestructor::initialize(Dispatcher* p) {
	dispatcherInstance = p;
}

Dispatcher& Dispatcher::getInstance() {
	if (!dispatcherInstance) {
		dispatcherInstance = new Dispatcher();
		LOGGER.logDebug("Dispatcher::getInstance: Instance created");
	}
	return *dispatcherInstance;
}

void Dispatcher::setIpFile(std::string ipFile) {
	LOGGER.logDebug("Dispatcher::setIpFile: ipFile = " + ipFile);
	this->ipFile = ipFile;
}