#include <iostream>
#include "logger.h"
#include "tools.h"
#include "file.h"

#define TOOLS Tools::getInstance()

Logger *loggerInstance = 0;
File *logFile = 0;

LoggerDestructor::~LoggerDestructor() {
    logFile->fileClose();
    delete logFile;
    delete loggerInstance;
}

void LoggerDestructor::initialize(Logger* p) {
    loggerInstance = p;
}

Logger &Logger::getInstance() {
    if (!loggerInstance) {
        loggerInstance = new Logger();
        loggerInstance->setDefaultParameters();
    }
    return *loggerInstance;
}

void Logger::setDefaultParameters() {
    logLevel = LOG_LEVEL_ERROR;
    logDestination = LOG_DEST_FILE;
    logFileName = LOG_FILE_NAME;
    logFile = new File(logFileName, File::FILE_TXT, File::FILE_IO_OUT);
}

void Logger::setLogLevel(char logLevel) {
    if (logLevel > this->logLevel) {
        this->logLevel = logLevel;
        logDebug("Logger::setLogLevel: logLevel = " + std::to_string(this->logLevel));
    }
}

void Logger::setLogDestination(char destination) {
    if (destination == LOG_DEST_CONSOLE) {
        logFile->fileClose();
        delete logFile;
    }
    this->logDestination = destination;
}

bool Logger::setLogFileName(std::string fileName) {
    if (fileName.substr(0, 1).compare("-") != std::string::npos) {
        return false;
    }
    if (fileName != this->logFileName) {
        logFile->fileClose();
        File* tmpLogFile = new File(logFileName, File::FILE_TXT, File::FILE_IO_IN);
        logFile = new File(logFileName, File::FILE_TXT, File::FILE_IO_OUT);
    }
    this->logFileName = fileName;
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
    logText(TOOLS.getTime() + " " + level + " " + message);
}

void Logger::logText(std::string message) {
    if (logDestination == LOG_DEST_CONSOLE) {
        std::cout << message << std::endl;
    } else {
        short writeResult = logFile->writeLine(message);
        if (writeResult != logFile->FILE_OK) {
            std::string errorMessage;
            if (writeResult == logFile->FILE_NOT_OPENED) {
                errorMessage = " is not opened";
            }
            if (writeResult == logFile->FILE_NOT_GOOD) {
                errorMessage = " is not good";
            }
            if (writeResult == logFile->FILE_MODE_NOT_CORRECT) {
                errorMessage = " is not opened for out";
            }
            if (writeResult == logFile->FILE_TYPE_NOT_CORRECT) {
                errorMessage = " is not a TXT file";
            }
            std::cout << TOOLS.getTime() << " " << "[ERROR] " << logFileName << errorMessage << std::endl;
            std::cout << message << std::endl;
        }
    }
}

