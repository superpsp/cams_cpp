#include <string>

class Tools;

class ToolsDestructor {
public:
    ~ToolsDestructor();
    void initialize(Tools* p);
private:
    Tools* toolsInstance;
};

class Tools {
    public:
        Tools& operator = (Tools&);
        struct timeZoneShift {
            short int
                hour
                , minute;
        };

        static Tools& getInstance();
        static bool checkIP(std::string ip);
        static std::string
            digitToYesNo(short digit)
            , digitToTrueFalse(short digit)
            , getDayOfWeekEng(short dayNumber)
            , getSubStringBetweenSubStrings(std::string source
                , std::string substr1
                , std::string substr2
                , bool returnHead = false
                , bool returnTail = false)
            , getIpPaddedZeros(std::string ip)
            , getIpRemovedZeros(std::string ip)
            , padl(std::string source, size_t resultLength, char character = ' ')
            , padr(std::string source, size_t resultLength, char character = ' ')
            , getTime()
            , getUUID();
        unsigned long getIntFromString(std::string source);
    protected:
        Tools() {}
        Tools(const Tools&);
        ~Tools() {}
    friend class ToolsDestructor;
    private:
        static ToolsDestructor destructor;
        static std::string getUUIDPart(const char length);
        static unsigned char getRandomChar();
        static bool isDigitValue(std::string value);
};
