#pragma once
#include <string>
#include <memory>
#include "text_file.h"

class Logger;

class LoggerDestructor {
    public:
        ~LoggerDestructor();
        void initialize(Logger *p);
    private:
        Logger* loggerInstance;
};

class Logger {
    public:
        Logger(Logger const&) = delete;
        Logger& operator = (Logger const&) = delete;
        ~Logger() {}
        static Logger* getInstance();
        bool
            setLogFileName(std::string fileName)
            , setLogDestination(char destination)
            , setLogDestination(char destination, bool isQuiet);
        void
            setLogLevel(char logLevel)
            , setLogLevel(char logLevel, bool force)
            , logError(std::string message)
            , logWarning(std::string message)
            , logInfo(std::string message)
            , logDebug(std::string message)
            , logText(std::string message);

        const char
            LOG_LEVEL_ERROR = 0
            , LOG_LEVEL_WARNING = 10
            , LOG_LEVEL_INFO = 20
            , LOG_LEVEL_DEBUG = 30
            , LOG_DEST_CONSOLE = 0
            , LOG_DEST_FILE = 1;
protected:
    Logger() {}
    friend class LoggerDestructor;
private:
    const std::string LOG_FILE_NAME = "cams.log";
    char logLevel = LOG_LEVEL_ERROR
        , logDestination = LOG_DEST_FILE;
    std::string logFileName = LOG_FILE_NAME;
    inline static std::unique_ptr<Logger> loggerInstance{ nullptr };
    FileText* logFile = new FileText(logFileName, File::FILE_IO_OUT);

    static LoggerDestructor destructor;
    void 
        logPrint(std::string message, std::string level)
        , setDefaultParameters()
        , openLogFile();
};
