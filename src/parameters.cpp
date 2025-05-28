#include "parameters.h"
#include "loggerpsp.h"

#define LOGGER Logger::getInstance()

AppParameters* appParametersInstance = 0;

AppParametersDestructor::~AppParametersDestructor() {
	LOGGER.logDebug("AppParametersDestructor: Instance deleted");
	delete appParametersInstance;
}

void AppParametersDestructor::initialize(AppParameters* p) {
	appParametersInstance = p;
}

AppParameters& AppParameters::getInstance() {
	if (!appParametersInstance) {
		appParametersInstance = new AppParameters();
	}
	return *appParametersInstance;
}

void AppParameters::parseParameters(int argc, char* argv[]) {
	std::string
		argument
		, nextArgument
		, executedCommand = argv[0];
	short logDestination = LOGGER.LOG_DEST_FILE;

	if (argc && argc > 1) {
		for (int i = 1; i < argc; i++) {
			argument = argv[i];
			LOGGER.logDebug("AppParameters::parseParameters: argument = " + argument);
			if (argument.find("-h") || argument.find("--help") != std::string::npos) {
				LOGGER.setLogDestination(LOGGER.LOG_DEST_CONSOLE);
				printHelp();
				break;
			} else if (argument.find("-d") != std::string::npos) {
				LOGGER.setLogLevel(LOGGER.LOG_LEVEL_DEBUG);
				for (int j = 1; j < argc; j++) {
					executedCommand = executedCommand + " " + argv[j];
				}
				LOGGER.logDebug("AppParameters::parseParameters: executedCommand = " + executedCommand);
			} else if (argument.find("--info") != std::string::npos) {
				LOGGER.setLogLevel(LOGGER.LOG_LEVEL_INFO);
			} else if (argument.find("--warning") != std::string::npos) {
				LOGGER.setLogLevel(LOGGER.LOG_LEVEL_WARNING);
			} else if (argument.find("--log_destination") != std::string::npos && validateParameter(argument, argv[i + 1])) {
				i++;
			} else if (argument.find("--log_file_name") != std::string::npos && validateParameter(argument, argv[i + 1])) {
				i++;
			} else {
				printHelp();
				break;
			}
		}
	}
	LOGGER.logDebug("AppParameters::setParameters: End");
}

bool AppParameters::validateParameter(std::string argument, std::string nextArgument) {
	bool isArgumentValid = false;

	LOGGER.logDebug("AppParameters::validateParameter: nextArgument = " + nextArgument);

	if (argument.find("--log_destination") != std::string::npos) {
		if (//!isdigit(nextArgument.at(0))
			//|| nextArgument.substr(0, 1) != std::to_string(LOGGER.LOG_DEST_CONSOLE)
			//|| nextArgument.substr(0, 1) != std::to_string(LOGGER.LOG_DEST_FILE)
			//|| nextArgument.length() > 1
			//|| 
			nextArgument.compare(std::to_string(LOGGER.LOG_DEST_CONSOLE)) == 0
			|| nextArgument.compare(std::to_string(LOGGER.LOG_DEST_FILE)) == 0
			) 
		{
			isArgumentValid = true;
		}
	}
	if (!isArgumentValid) {
		LOGGER.logDebug("AppParameters::validateParameter: parameter " + argument + " or it's argument " + nextArgument + " is invalid");
	}
	return isArgumentValid;
}

void AppParameters::printHelp() {
	LOGGER.logText("Usage: cams <options>");
	LOGGER.logText("<options>:");
	LOGGER.logText("\t -h --help\tShow help");
	LOGGER.logText("\t -d\t\tDebug logging level (the deepest level will be used from the provided ones)");
	LOGGER.logText("\t --info\t\tInfo logging level (the deepest level will be used from the provided ones)");
	LOGGER.logText("\t --warning\tWarning logging level (the deepest level will be used from the provided ones)");
	LOGGER.logText("\t --log_destination destination\t\tdestination = 0 - console, destination = 1 - file (default 1)");
	LOGGER.logText("\t --log_file_name log_file_name\t\tdefault log_file_name = cams.log, log_file_name can not have - as a first character");
	LOGGER.logText("\t --ip_file_name ip_file_name\t\tdefault ip_file_name = ips.txt");
}