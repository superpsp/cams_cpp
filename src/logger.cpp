#include <iostream>
#include "logger.h"
#include "tools.h"
#include "file.h"
#include "fsmanager.h"

#define TOOLS Tools::getInstance()
#define FSMANAGER FileSystemManager::getInstance()

Logger *loggerInstance = 0;
File *logFile = 0;

LoggerDestructor::~LoggerDestructor() {
    if (logFile->checkFile() == logFile->FILE_OK) {
        logFile->fileClose();
        delete logFile;
    }
    delete loggerInstance;
}

void LoggerDestructor::initialize(Logger* p) {
    loggerInstance = p;
}

Logger &Logger::getInstance() {
    if (!loggerInstance) {
        loggerInstance = new Logger();
        loggerInstance->logDebug("Logger::getInstance: Instance created");
        loggerInstance->setDefaultParameters();
    }
    return *loggerInstance;
}

void Logger::setDefaultParameters() {
    //this->logLevel = LOG_LEVEL_ERROR;
    this->logLevel = LOG_LEVEL_DEBUG;
    this->logFileName = LOG_FILE_NAME;
    this->logDestination = LOG_DEST_FILE;
    switchLogFile(this->logFileName, this->logFileName, true);
    loggerInstance->logDebug("Logger::setDefaultParameters: Parameters were set");
}

void Logger::switchLogFile(std::string oldName, std::string newName) {
    logDebug("Logger::switchLogFile: oldFile = " + oldName + " newName = " + newName);
    switchLogFile(oldName, newName, false);
}

void Logger::switchLogFile(std::string oldName, std::string newName, bool isNew) {
    if (isNew) {
        this->logFileName = newName;
        logFile = new File(this->logFileName, File::FILE_TXT, File::FILE_IO_OUT);
        logDebug("Logger::switchLogFile: log file " + newName + " was created");
        return;
    }
    logDebug("Logger::switchLogFile: oldName " + oldName + " newName = " + newName);
    if (oldName.compare(newName) != 0 || oldName.compare(this->logFileName) == 0) {
        if (FSMANAGER.isFileExisting(oldName) && logFile->checkFile() == logFile->FILE_OK) {
            logFile->fileClose();
            delete logFile;
        logDebug("Logger::switchLogFile: oldFile " + oldName + " was closed");
        }

        this->logFileName = newName;
        logFile = new File(this->logFileName, File::FILE_TXT, File::FILE_IO_OUT);

        if (FSMANAGER.isFileExisting(oldName) && !FSMANAGER.isFileEmpty(oldName)) {
            std::string tmpFileName = TOOLS.getUUID();
            FSMANAGER.copyFile(oldName, tmpFileName);
            logDebug("Logger::switchLogFile: oldFile = " + oldName + " was copied to tmpFileName = " + tmpFileName);

            File* tmpFile = new File(tmpFileName, File::FILE_TXT, File::FILE_IO_IN);

            while (tmpFile->checkFile() == tmpFile->FILE_OK) {
                logFile->writeLine(tmpFile->readLine());
            }
            logDebug("Logger::switchLogFile: content of tmpFileName = " + tmpFileName + " was copied to newName = " + newName);
            delete tmpFile;
            FSMANAGER.deleteFile(tmpFileName);
            logDebug("Logger::switchLogFile: tmpFileName = " + tmpFileName + " was deleted");

            if (oldName.compare(newName) != 0) {
                FSMANAGER.deleteFile(oldName);
                logDebug("Logger::switchLogFile: old file " + oldName + " was deleted");
            }
        }
    }
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
    logDebug("Logger::setLogDestination: destination = " + std::to_string(destination));

    if (destination == LOG_DEST_CONSOLE) {
        if (this->logDestination != LOG_DEST_CONSOLE) {
            logDebug("Logger::setLogDestination: destination was set to " + std::to_string(destination));
            this->logDestination = LOG_DEST_CONSOLE;
            delete logFile;
        }
    } else if (destination == LOG_DEST_FILE) {
        if (this->logDestination != LOG_DEST_FILE) {
            logDebug("Logger::setLogDestination: destination was set to " + std::to_string(destination));
            this->logDestination = LOG_DEST_FILE;
            switchLogFile(this->logFileName, this->logFileName);
        }
    }else {
        logError("Logger::setLogDestination: unknown destination " + std::to_string(destination));
        return false;
    }
    return true;
}

bool Logger::setLogFileName(std::string fileName) {
    logDebug("Logger::setLogFileName: fileName = " + fileName);
    if (fileName.substr(0, 1).compare("-") == 0) {
        logError("Logger::setLogFileName: error in fileName " + fileName);
        return false;
    }
    if (fileName != this->logFileName) {
        switchLogFile(this->logFileName, fileName);
        logDebug("Logger::setLogFileName: fileName was changed to " + fileName);
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
            std::cout << TOOLS.getTime() << " " << "[ERROR] " << "Logger::logText: " << logFileName << errorMessage << std::endl;
            std::cout << message << std::endl;
        }
    }
}

