#include <string>

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
        Logger& operator = (Logger&);
        static Logger& getInstance();
        void
            setLogLevel(short logLevel)
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
        Logger() {}
        Logger(const Logger&);
        ~Logger() {}
        friend class LoggerDestructor;
    private:
        short logLevel
            , logDestination;

        const std::string LOG_FILE_NAME = "cams.log";
        std::string logFileName;
        
        void logPrint(std::string message, std::string level);
        static LoggerDestructor destructor;
        void setDefaultParameters();
};
