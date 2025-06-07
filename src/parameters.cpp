#include <limits.h>
#include "parameters.h"
#include "logger.h"
#include <iostream>

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

bool AppParameters::parseParameters(int argc, char* argv[]) {
	std::string executedCommand = argv[0];
	if (argc && argc > 1) {
		for (int i = 1; i < argc; i++) {
			executedCommand = executedCommand + " " + argv[i];
		}
		if (argc - 1 > PARAMETERS_NUMBER) {
			LOGGER.setLogDestination(LOGGER.LOG_DEST_CONSOLE);
			LOGGER.logText("AppParameters::parseParameters: allowed number of parameters with arguments is " + std::to_string(PARAMETERS_NUMBER)
				+ ", but " + std::to_string(argc - 1) + " was provided in command " + executedCommand);
			printHelp();
			return false;
		}
		for (int i = 1; i < argc; i++) {
			parameters[i - 1] = argv[i];
		}
		if (parameters->find("-h") != std::string::npos || parameters->find("--help") != std::string::npos) {
			printHelp();
			return false;
		}
		if (findAndValidate("-d")) {
			LOGGER.setLogLevel(LOGGER.LOG_LEVEL_DEBUG);
		}
		size_t position;
		std::string parameter;

		initParameters();
		arguments[0] = std::to_string(LOGGER.LOG_DEST_CONSOLE);
		arguments[1] = std::to_string(LOGGER.LOG_DEST_FILE);
		parameter = "--log_destination";
		if (findAndValidate(parameter), argc) {
			std::string argument = parameters[parameters->find(parameter) + 1];
			if (argument.compare(std::to_string(LOGGER.LOG_DEST_CONSOLE)) == 0) {
				LOGGER.setLogDestination(LOGGER.LOG_DEST_CONSOLE);
			}
			else {
				LOGGER.setLogDestination(LOGGER.LOG_DEST_FILE);
			}
		}
		parameter = "--log_file_name";
		if (findAndValidate(parameter)) {
			std::string argument = parameters[parameters->find(parameter) + 1];
			LOGGER.setLogDestination(LOGGER.LOG_DEST_FILE);
			LOGGER.setLogFileName(argument);
		}
		if (parameters->find("--info") != std::string::npos) {
			short currentLevel = LOGGER.getLogLevel();
			if (currentLevel <= LOGGER.LOG_LEVEL_INFO) {
				LOGGER.setLogLevel(LOGGER.LOG_LEVEL_INFO);
			}
			else {
				LOGGER.logDebug("AppParameters::parseParameters: parameter --info has been ignored, since level is " + std::to_string(currentLevel));
			}
		}
		if (parameters->find("--warning") != std::string::npos) {
			short currentLevel = LOGGER.getLogLevel();
			if (currentLevel <= LOGGER.LOG_LEVEL_WARNING) {
				LOGGER.setLogLevel(LOGGER.LOG_LEVEL_WARNING);
			}
			else {
				LOGGER.logDebug("AppParameters::parseParameters: parameter --warning has been ignored, since level is " + std::to_string(currentLevel));
			}
		}
		LOGGER.logDebug("AppParameters::parseParameters: executedCommand = " + executedCommand);
	}
	return true;
}

void AppParameters::initParameters() {
	parametersRequireArgument[] = {"--log_destination", "--log_file_name", "--p_file_name"};

	for (short i = 0; i < ARGUMENTS_NUMBER; i++) {
		if (arguments[i].compare(std::to_string(SHRT_MAX)) != 0) {
			arguments[i] = std::to_string(SHRT_MAX);
		}
	}
}

bool AppParameters::findAndValidate(std::string parameter) {
	return findAndValidate(parameter, SHRT_MAX);
}

bool AppParameters::findAndValidate(std::string parameter, short last) {
	LOGGER.logDebug("AppParameters::findAndValidate: parameter = " + parameter + ", last =" + std::to_string(last));
	size_t position;
	position = parameters->find(parameter);
	if (position != std::string::npos && position < PARAMETERS_NUMBER - 1 && parameters->find(parameter, position + 1) == std::string::npos) {
		LOGGER.logDebug("AppParameters::findAndValidate: " + parameter + " was found");
	}
	else {
		printErrorMessage("AppParameters::findAndValidate: parameter " + parameter + " is not unique");
		return false;
	}
	if (last != SHRT_MAX) {
		if (position == last - 1) {
			printErrorMessage("AppParameters::findAndValidate: parameter " + parameter + " requires an argument");
			return false;
		}
		bool isArgumentValid = false;
		std::string argument = parameters[position + 1];
		for (short i = 0; i < ARGUMENTS_NUMBER && arguments[i].compare(std::to_string(SHRT_MAX)) == 0; i++) {
			if (argument.compare(arguments[i]) == 0) {
				isArgumentValid = true;
				break;
			}
		}
		if (!isArgumentValid) {
			printErrorMessage("AppParameters::validateParameter: parameter " + parameter + " or it's argument " + argument + " is invalid");
			return false;
		}
	}
	return true;
}

void AppParameters::printErrorMessage(std::string message) {
	LOGGER.setLogDestination(LOGGER.LOG_DEST_CONSOLE);
	LOGGER.logText(message);
	printHelp();
}

void AppParameters::printHelp() {
	LOGGER.logText("Usage: cams <options>");
	LOGGER.logText("<options>:");
	LOGGER.logText("\t -h --help\t\t\t\tShow help");
	LOGGER.logText("\t -d\t\t\t\t\tDebug logging level (the deepest level will be used from the provided ones)");
	LOGGER.logText("\t --info\t\t\t\t\tInfo logging level (the deepest level will be used from the provided ones)");
	LOGGER.logText("\t --warning\t\t\t\tWarning logging level (the deepest level will be used from the provided ones)");
	LOGGER.logText("\t --log_destination destination\t\tdestination = 0 - console, destination = 1 - file (default 1)");
	LOGGER.logText("\t --log_file_name log_file_name\t\tdefault log_file_name = cams.log, log_file_name can not have - as a first character");
	LOGGER.logText("\t --ip_file_name ip_file_name\t\tdefault ip_file_name = ips.txt");
}