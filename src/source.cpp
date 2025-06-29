#include "source.h"
#include "logger.h"

#define LOGGER Logger::getInstance()

Source::Source() {
	LOGGER.logDebug("Source::Source: Start");
	setDefaultParameters();
}

Source::~Source() {
	if (ipFile != NULL) {
		LOGGER.logDebug("Source::~Source: Deleting " + ipFileName);
		delete ipFile;
	}
}

void Source::setDefaultParameters() {
	LOGGER.logDebug("Source::setDefaultParameters: Start");
	setSource(SOURCE_FILE);
	ipFileName = SOURCE_IP_FILE_NAME;
	setFileName(SOURCE_FILE_IP, ipFileName);
	openFile(SOURCE_FILE_IP);
}

void Source::setSource(unsigned char source) { // TODO: add DB to parameters
	LOGGER.logDebug("Source::setSource: source = " + std::to_string(source));
	this->source = source;
}

void Source::setFileName(unsigned char target, std::string name) {  // TODO: add IP file name to parameters
	LOGGER.logDebug("Source::setFileName: target = " + std::to_string(target) + ", name = " + name);
	switch (target)	{
	case SOURCE_FILE_IP:
		ipFileName = name;
		break;
	default:
		break;
	}
}

void Source::openFile(unsigned char target) {
	LOGGER.logDebug("Source::openFile: target = " + std::to_string(target));
	switch (target) {
		case SOURCE_FILE_IP: {
			ipFile = new FileText(ipFileName, File::FILE_IO_IN);
			unsigned char openResult = ipFile->open();
			checkFile(target, openResult);
			break;
		}
		default: {
			LOGGER.logError("Source::openFile: unknown target = " + std::to_string(target));
			break;
		}
	}
}

bool Source::checkFile(unsigned char target, unsigned char openResult) {
	switch (target) {
		case SOURCE_FILE_IP: {
			if (openResult == File::FILE_OK) {
				LOGGER.logDebug("Source::checkFile: File " + ipFileName + " was opened");
				return true;
			} else {
				LOGGER.logError("Source::checkFile: File " + ipFileName + ipFile->getErrorMessage(openResult));
				delete ipFile;
			}
			break;
		}
		default: {
			break;
			LOGGER.logError("Source::checkFile: unknown target = " + std::to_string(target));
		}
	}
	return false;
}
