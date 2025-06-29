#include <iostream>
#include "logger.h"
#include "tools.h"
#include "text_file.h"

#define TOOLS Tools::getInstance()

Logger *loggerInstance = 0;
FileText *logFile = 0;

LoggerDestructor::~LoggerDestructor() {
    delete loggerInstance;
    if (logFile != NULL) {
        delete logFile;
    }
}

void LoggerDestructor::initialize(Logger* p) {
    loggerInstance = p;
}

Logger &Logger::getInstance() {
    if (!loggerInstance) {
        loggerInstance = new Logger();
        //loggerInstance->logDebug("Logger::getInstance: Instance created");
        loggerInstance->setDefaultParameters();
    }
    return *loggerInstance;
}

void Logger::setDefaultParameters() {
    logLevel = LOG_LEVEL_ERROR;
    //loggerInstance->logLevel = LOG_LEVEL_DEBUG;
    logFileName = LOG_FILE_NAME;
    logDestination = LOG_DEST_FILE;
    //logDestination = LOG_DEST_CONSOLE;
    openLogFile();
    loggerInstance->logDebug("Logger::setDefaultParameters: Parameters were set");
}

void Logger::openLogFile() {
    logFile = new FileText(logFileName, File::FILE_IO_OUT);
    unsigned char openResult = logFile->open();
    if (openResult != File::FILE_OK) {
        logDestination = LOG_DEST_CONSOLE;
        logError("Logger::openLogFile: File " + logFileName + logFile->getErrorMessage(openResult));
    }
    logDebug("Logger::openLogFile: File " + logFileName + " was opened");
}

void Logger::setLogLevel(char logLevel) {
    logDebug("Logger::setLogLevel: logLevel = " + std::to_string(logLevel));
    setLogLevel(logLevel, false);
}

void Logger::setLogLevel(char logLevel, bool force) {
    if (logLevel > this->logLevel || force) {
        logDebug("Logger::setLogLevel: logLevel is set to " + std::to_string(logLevel));
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
        logDebug("Logger::setLogDestination: destination = " + std::to_string(destination));
    }

    if (destination == LOG_DEST_CONSOLE) {
        if (this->logDestination != LOG_DEST_CONSOLE) {
            this->logDestination = LOG_DEST_CONSOLE;
            if (!isQuiet) {
                logDebug("Logger::setLogDestination: destination was set to " + std::to_string(destination));
                logFile->copyToConsole();
            }
        }
    } else if (destination == LOG_DEST_FILE) {
        if (this->logDestination != LOG_DEST_FILE) {
            this->logDestination = LOG_DEST_FILE;
            if (!isQuiet) {
                logDebug("Logger::setLogDestination: destination was set to " + std::to_string(destination));
            }
        }
    }else {
        logError("Logger::setLogDestination: unknown destination " + std::to_string(destination));
        return false;
    }
    return true;
}

bool Logger::setLogFileName(std::string fileName) {
    logDebug("Logger::setLogFileName: fileName = " + fileName);
    if (fileName != logFileName) {
        if (fileName.substr(0, 1).compare("-") == 0) {
            logError("Logger::setLogFileName: error in fileName " + fileName);
            return false;
        }
        unsigned char result = logFile->rename(fileName);
        if (result == File::FILE_OK) {
            logDebug("Logger::setLogFileName: fileName was changed to " + fileName);
        } else {
            logError("Logger::setLogFileName: Can't rename " + logFileName + " to " + fileName + ", file " + logFile->getErrorMessage(result));
        }
    } else {
        logWarning("New file name " + fileName + " is the same as a previouse one " + logFileName);
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
    logText(TOOLS.getTime() + " " + level + " " + message);
}

void Logger::logText(std::string message) {
    if (this->logDestination == LOG_DEST_CONSOLE) {
        std::cout << message << std::endl;
    } else {
        short writeResult = logFile->writeLine(message);
        if (writeResult != logFile->FILE_OK) {
            std::string errorMessage = logFile->getErrorMessage(writeResult);
            std::cout << TOOLS.getTime() << " " << "[ERROR] " << "Logger::logText: " << this->logFileName << errorMessage << std::endl;
            std::cout << message << std::endl;
        }
    }
}

