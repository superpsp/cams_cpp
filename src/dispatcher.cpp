#include "dispatcher.h"
#include "logger.h"

#define LOGGER Logger::getInstance()

Dispatcher* dispatcherInstance = 0;


DispatcherDestructor::~DispatcherDestructor() {
	if (dispatcherInstance->source != NULL) {
		delete dispatcherInstance->source;
	}
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
}

void Dispatcher::setNumberOfDevices(unsigned long number) {
	this->numberOfDevices = number;
}

bool Dispatcher::run() {
	LOGGER.logDebug("Dispatcher::run: Start");
	LOGGER.logDebug("Dispatcher::run: Stop");
	return true;
}

void Dispatcher::registerDevice() {
	LOGGER.logDebug("Dispatcher::registerDevice: ipFile = ");
}