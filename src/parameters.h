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
        static const char
            PARAMETERS_NUMBER = 8
            , PARAMETER_NOT_OK = 255;

        static AppParametersDestructor destructor;
        char getCharValue(std::string value);
        void printError(std::string message);
        void printHelp();
};
