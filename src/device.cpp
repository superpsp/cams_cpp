#include "device.h"
#include "storage.h"
#include "logger.h"
#include "tools.h"

#define LOGGER Logger::getInstance()
#define STORAGE Storage::getInstance()
#define TOOLS Tools::getInstance()

Device::Device() {
	LOGGER->logDebug("Device::Device: Start");
	getIp();
	if (ip != 0 && login != nullptr) {
		createConnection();
	}
}

void Device::getIp() {
	std::list<std::string> ipString = STORAGE->getIp();
	if (ipString.size() > 0) {
		LOGGER->logDebug("Device::getIp: ip = " + ipString.front());
		if (!ipString.front().empty()) {
			ip = TOOLS->getNumericIpFromString(ipString.front());
		}
		if (ipString.size() > 1) {
			LOGGER->logDebug("Device::getIp: login = " + ipString.back());
			deviceFromQueueFlag = true;
			login = STORAGE->getlogin(std::stoi(ipString.back()));
		} else {
			LOGGER->logDebug("Device::getIp: First time");
			login = STORAGE->getKnownlogin(ip);
			if (login == nullptr) {
				LOGGER->logDebug("Device::getIp: IP is not known yet");
				login = STORAGE->getlogin(0);
			} else {
				knownLoginFlag = true;
			}
		}
	}
}

void Device::createConnection() {
}

void Device::brute()
{
}
