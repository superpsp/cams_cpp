#include "loggerpsp.h"
#include "tools.h"
#include "fsmanager.h"

#define TOOLS Tools::getInstance()
#define FILE_SYSTEM_MANAGER FileSystemManager::getInstance()

LoggerPSP *loggerInstance = 0;

LoggerPSPDestructor::~LoggerPSPDestructor() {
    delete loggerInstance;
}

void LoggerPSPDestructor::initialize(LoggerPSP* p) {
    loggerInstance = p;
}

LoggerPSP &LoggerPSP::getInstance() {
    if (!loggerInstance) {
        loggerInstance = new LoggerPSP();
        loggerInstance->setDefaultParameters();
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
    logDebug("LoggerPSP::setLogDestination: destination =  " + destination);
    if (logDestination != destination) {
        if (logDestination = LOG_DEST_FILE) {
            FILE_SYSTEM_MANAGER.closeFile(LOG_FILE_NAME);
            logDebug("LoggerPSP::setLogDestination: File " + LOG_FILE_NAME + " was closed");
            if (FILE_SYSTEM_MANAGER.printFile(LOG_FILE_NAME) != FILE_SYSTEM_MANAGER.FILE_MANAGER_OPERATION_RESULT_OK) {
                if (FILE_SYSTEM_MANAGER.FILE_MANAGER_OPERATION_RESULT_FILE_IS_OPENED) {
                    logDebug("LoggerPSP::setLogDestination: Can not read from " + LOG_FILE_NAME + " , because it is opened");
                }
            }
        }
        this->logDestination = destination;
        logDebug(" LoggerPSP::setLogDestination: logDestination = " + std::to_string(this->logDestination));
    }
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
        short writeResult = FILE_SYSTEM_MANAGER.writeToFile(logFileName, message);
        if (!writeResult == FILE_SYSTEM_MANAGER.FILE_MANAGER_OPERATION_RESULT_OK) {
            logDestination = LOG_DEST_CONSOLE;
            if (writeResult == FILE_SYSTEM_MANAGER.FILE_MANAGER_OPERATION_RESULT_NOT_OPENED) {
                std::cout << TOOLS.getTime() << " " << "[ERROR]" << "  Can not open File " << logFileName << std::endl;
            }
            if (writeResult == FILE_SYSTEM_MANAGER.FILE_MANAGER_OPERATION_RESULT_NOT_GOOD) {
                std::cout << TOOLS.getTime() << " " << "[ERROR]" << "  File " << logFileName << " is not good" << std::endl;
            }
            std::cout << message << std::endl;
        }
    }
}

