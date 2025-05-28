#include "dispatcher.h"
#include "logger.h"
#include <fstream>

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
	}
	return *dispatcherInstance;
}

bool Dispatcher::initialize(std::string ipFile, unsigned int numberOfDevices, unsigned int deviceLifetimeSeconds, unsigned int deviceLifeTimeCheckSeconds) {
	LOGGER.logDebug("Dispatcher::setIpFile: ipFile = " + ipFile
		+ ", numberOfDevices = " + std::to_string(numberOfDevices)
		+ ", deviceLifetimeSeconds = " + std::to_string(deviceLifetimeSeconds)
		+ ", deviceLifetimeCheckSeconds = " + std::to_string(deviceLifeTimeCheckSeconds));
	this->ipFile = ipFile;
	this->numberOfDevices = numberOfDevices;
	this->deviceLifetimeSeconds = deviceLifetimeSeconds;
	this->deviceLifeTimeCheckSeconds = deviceLifeTimeCheckSeconds;
	std::ifstream inIpFile(this->ipFile, std::ifstream::in);
	if (!inIpFile.is_open()) {
		LOGGER.logError("Can't open file " + this->ipFile + " for reading");
		return false;
	}
	return true;
}

void Dispatcher::registerDevice() {
	LOGGER.logDebug("Dispatcher::registerDevice: ipFile = " + ipFile);
}