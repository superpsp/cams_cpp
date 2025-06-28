#include "dispatcher.h"
#include "logger.h"
#include "text_file.h"

#define LOGGER Logger::getInstance()

Dispatcher* dispatcherInstance = 0;
FileText* ipFile = 0;


DispatcherDestructor::~DispatcherDestructor() {
	LOGGER.logDebug("DispatcherDestructor::~DispatcherDestructor: Instance deleted");
	delete dispatcherInstance;
	delete ipFile;
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
	ipFileName = IP_FILE_NAME;
	numberOfDevices = NUMBER_OF_DEVICES;
	LOGGER.logDebug("Dispatcher::setDefaultParameters: ipFileName = "
		+ ipFileName
		+ " numberOfDevices = " + std::to_string(numberOfDevices)
	);
}

bool Dispatcher::openIpFile() {
	ipFile = new FileText(ipFileName, File::FILE_IO_IN);
	unsigned char openResult = ipFile->open();
	if (openResult == File::FILE_OK) {
		LOGGER.logDebug("Dispatcher::openIpFile: File " + ipFileName + " was opened");
		return false;
	} else {
		LOGGER.logError("Dispatcher::openIpFile: File " + ipFileName + ipFile->getErrorMessage(openResult));
	}
	return true;
}

void Dispatcher::setNumberOfDevices(unsigned long number) {
	this->numberOfDevices = number;
}

bool Dispatcher::run() {
	LOGGER.logDebug("Dispatcher::run: Start");
	if (!openIpFile()) {
		return false;
	}
	LOGGER.logDebug("Dispatcher::run: Stop");
	return true;
}

void Dispatcher::registerDevice() {
	LOGGER.logDebug("Dispatcher::registerDevice: ipFile = " + ipFileName);
}