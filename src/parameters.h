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
        void parseParameters(int argc, char* argv[]);
    protected:
        AppParameters() {}
        AppParameters(const AppParameters&);
        AppParameters& operator = (AppParameters&);
        ~AppParameters() {}
        friend class AppParametersDestructor;
    private:
        static AppParametersDestructor destructor;
        void printHelp();
        bool validateParameter(std::string argument, std::string nextArgument);
};
