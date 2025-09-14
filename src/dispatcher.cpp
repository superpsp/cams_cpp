#include "dispatcher.h"
#include "logger.h"
#include "storage.h"
#include "tools.h"
#include "dhnetsdk.h"

#define LOGGER Logger::getInstance()
#define STORAGE Storage::getInstance()
#define TOOLS Tools::getInstance()

std::mutex dispatcherMutex;

DispatcherDestructor::~DispatcherDestructor() {
	StorageDestructor* storageDestructor = new StorageDestructor();
	storageDestructor->initialize(STORAGE);
	delete storageDestructor;
	//delete dispatcherInstance;
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
		//if (!CLIENT_Init(disConnect, 0)) {
		//	int error = logSDKError("Can't init SDK.");
		//	return 1;
		//}
	}
	return dispatcherInstance.get();
}

void Dispatcher::setNumberOfDevices(unsigned long number) {
	LOGGER->logDebug("Dispatcher::setNumberOfDevices: number = " + std::to_string(number));
	this->numberOfDevices = number;
	if (number == 0) {
		toContinueBrute = false;
	}
}

void Dispatcher::setVendor(unsigned char vendor) {
	this->vendor = vendor;
}

void Dispatcher::registerDevice(Device* device) {
	LOGGER->logDebug("Dispatcher::registerDevice: " + std::to_string((unsigned long long)device));
	devices.push_back(device);
}

void Dispatcher::bruteDevices() {
	LOGGER->logDebug("Dispatcher::bruteDevices: Start");
	while (toContinueBrute || !devices.empty()) {
		if (devices.size() < numberOfDevices && toContinueBrute) {
			LOGGER->logInfo("Dispatcher::bruteDevices: devices in list: " + std::to_string(devices.size()) + ", creating new");
			createDevice(vendor, Device::MODE_BRUT);
		}
		else {
			LOGGER->logDebug("Dispatcher::bruteDevices: devices in list: " + std::to_string(devices.size()) + ", toContinueBrute = " + std::to_string(toContinueBrute));
		}
	}
	LOGGER->logDebug("Dispatcher::bruteDevices: Stop");
}

void Dispatcher::stopBruteDevices() {
	LOGGER->logDebug("Dispatcher::stopBruteDevices: toContinueBrut = " + std::to_string(toContinueBrute) + ", devices in list: " + std::to_string(devices.size()));
	if (toContinueBrute) {
		toContinueBrute = false;
		std::list<Device*> devicesToDelete = devices;
		for (auto it = devicesToDelete.begin(); it != devicesToDelete.end(); it++) {
			(*it)->stop();
		}
		LOGGER->logDebug("Dispatcher::stopBruteDevices: devices in list: " + std::to_string(devices.size()));
	}
}

void Dispatcher::deleteDevice(Device* device) {
	LOGGER->logDebug("Dispatcher::deleteDevice: device to be deleted " + std::to_string((unsigned long long)device) + ", ip = " + TOOLS->getStringIpFromNumeric(device->getIp()));
	if (!devices.empty()) {
		auto it = std::find(devices.begin(), devices.end(), device);
		if (it != devices.end()) {
			LOGGER->logDebug("Dispatcher::deleteDevice: device ip = " + TOOLS->getStringIpFromNumeric(device->getIp()) + " deleted");
			devices.erase(it);
			delete device;
		}
	}
	LOGGER->logDebug("Dispatcher::deleteDevice: devices in list: " + std::to_string(devices.size()) + ", toContinueBrute = " + std::to_string(toContinueBrute));
}

void Dispatcher::createDevice(unsigned char vendor, unsigned char mode) {
	LOGGER->logDebug("Dispatcher::createDevice: mode =  " + std::to_string(mode));
	switch (mode) { // TODO: For other modes
		case Device::MODE_BRUT:
			if (toContinueBrute) {
				Device* device = new Device(vendor, mode);
				LOGGER->logDebug("Dispatcher::createDevice: device " + std::to_string((unsigned long long)device));
			}
			break;
		default:
			break;
	}
}
