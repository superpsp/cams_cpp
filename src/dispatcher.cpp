#include "dispatcher.h"
#include "logger.h"
#include "storage.h"
#include "text_file.h"

#define LOGGER Logger::getInstance()
#define STORAGE Storage::getInstance()

std::mutex dispatcherMutex;


DispatcherDestructor::~DispatcherDestructor() {
	StorageDestructor* storageDestructor = new StorageDestructor();
	storageDestructor->initialize(STORAGE);
	delete storageDestructor;
	delete dispatcherInstance;
	LOGGER->logDebug("DispatcherDestructor::~DispatcherDestructor: Instance deleted");
}

void DispatcherDestructor::initialize(Dispatcher* p) {
	LOGGER->logDebug("DispatcherDestructor::~initialize: initialized");
	dispatcherInstance = p;
}

Dispatcher* Dispatcher::getInstance() {
	std::lock_guard<std::mutex> lock(dispatcherMutex);
	if (!dispatcherInstance) {
		dispatcherInstance = std::unique_ptr<Dispatcher>(new Dispatcher());
		LOGGER->logDebug("Dispatcher::getInstance: Instance created");
	}
	return dispatcherInstance.get();
}

void Dispatcher::setNumberOfDevices(unsigned long number) {
	this->numberOfDevices = number;
}

bool Dispatcher::run() {
	LOGGER->logDebug("Dispatcher::run: Start");
	createLogins();
	LOGGER->logDebug("Dispatcher::run: Stop");
	return true;
}

void Dispatcher::createLogins() {
	LOGGER->logDebug("Dispatcher::createLogins: Start");
	unsigned int id = 0;
	std::string
		user
		, password;
	bool
		areUsersCompleted = false
		, arePasswordsCompleted = false;
	while (!areUsersCompleted) {
		user = STORAGE->getNextUser();
		areUsersCompleted = user.compare(FileText::FILE_READ_ERROR) == 0;
		while (!arePasswordsCompleted) {
			password = STORAGE->getNextPassword();
			arePasswordsCompleted = password.compare(FileText::FILE_READ_ERROR) == 0;
			if (!arePasswordsCompleted) {
				LOGGER->logDebug("Dispatcher::createLogins: user = " + user + ", password = " + password);
				logins.push_back(new Login(id, user, password));
				id++;
			}
		}
	}
	numberOfLogins = logins.size();
	LOGGER->logDebug("Dispatcher::createLogins: numberOfLogins = " + std::to_string(numberOfLogins));
	LOGGER->logDebug("Dispatcher::createLogins: Stop");
}

void Dispatcher::deviceInQueue(unsigned int ip, unsigned int login) { // TODO: two queues should be existing: one - for devices from ip.txt, second - from unsucessful brut
}
