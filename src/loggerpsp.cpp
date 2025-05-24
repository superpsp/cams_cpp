#include "loggerpsp.h"
#include "tools.h"
#include "fsmanager.h"

#define TOOLS Tools::getInstance()
#define FILE_SYSTEM_MANAGER FileSystemManager::getInstance()

LoggerPSP *loggerInstance = 0;

LoggerPSPDestructor::~LoggerPSPDestructor() {
    //if (FILE_SYSTEM_MANAGER.isFileOpened(loggerInstance->logFileName, FILE_SYSTEM_MANAGER.FILE_MANAGER_FILE_OUT)) {
    //    FILE_SYSTEM_MANAGER.fileClose(loggerInstance->logFileName);
    //}
    delete loggerInstance;
}

void LoggerPSPDestructor::initialize(LoggerPSP* p) {
    loggerInstance = p;
}

LoggerPSP &LoggerPSP::getInstance() {
    if (!loggerInstance) {
        loggerInstance = new LoggerPSP();
    }
    return *loggerInstance;
}

void LoggerPSP::setDefaultParameters() {
    logLevel = LOG_LEVEL_ERROR;
    logDestination = LOG_DEST_FILE;
    logFileName = LOG_FILE_NAME;
}

void LoggerPSP::setLogLevel(short logLevel) {
    this->logLevel = logLevel;
    if (this->logLevel >= LOG_LEVEL_DEBUG) {
        logDebug("LoggerPSP::setLogLevel: logLevel = " + std::to_string(this->logLevel));
    }
}

void LoggerPSP::setLogDestination(short destination) {
    this->logDestination = destination;
    logDebug(" LoggerPSP::setLogDestination: logDestination = " + std::to_string(this->logDestination));
}

void LoggerPSP::setLogFileName(std::string fileName) {
    this->logFileName = fileName;
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

void LoggerPSP::logPrint(std::string message, std::string level) {
    logText(TOOLS.getTime() + " " + level + " " + message);
}

void LoggerPSP::logText(std::string message) {
    if (logDestination == LOG_DEST_CONSOLE) {
        std::cout << message << std::endl;
    } else {
        if (!FILE_SYSTEM_MANAGER.isFileOpened(logFileName, FILE_SYSTEM_MANAGER.FILE_MANAGER_FILE_OUT)
            || FILE_SYSTEM_MANAGER.writeToFile(logFileName, message) != FILE_SYSTEM_MANAGER.FILE_MANAGER_OK) {
            std::cout << "Can not write to file " << logFileName << std::endl;
            logDestination = LOG_DEST_CONSOLE;
            std::cout << message << std::endl;
        }
    }
}

