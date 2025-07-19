#include "dispatcher.h"
#include "logger.h"

#define LOGGER Logger::getInstance()

std::mutex dispatcherMutex;


DispatcherDestructor::~DispatcherDestructor() {
	LOGGER->logDebug("DispatcherDestructor::~DispatcherDestructor: Instance deleted");
	delete dispatcherInstance;
}

void DispatcherDestructor::initialize(Dispatcher* p) {
	LOGGER->logDebug("DispatcherDestructor::~initialize: initialized");
	dispatcherInstance = p;
}

Dispatcher* Dispatcher::getInstance() {
	std::lock_guard<std::mutex> lock(dispatcherMutex);
	if (!dispatcherInstance) {
		dispatcherInstance = std::unique_ptr<Dispatcher>(new Dispatcher());
		LOGGER->logDebug("Dispatcher::getInstance: Instance created");
	}
	return dispatcherInstance.get();
}

void Dispatcher::setNumberOfDevices(unsigned long number) {
	this->numberOfDevices = number;
}

bool Dispatcher::run() {
	LOGGER->logDebug("Dispatcher::run: Start");

	if (!source->initialize(source->SOURCE_FILE)) { // TODO: add to parameters
		LOGGER->logError("Dispatcher::run: Can not initialize Source");
		delete source;
		return false;
	}

	delete source;
	LOGGER->logDebug("Dispatcher::run: Stop");
	return true;
}

void Dispatcher::registerDevice() {
	LOGGER->logDebug("Dispatcher::registerDevice: ipFile = ");
}