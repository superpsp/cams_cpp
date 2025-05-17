class AppParameters;

class AppParametersDestructor {
    public:
        ~AppParametersDestructor();
        void initialize(AppParameters* p);
    private:
        AppParameters* instance;
};

class AppParameters {
    public:
        static AppParameters& getInstance();
    protected:
        friend class AppParametersDestructor;
    private:
        static AppParameters* instance;
        static AppParametersDestructor destructor;
};
