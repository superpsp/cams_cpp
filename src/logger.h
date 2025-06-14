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
        bool
            setLogFileName(std::string fileName)
            , setLogDestination(char destination);
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
        Logger(const Logger&);
        ~Logger() {}
        friend class LoggerDestructor;
private:
    char logLevel
        , logDestination;
    const std::string LOG_FILE_NAME = "cams.log";
    std::string logFileName;
    
    static LoggerDestructor destructor;
    void 
        logPrint(std::string message, std::string level)
        , setDefaultParameters()
        , switchLogFile(std::string oldName, std::string newName)
        , switchLogFile(std::string oldName, std::string newName, bool isNew);
};
