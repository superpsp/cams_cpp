#include "device.h"
#include "storage.h"
#include "logger.h"
#include "tools.h"
#include "dispatcher.h"

#define LOGGER Logger::getInstance()
#define STORAGE Storage::getInstance()
#define TOOLS Tools::getInstance()
#define DISPATCHER Dispatcher::getInstance()

Device::Device(unsigned char vendor, unsigned char mode) {
	LOGGER->logDebug("Device::Device: Start " + std::to_string((unsigned long long)this) + ", mode = " + std::to_string(mode));
	DISPATCHER->registerDevice(this);
	this->vendor = vendor;
	this->mode = mode;
	switch (mode) { // TODO: For other modes
	case MODE_BRUT:
		LOGGER->logDebug("Device::Device: Device to brute " + std::to_string((unsigned long long)this));
		setIp();
		brute();
		break;
	default:
		break;
	}
}

void Device::setIp() {
	LOGGER->logDebug("Device::setIp: Device " + std::to_string((unsigned long long)this));
	std::list<std::string> ipString = STORAGE->getIp();
	if (ipString.size() > 0) {
		LOGGER->logDebug("Device::setIp: ip = " + ipString.front());
		if (!ipString.front().empty()) {
			ip = TOOLS->getNumericIpFromString(ipString.front());
		}
		if (ipString.size() > 1) {
			LOGGER->logDebug("Device::setIp: login = " + ipString.back());
			deviceFromQueueFlag = true;
			login = STORAGE->getlogin(std::stoi(ipString.back()));
		} else {
			LOGGER->logDebug("Device::setIp: First time");
			login = STORAGE->getKnownlogin(ip);
			if (login == nullptr) {
				LOGGER->logDebug("Device::setIp: IP is not known yet");
				login = STORAGE->getlogin(0);
			} else {
				knownLoginFlag = true;
			}
		}
		LOGGER->logDebug("Device::setIp: user = " + login->getUser() + ", password = " + login->getPassword());
	}
	LOGGER->logDebug("Device::setIp: ip = " + TOOLS->getStringIpFromNumeric(ip));
}

void Device::openConnection() {
	LOGGER->logDebug("Device::openConnection: ip = " + TOOLS->getStringIpFromNumeric(ip));
	connection = 0;
	if (connection != 0) {
		STORAGE->setKnownlogin(ip, login);
	}
}

unsigned long Device::getIp() const {
	return ip;
}

unsigned char Device::getMode() {
	return mode;
}

void Device::brute() {
	LOGGER->logDebug("Device::brute: Start ip = " + TOOLS->getStringIpFromNumeric(ip));
	if (ip != 0) {
		openConnection();
		LOGGER->logDebug("Device::brute: Stop ip = " + TOOLS->getStringIpFromNumeric(ip));
		stop();
	} else {
		LOGGER->logDebug("Device::brute: stopBruteDevices ip = " + TOOLS->getStringIpFromNumeric(ip));
		DISPATCHER->stopBruteDevices();
	}
}

void Device::stop() {
	if (isAlive) {
		LOGGER->logDebug("Device::stop: stopping device " + std::to_string((unsigned long long)this));
		isAlive = false;
		DISPATCHER->deleteDevice(this);
	}
}

Device::~Device() {
	//TODO: clear data
	LOGGER->logDebug("Device::~Device: deleted device " + std::to_string((unsigned long long)this));
}
