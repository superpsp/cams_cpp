#include "parameters.h"
#include "logger.h"

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
		if (argc -1 > PARAMETERS_NUMBER) {
			printError("Allowed number of parameters with arguments is " + std::to_string(PARAMETERS_NUMBER)
				+ ", but " + std::to_string(argc - 1) + " was provided in command " + executedCommand);
			return false;
		}

		std::string
			parameter
			, message;
		char argument;
		for (int i = 1; i < argc; i++) {
			parameter = argv[i];
			LOGGER.logDebug("AppParameters::parseParameters: parameter = " + parameter);
			if (parameter.compare("-d") == 0) {
				LOGGER.setLogLevel(LOGGER.LOG_LEVEL_DEBUG);
				LOGGER.logDebug("AppParameters::parseParameters: executedCommand = " + executedCommand);
			} else if (parameter.compare("--info") == 0) {
				LOGGER.setLogLevel(LOGGER.LOG_LEVEL_INFO);
			} else if (parameter.compare("--warning") == 0) {
				LOGGER.setLogLevel(LOGGER.LOG_LEVEL_WARNING);
			} else if (parameter.compare("--help") == 0 || parameter.compare("-h") == 0) {
				printHelp();
				return false;
			} else if (parameter.compare("--log_destination") == 0) {
				if (i == argc - 1) {
					printError("Parameter --log_destination requires an argument");
					return false;
				}
				i++;
				parameter = argv[i];
				argument = getCharValue(parameter);
				message = "";
				if (argument == PARAMETER_NOT_OK || !LOGGER.setLogDestination(argument)) {
					message = "Parameter --log_destination requires a valid argument, but " + parameter + " was provided";
				}
				if (!message.empty()) {
					printError(message);
					return false;
				}
			} else if (parameter.compare("--log_file_name") == 0) {
				message = "";
				if (i == argc - 1) {
					message = "Parameter --log_file_name requires an argument";
				}
				i++;
				parameter = argv[i];
				if (!LOGGER.setLogFileName(parameter)) {
					message = "Parameter --log_file_name requires a valid argument, but " + parameter + " was provided";
				}
				if (!message.empty()) {
					printError(message);
					return false;
				}
			} else {
				printError("Invalid parameter " + parameter + " was provided");
				return false;
			}
		}
	}
	return true;
}

AppParameters::AppParameters(const AppParameters&){}

char AppParameters::getCharValue(std::string value) {
	if (value.compare("0") == 0) {
		return 0;
	}
	else if (value.compare("1") == 0) {
		return 1;
	}
	return PARAMETER_NOT_OK;
}

void AppParameters::printError(std::string message) {
	LOGGER.logDebug("AppParameters::printError: " + message);
	if (LOGGER.setLogDestination(LOGGER.LOG_DEST_CONSOLE, true)) {
		LOGGER.logText(message);
		printHelp();
	}
}

void AppParameters::printHelp() {
	if (LOGGER.setLogDestination(LOGGER.LOG_DEST_CONSOLE, true)) {
		LOGGER.logText("Usage: cams <options>");
			LOGGER.logText("<options>:");
			LOGGER.logText("\t -h --help\t\t\t\tShow help");
			LOGGER.logText("\t -d\t\t\t\t\tDebug logging level (the deepest level will be used from the provided ones)");
			LOGGER.logText("\t --info\t\t\t\t\tInfo logging level (the deepest level will be used from the provided ones)");
			LOGGER.logText("\t --warning\t\t\t\tWarning logging level (the deepest level will be used from the provided ones)");
			LOGGER.logText("\t --log_destination destination\t\tdestination = 0 - console, destination = 1 - file (default 1)");
			LOGGER.logText("\t --log_file_name log_file_name\t\tdefault log_file_name = cams.log, log_file_name can not have '-' as a first character");
			LOGGER.logText("\t --ip_file_name ip_file_name\t\tdefault ip_file_name = ips.txt, ip_file_name can not have '-' as a first character");
		if (!LOGGER.setLogDestination(LOGGER.LOG_DEST_FILE, true)) {
			LOGGER.logError("AppParameters::printHelp: Can not switch destination to file");
		}
	} else {
		LOGGER.logError("AppParameters::printHelp: Can not switch destination to console");
	}
}