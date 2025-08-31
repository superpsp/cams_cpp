#include "dispatcher.h"
#include "logger.h"
#include "storage.h"
#include "text_file.h"

#define LOGGER Logger::getInstance()
#define STORAGE Storage::getInstance()

std::mutex dispatcherMutex;


DispatcherDestructor::~DispatcherDestructor() {
	StorageDestructor* storageDestructor = new StorageDestructor();
	//dispatcherInstance->deleteLogins();
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
	//createLogins();
	LOGGER->logDebug("Dispatcher::run: Stop");
	return true;
}

//void Dispatcher::createLogins() {
//	LOGGER->logDebug("Dispatcher::createLogins: Start");
//	logins = STORAGE->getlogins();
//	LOGGER->logDebug("Dispatcher::createLogins: Stop");
//}
//
//void Dispatcher::deleteLogins() {
//	LOGGER->logDebug("Dispatcher::deleteLogins: Start");
//	for (Login* login : logins) {
//		LOGGER->logDebug("Dispatcher::deleteLogins: Login " + std::to_string(login->getId()) + " " + login->getUser() + " " + login->getPassword() + " was deleted");
//		delete login;
//	}
//	logins.clear();
//	LOGGER->logDebug("Dispatcher::deleteLogins: Stop");
//}

//void Dispatcher::deviceInQueue(DeviceProto* device) {
//}
