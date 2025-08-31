#include "storage.h"
#include "logger.h"
#include "tools.h"

#define LOGGER Logger::getInstance()
#define TOOLS Tools::getInstance()

std::mutex storageMutex;

StorageDestructor::~StorageDestructor() {
	std::lock_guard<std::mutex> lock(storageMutex);
	if (storageInstance->storage == storageInstance->STORAGE_FILE) {
		storageInstance->logins.clear();
		for (unsigned char i = storageInstance->STORAGE_FILE_IP; i < storageInstance->STORAGE_FILE_NUMBER; i++) {
			storageInstance->deleteFile(i);
			LOGGER->logDebug("StorageDestructor::~StorageDestructor: File " + storageInstance->getFileName(i) + " was deleted");
		}
	}
	delete storageInstance;
	LOGGER->logDebug("StorageDestructor::~StorageDestructor: Instance deleted");
}

void StorageDestructor::initialize(Storage* p) {
	LOGGER->logDebug("StorageDestructor::~initialize: initialized");
	storageInstance = p;
}

Storage* Storage::getInstance() {
	std::lock_guard<std::mutex> lock(storageMutex);
	if (!storageInstance) {
		storageInstance = std::unique_ptr<Storage>(new Storage());
		LOGGER->logDebug("Storage::getInstance: Instance created");
		storageInstance.get()->initialize();
	}
	return storageInstance.get();
}

void Storage::setStorage(unsigned char storage) { // TODO: implement change storage to DB
	this->storage = storage;
}

void Storage::initialize() {
	LOGGER->logDebug("Storage::initialize: Start");
	if (storage == STORAGE_FILE) {
		openFiles();
		if (isFileKnownLoginsExisting) {
			createKnownLogins();
		}
	}
	LOGGER->logDebug("Storage::initialize: End");
}

void Storage::createKnownLogins() {
	unsigned long ipNumeric;
	std::string
		line
		, ip
		, user
		, password;
	line = readLineFromFile(STORAGE_FILE_KNOWN_IP);
	while (!line.empty()) {
		LOGGER->logDebug("Storage::createKnownLogins: line = " + line);
		line = readLineFromFile(STORAGE_FILE_KNOWN_IP);
		ip = TOOLS->getSubStringBetweenSubStrings(line, "i:", "u:");
		user = TOOLS->getSubStringBetweenSubStrings(line, "u:", "p:");
		password = TOOLS->getSubStringBetweenSubStrings(line, "p:", "#:", false, true);
		ipNumeric = TOOLS->getNumericIpFromString(ip);
		LOGGER->logDebug("Storage::createKnownLogins: ip = " + ip + ", user = " + user + ", password = " + password + ", ipNumeric = " + std::to_string(ipNumeric));

		if (ipNumeric != 0 && !user.empty() && !password.empty()) {
			for (auto login = logins.begin(); login != logins.end(); login++) {
				if (user.compare(login->second->getUser()) == 0 && password.compare(login->second->getPassword()) == 0) {
					LOGGER->logDebug("Storage::createKnownLogins: Found user = " + user + ", password = " + password + ", ipNumeric = " + std::to_string(ipNumeric));
					knownLogins[ipNumeric] = login->second;
					break;
				}
			}
		}
	}
}

void Storage::createLogins() {
	std::list<std::string>
		users
		, passwords;
	unsigned int id = 0;
	std::string
		user
		, password;

	user = readLineFromFile(STORAGE_FILE_USER);
	while (!user.empty()) {
		LOGGER->logDebug("Storage::createLogins: user " + user);
		users.push_back(user);
		user = readLineFromFile(STORAGE_FILE_USER);
	}
	password = readLineFromFile(STORAGE_FILE_PASSWORD);
	while (!password.empty()) {
		LOGGER->logDebug("Storage::createLogins: password " + password);
		passwords.push_back(password);
		password = readLineFromFile(STORAGE_FILE_PASSWORD);
	}
	//if (users.empty() || passwords.empty()) {} // TODO: Trigger event for Dispatcher to abort
	for (; !users.empty(); users.pop_front()) {
		user = users.front();
		for (; !passwords.empty(); passwords.pop_front()) {
			password = passwords.front();
			LOGGER->logDebug("Storage::getlogins: Creating Login: " + user + ", " + password);
			logins[id] = new Login(id, user, password);
			id++;
		}
	}
}

void Storage::deleteFile(unsigned char file) {
	delete files[file];
}

