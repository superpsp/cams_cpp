#include "dispatcher.h"
#include "logger.h"
#include "storage.h"

#define LOGGER Logger::getInstance()
#define STORAGE Storage::getInstance()

std::mutex dispatcherMutex;


DispatcherDestructor::~DispatcherDestructor() {
	StorageDestructor* storageDestructor = new StorageDestructor();
	storageDestructor->initialize(STORAGE);
	delete storageDestructor;
	delete dispatcherInstance;
	LOGGER->logDebug("DispatcherDestructor::~DispatcherDestructor: Instance deleted");
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

	LOGGER->logDebug("Dispatcher::run: Stop");
	return true;
}

void Dispatcher::registerDevice() {
	LOGGER->logDebug("Dispatcher::registerDevice: ipFile = ");
}