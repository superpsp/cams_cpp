#include "dispatcher.h"
#include "logger.h"
#include "file.h"

#define LOGGER Logger::getInstance()

Dispatcher* dispatcherInstance = 0;
File* ipFile = 0;


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
		dispatcherInstance->initialize();
	}
	return *dispatcherInstance;
}

// TODO: number of devices from parameters
void Dispatcher::initialize() {
	this->ipFileName = IP_FILE_NAME;
	this->numberOfDevices = NUMBER_OF_DEVICES;
	LOGGER.logDebug("Dispatcher::initialize: ipFileName = "
		+ this->ipFileName
		+ " numberOfDevices = " + std::to_string(this->numberOfDevices)
	);
}

void Dispatcher::run() {
	LOGGER.logDebug("Dispatcher::run: Start");
	LOGGER.logDebug("Dispatcher::run: Stop");
}

void Dispatcher::registerDevice() {
	LOGGER.logDebug("Dispatcher::registerDevice: ipFile = " + this->ipFileName);
}