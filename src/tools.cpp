#include <time.h>
#include "tools.h"
#include "loggerpsp.h"

#define LOGGER LoggerPSP::getInstance()

Tools* instance = 0;

ToolsDestructor::~ToolsDestructor() {
    LOGGER.logDebug("ToolsDestructor: Instance deleted");
    delete instance;
}

void ToolsDestructor::initialize(Tools* p) {
    instance = p;
}

Tools& Tools::getInstance() {
    if (!instance) {
        instance = new Tools();
        LOGGER.logDebug("Tools: Instance created");
    }
    return *instance;
}

bool Tools::checkIP(std::string ip) {
    std::string::size_type lastDotPos = 0,
        dotPos;
    short numberOfOctets = 0;//,
    std::string octet,
        allowedCharacters("0123456789");

    numberOfOctets = 0;
    ip.append("."); // to get the last octet
    dotPos = ip.find(".");
    while (dotPos != std::string::npos) {
        octet = ip.substr(lastDotPos, dotPos - lastDotPos);
        if (octet.find_first_not_of(allowedCharacters) != std::string::npos
            || octet.empty()
            || stoi(octet) > 255
            ) {
            numberOfOctets = 0;
            break;
        }

        numberOfOctets++;
        lastDotPos = dotPos + 1;
        dotPos = ip.find(".", lastDotPos);
    }

    if (numberOfOctets == 4) {
        return true;
    }
    else {
        return false;
    }
}

std::string Tools::digitToYesNo(short digit) {
    if (digit < 0 || digit > 1) {
        return "";
    }

    if (digit == 1) {
        return "Yes";
    }
    else {
        return "No";
    }
}

std::string Tools::digitToTrueFalse(short digit) {
    if (digit < 0 || digit > 1) {
        return "";
    }

    if (digit == 1) {
        return "true";
    }
    else {
        return "false";
    }
}

std::string Tools::getDayOfWeekEng(short dayNumber) {
    switch (dayNumber) {
    case 0:
        return "Sunday";
        break;
    case 1:
        return "Monday";
        break;
    case 2:
        return "Tuesday";
        break;
    case 3:
        return "Wednesday";
        break;
    case 4:
        return "Thursday";
        break;
    case 5:
        return "Friday";
        break;
    case 6:
        return "Saturday";
        break;
    case 7:
        return "Holiday";
        break;
    default:
        return "";
    }
}

std::string Tools::getSubStringBetweenSubStrings(std::string source, std::string substr1, std::string substr2
    , bool returnHead, bool returnTail) {
    if (source.empty() || substr1.empty() || substr2.empty()) {
        return "";
    }

    std::size_t
        found1
        , found2;

    found1 = source.find(substr1);
    found2 = source.find(substr2, found1 + substr1.length());
    if (found1 != std::string::npos and found2 != std::string::npos) {
        return source.substr(found1 + substr1.length(), found2 - found1 - substr1.length());
    }
    else {
        if (found1 != std::string::npos and returnTail) {
            return source.substr(found1 + substr1.length(), source.length() + 1 - found1 - substr1.length());
        }
        if (found2 != std::string::npos and returnHead) {
            return source.substr(0, found2);
        }
    }
    return "";
}

std::string Tools::getIpPaddedZeros(std::string ip) {
    std::string::size_type lastDotPos = 0,
        dotPos;
    short numberOfOctets = 0;//,
    std::string octet
        , allowedCharacters("0123456789")
        , result;

    numberOfOctets = 0;
    ip.append("."); // to get the last octet
    dotPos = ip.find(".");
    while (dotPos != std::string::npos) {
        octet = ip.substr(lastDotPos, dotPos - lastDotPos);
        if (octet.find_first_not_of(allowedCharacters) != std::string::npos
            || octet.empty()
            || stoi(octet) > 255
            ) {
            numberOfOctets = 0;
            break;
        }

        octet.insert(octet.begin(), 3 - octet.length(), '0');
        result.append(octet + ".");
        numberOfOctets++;
        lastDotPos = dotPos + 1;
        dotPos = ip.find(".", lastDotPos);
    }

    if (numberOfOctets == 4) {
        return result.substr(0, result.length() - 1);
    }
    else {
        return "";
    }
}

std::string Tools::getIpRemovedZeros(std::string ip) {
    std::string::size_type lastDotPos = 0,
        dotPos;
    short numberOfOctets = 0;//,
    std::string octet
        , allowedCharacters("0123456789")
        , result;

    numberOfOctets = 0;
    ip.append("."); // to get the last octet
    dotPos = ip.find(".");
    while (dotPos != std::string::npos) {
        octet = ip.substr(lastDotPos, dotPos - lastDotPos);
        if (octet.find_first_not_of(allowedCharacters) != std::string::npos
            || octet.empty()
            || stoi(octet) > 255
            ) {
            numberOfOctets = 0;
            break;
        }

        result.append(std::to_string(stoi(octet)) + ".");
        numberOfOctets++;
        lastDotPos = dotPos + 1;
        dotPos = ip.find(".", lastDotPos);
    }

    if (numberOfOctets == 4) {
        return result.substr(0, result.length() - 1);
    }
    else {
        return "";
    }
}

std::string Tools::padl(std::string source, size_t resultLength, char character) {
    if (resultLength <= source.length()) {
        return source;
    }

    std::string result = source;
    result.insert(result.begin(), resultLength - result.length(), character);
    return result;
}

std::string Tools::padr(std::string source, size_t resultLength, char character) {
    if (resultLength <= source.length()) {
        return source;
    }

    std::string result = source;
    result.insert(result.end(), resultLength - result.length(), character);
    return result;
}

std::string Tools::getTime(short int shiftHours, short int shiftMinutes) {
    time_t rawTime;
    struct tm* timeInfo;

    time(&rawTime);
    if (shiftHours == 99) {
        timeInfo = localtime(&rawTime);
    }
    else {
        timeInfo = gmtime(&rawTime);
    }

    return std::to_string(timeInfo->tm_year + 1900)
        + "."
        + padl(std::to_string(timeInfo->tm_mon + 1), 2, '0')
        + "."
        + padl(std::to_string(timeInfo->tm_mday), 2, '0')
        + " "
        + padl(std::to_string(timeInfo->tm_hour), 2, '0')
        + ":"
        + padl(std::to_string(timeInfo->tm_min), 2, '0')
        + ":"
        + padl(std::to_string(timeInfo->tm_sec), 2, '0')
        ;
}
