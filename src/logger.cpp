#include <iostream>
#include "logger.h"
#include "tools.h"

#define TOOLS Tools::getInstance()

std::mutex logMutex;

LoggerDestructor::~LoggerDestructor() {
    delete loggerInstance->logFile;
    delete loggerInstance;
}

void LoggerDestructor::initialize(Logger* p) {
    loggerInstance = p;
}

Logger* Logger::getInstance() {
    std::lock_guard<std::mutex> lock(logMutex);
    if (!loggerInstance) {
        loggerInstance = std::unique_ptr<Logger>(new Logger());
        loggerInstance->setDefaultParameters();
    }
    return loggerInstance.get();
}

void Logger::setDefaultParameters() {
    openLogFile();
}

void Logger::openLogFile() {
    unsigned char openResult = logFile->open();
    if (openResult != File::FILE_OK) {
        logDestination = LOG_DEST_CONSOLE;
    }
}

void Logger::setLogLevel(char logLevel) {
    setLogLevel(logLevel, false);
}

void Logger::setLogLevel(char logLevel, bool force) {
    if (logLevel > this->logLevel || force) {
        this->logLevel = logLevel;
    }
}

bool Logger::setLogDestination(char destination) {
    if (destination > LOG_DEST_FILE) {
        return false;
    }
    return setLogDestination(destination, false);
}

bool Logger::setLogDestination(char destination, bool isQuiet) {
    if (!isQuiet) {
    }

    if (destination == LOG_DEST_CONSOLE) {
        if (this->logDestination != LOG_DEST_CONSOLE) {
            this->logDestination = LOG_DEST_CONSOLE;
            if (!isQuiet) {
                logFile->copyToConsole();
            }
        }
    } else if (destination == LOG_DEST_FILE) {
        if (this->logDestination != LOG_DEST_FILE) {
            this->logDestination = LOG_DEST_FILE;
            if (!isQuiet) {
            }
        }
    }else {
        return false;
    }
    return true;
}

bool Logger::setLogFileName(std::string fileName) {
    if (fileName != logFileName) {
        if (fileName.substr(0, 1).compare("-") == 0) {
            return false;
        }
        unsigned char result = logFile->rename(fileName);
        if (result == File::FILE_OK) {
        } else {
        }
    } else {
    }
    return true;
}

void Logger::logError(std::string message) {
    if (this->logLevel >= LOG_LEVEL_ERROR) {
        logPrint(message, "[ERROR]");
    }
}

void Logger::logWarning(std::string message) {
    if (this->logLevel >= LOG_LEVEL_WARNING) {
        logPrint(message, "[WARNING]");
    }
}

void Logger::logInfo(std::string message) {
    if (this->logLevel >= LOG_LEVEL_INFO) {
        logPrint(message, "[INFO]");
    }
}

void Logger::logDebug(std::string message) {
    if (this->logLevel >= LOG_LEVEL_DEBUG) {
        logPrint(message, "[DEBUG]");
    }
}

void Logger::logPrint(std::string message, std::string level) {
    logText(TOOLS->getTime() + " " + level + " " + message);
}

void Logger::logText(std::string message) {
    if (this->logDestination == LOG_DEST_CONSOLE) {
        std::cout << message << std::endl;
    } else {
        short writeResult = logFile->writeLine(message);
        if (writeResult != logFile->FILE_OK) {
            std::string errorMessage = logFile->getErrorMessage(writeResult);
            std::cout << TOOLS->getTime() << " " << "[ERROR] " << "Logger::logText: " << this->logFileName << errorMessage << std::endl;
            std::cout << message << std::endl;
        }
    }
}

unsigned char Logger::getLogDestination() {
    return logDestination;
}

