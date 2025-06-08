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
            PARAMETERS_NUMBER = 8;
        std::string
            parameters[PARAMETERS_NUMBER - 1]
            , parameterDescriptions[PARAMETERS_NUMBER - 1];

        static AppParametersDestructor destructor;
        void printHelp();
        void printErrorMessage(std::string message);
        bool validatePosition(std::string parameter, int position, int last);
        bool validateParameter(std::string argument, std::string nextArgument);
        void initParameters();
        bool findAndValidate(std::string parameter);
        bool findAndValidate(std::string parameter, short last);
        void addPerameter(char position, std::string parameter, std::string description);
};