bool Storage::openFile(unsigned char file) {
	files[file] = new FileText(getFileName(file), modes[file]);
	unsigned char result = files[file]->open();
	if (result == File::FILE_OK) {
		LOGGER->logDebug("Storage::openFile: File " + getFileName(file) + " was opened");
		if (file != STORAGE_FILE_KNOWN_IP) {
			isFileKnownLoginsExisting = true;
		}
	}
	else {
		if (file != STORAGE_FILE_KNOWN_IP && file != STORAGE_FILE_IP_QUEUE) {
			LOGGER->logError("Storage::openFile: Can not open required file " + getFileName(file));
			return false;
		}
	}
	return true;
}

//void Storage::switchMode(unsigned char file, unsigned char newMode) { // TODO: Use lock in calling method
//	LOGGER->logDebug("Storage::switchMode: File " + std::to_string(file) + ", mode " + std::to_string(newMode));
//	if (modes[file] != newMode) {
//		deleteFile(file);
//		openFile(file);
//		LOGGER->logDebug("Storage::switchMode: Mode was switched");
//	}
//}

std::string Storage::getFileName(unsigned char storageFile) {
	LOGGER->logDebug("Storage::getFileName: storageFile " + std::to_string(storageFile));
	switch (storageFile) {
		case STORAGE_FILE_IP:
			return STORAGE_IP_FILE_NAME;
		case STORAGE_FILE_USER:
			return STORAGE_USER_FILE_NAME;
		case STORAGE_FILE_PASSWORD:
			return STORAGE_PASSWORD_FILE_NAME;
		case STORAGE_FILE_KNOWN_IP:
			return STORAGE_KNOWN_IP_FILE_NAME;
		case STORAGE_FILE_IP_QUEUE_IN:
			return STORAGE_IP_QUEUE_IN_FILE_NAME;
		case STORAGE_FILE_IP_QUEUE_OUT:
			return STORAGE_IP_QUEUE_OUT_FILE_NAME;
	}
	return std::string();
}

std::string Storage::readLineFromFile(unsigned char storageFile) {
	LOGGER->logDebug("Storage::readLineFromFile: storageFile = " + std::to_string(storageFile));
	std::string result = files[storageFile]->readLine();
	if (result.starts_with(files[storageFile]->FILE_READ_ERROR)) {
		LOGGER->logDebug("Storage::readLineFromFile: result = " + result);
		result = std::string();
	}
	return result;
}

void Storage::openFiles() {
	for (unsigned char i = STORAGE_FILE_IP; i < STORAGE_FILE_NUMBER; i++) {
		LOGGER->logDebug("Storage::openFiles: File " + std::to_string(i));
		if (i == STORAGE_FILE_IP_QUEUE_OUT) {
			modes[i] = File::FILE_IO_OUT;
		} else {
			modes[i] = File::FILE_IO_IN;
		}
		if (!openFile(i)) {
			break; // TODO: Trigger event for Dispatcher to abort
		}
	}
}

std::string Storage::getIp() {
	std::string result;
	if (storage == STORAGE_FILE) {
		if (files[STORAGE_FILE_IP_QUEUE_IN]->getFileSize(getFileName(STORAGE_FILE_IP_QUEUE_IN)) == 0) {
			result = readLineFromFile(STORAGE_FILE_IP);
		} else {
			result = readLineFromFile(STORAGE_FILE_IP_QUEUE_IN);
		}
	}
	LOGGER->logDebug("Storage::getIp: IP = " + result);
	return result;
}

Login* Storage::getlogin(unsigned int id) {
	if (storage == STORAGE_FILE) {
		if (logins.empty()) {
			createLogins();
		}
		if (auto element = logins.find(id); element != logins.end()) {
			LOGGER->logDebug("Storage::getlogin: user = " + element->second->getUser() + ", password = " + element->second->getPassword());
			return element->second;
		}
	}
	return nullptr;
}

Login* Storage::getKnownlogin(unsigned long ip) {
	if (auto element = knownLogins.find(ip); element != knownLogins.end()) {
		LOGGER->logDebug("Storage::getKnownlogin: user = " + element->second->getUser() + ", password = " + element->second->getPassword());
		return element->second;
	}
	return nullptr;
}

void Storage::setNextKnownIp(std::string line) {
	LOGGER->logDebug("Storage::setNextKnownIp: line = " + line);
	files[STORAGE_FILE_KNOWN_IP]->writeLine(line);
}
