#include <string>
#include <iostream>

class LoggerPSP;

class LoggerPSPDestructor {
    public:
        ~LoggerPSPDestructor();
        void initialize(LoggerPSP *p);
    private:
        LoggerPSP* loggerInstance;
};

class LoggerPSP {
    public:
        LoggerPSP& operator = (LoggerPSP&);
        static LoggerPSP& getInstance();
        void
            setDefaultParameters()
            , setLogLevel(short logLevel)
            , setLogDestination(short destination)
            , setLogFileName(std::string fileName)
            , logError(std::string message)
            , logWarning(std::string message)
            , logInfo(std::string message)
            , logDebug(std::string message)
            , logText(std::string message);

        const short
            LOG_LEVEL_ERROR = 0
            , LOG_LEVEL_WARNING = 10
            , LOG_LEVEL_INFO = 20
            , LOG_LEVEL_DEBUG = 30
            , LOG_DEST_CONSOLE = 0
            , LOG_DEST_FILE = 1;
protected:
        LoggerPSP() {}
        LoggerPSP(const LoggerPSP&);
        ~LoggerPSP() {}
        friend class LoggerPSPDestructor;
    private:
        short logLevel
            , logDestination;

        const std::string LOG_FILE_NAME = "cams.txt";
        std::string logFileName;
        
        void logPrint(std::string message, std::string level);
        static LoggerPSPDestructor destructor;
};
