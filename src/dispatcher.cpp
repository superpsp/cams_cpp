#include "dispatcher.h"
#include "logger.h"

#define LOGGER Logger::getInstance()

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
		dispatcherInstance->setDefaultParameters();
	}
	return *dispatcherInstance;
}

void Dispatcher::setDefaultParameters() {
	numberOfDevices = NUMBER_OF_DEVICES;
	source = new Source();
	LOGGER.logDebug("Dispatcher::setDefaultParameters: source = " + std::to_string(source->SOURCE_DB));
}

void Dispatcher::setNumberOfDevices(unsigned long number) {
	this->numberOfDevices = number;
}

bool Dispatcher::run() {
	LOGGER.logDebug("Dispatcher::run: Start");
	if (source != NULL) {
		LOGGER.logDebug("Dispatcher::run: Deleting source");
		delete source;
	}
	LOGGER.logDebug("Dispatcher::run: Stop");
	return true;
}

void Dispatcher::registerDevice() {
	LOGGER.logDebug("Dispatcher::registerDevice: ipFile = ");
}