#include <iostream>
#include "loggerpsp.h"
#include "tools.h"
#include "file.h"

#define TOOLS Tools::getInstance()

Logger *loggerInstance = 0;
File *logFile = 0;

LoggerPSPDestructor::~LoggerPSPDestructor() {
    delete loggerInstance;
}

void LoggerPSPDestructor::initialize(Logger* p) {
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

void Logger::setLogLevel(short logLevel) {
    this->logLevel = logLevel;
    if (this->logLevel >= LOG_LEVEL_DEBUG) {
        logDebug("Logger::setLogLevel: logLevel = " + std::to_string(this->logLevel));
    }
}

void Logger::setLogDestination(short destination) {
    logDebug("Logger::setLogDestination: destination =  " + destination);
    //if (logDestination != destination) {
    //    if (logDestination = LOG_DEST_FILE) {
    //        FILE_SYSTEM_MANAGER.closeFile(LOG_FILE_NAME);
    //        logDebug("Logger::setLogDestination: File " + LOG_FILE_NAME + " was closed");
    //        if (FILE_SYSTEM_MANAGER.printFile(LOG_FILE_NAME) != FILE_SYSTEM_MANAGER.FILE_MANAGER_OPERATION_RESULT_OK) {
    //            if (FILE_SYSTEM_MANAGER.FILE_MANAGER_OPERATION_RESULT_FILE_IS_OPENED) {
    //                logDebug("Logger::setLogDestination: Can not read from " + LOG_FILE_NAME + " , because it is opened");
    //            }
    //        }
    //    }
    //    this->logDestination = destination;
    //    logDebug(" Logger::setLogDestination: logDestination = " + std::to_string(this->logDestination));
    //}
}

void Logger::setLogFileName(std::string fileName) {
    this->logFileName = fileName;
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
            if (writeResult == logFile->FILE_NOT_GOOD) {
                errorMessage = " is not good";
            }
        }
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

