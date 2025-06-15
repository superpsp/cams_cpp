#include <string>

class AppParameters;

class AppParametersDestructor {
    public:
        ~AppParametersDestructor();
        void initialize(AppParameters* p);
    private:
        AppParameters* appParametersInstance;
};

class AppParameters {
    public:
        static AppParameters& getInstance();
        bool parseParameters(int argc, char* argv[]);
    protected:
        AppParameters() {}
        AppParameters(const AppParameters&);
        AppParameters& operator = (AppParameters&);
        ~AppParameters() {}
        friend class AppParametersDestructor;
    private:
        static const char PARAMETERS_NUMBER = 8;
        static AppParametersDestructor destructor;
        void
            printError(std::string message)
            , printHelp()
            , fillAllowedValues(char max);
        bool
            argumentIsEmpty(int i, int argc, std::string parameter);
};
