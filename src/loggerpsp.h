#include <string>

class LoggerPSP;

class LoggerPSPDestructor {
public:
    ~LoggerPSPDestructor();
    void initialize(LoggerPSP* p);
private:
    LoggerPSP* instance;
};

class LoggerPSP {
    public:
        static const short LOG_LEVEL_ERROR = 0
            , LOG_LEVEL_WARNING = 10
            , LOG_LEVEL_INFO = 20
            , LOG_LEVEL_DEBUG = 30
            , LOG_DEST_CONSOLE = 0
            , LOG_DEST_FILE = 1;

        static LoggerPSP& getInstance();
        void setLogLevel(short level),
            setLogDestination(short destination),
            setLogFileName(std::string fileName),
            logError(std::string message),
            logWarning(std::string message),
            logInfo(std::string message),
            logDebug(std::string message);
    protected:
        LoggerPSP() {}
        LoggerPSP(const LoggerPSP&);
        LoggerPSP& operator=(LoggerPSP&);
        ~LoggerPSP() {}
        friend class LoggerPSPDestructor;
    private:
        short logLevel,
            logDestination;
        std::string logFileName;

        static LoggerPSP* instance;
        static LoggerPSPDestructor destructor;
        void logPrint(std::string message, std::string level);
};
