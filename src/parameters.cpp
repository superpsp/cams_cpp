#include "parameters.h"

AppParameters* instance = 0;

AppParametersDestructor::~AppParametersDestructor() {
	delete instance;
}

void AppParametersDestructor::initialize(AppParameters* p) {
	instance = p;
}

AppParameters& AppParameters::getInstance() {
	if (!instance) {
		instance = new AppParameters();
	}
	return *instance;
}