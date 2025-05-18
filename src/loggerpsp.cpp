#include <iostream>
#include "loggerpsp.h"
#include "tools.h"

#define TOOLS Tools::getInstance()

LoggerPSP *instance = 0;

LoggerPSPDestructor::~LoggerPSPDestructor() {
    std::cout << "[DEBUG]" << " " << "LoggerPSPDestructor: Instance deleted" << std::endl;
    delete instance;
}

void LoggerPSPDestructor::initialize(LoggerPSP* p) {
    instance = p;
}

LoggerPSP &LoggerPSP::getInstance() {
    if (!instance) {
        instance = new LoggerPSP();
        std::cout << TOOLS.getTime() << " " << "[DEBUG]" << " " << "LoggerPSP: Instance created" << std::endl;
    }
    return *instance;
}

void LoggerPSP::setLogLevel(short level) {
    this->logLevel = level;
}

void LoggerPSP::setLogDestination(short destination) {
    this->logDestination = destination;
}

void LoggerPSP::setLogFileName(std::string fileName) {
    this->logFileName = fileName;
}

void LoggerPSP::logPrint(std::string message, std::string level) {
    if (LOG_DEST_FILE == 1) {
        std::cout << TOOLS.getTime() << " " << level << " " << message << std::endl;
    }
}

void LoggerPSP::logError(std::string message) {
    if (this->logLevel >= LOG_LEVEL_ERROR) {
        logPrint(message, "[ERROR]");
    }
}

void LoggerPSP::logWarning(std::string message) {
    if (this->logLevel >= LOG_LEVEL_WARNING) {
        logPrint(message, "[WARNING]");
    }
}

void LoggerPSP::logInfo(std::string message) {
    if (this->logLevel >= LOG_LEVEL_INFO) {
        logPrint(message, "[INFO]");
    }
}

void LoggerPSP::logDebug(std::string message) {
    if (this->logLevel >= LOG_LEVEL_DEBUG) {
        logPrint(message, "[DEBUG]");
    }
}

