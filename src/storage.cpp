#include "storage.h"
#include "logger.h"
#include "tools.h"
#include "dispatcher.h"

#define LOGGER Logger::getInstance()
#define TOOLS Tools::getInstance()
#define DISPATCHER Dispatcher::getInstance()

std::mutex storageMutex;

StorageDestructor::~StorageDestructor() {
	std::lock_guard<std::mutex> lock(storageMutex);
	if (storageInstance->isFileKnownLoginsExisting) {
		storageInstance->knownLogins.clear();
	}
	LOGGER->logDebug("StorageDestructor::~StorageDestructor: knownLogins were cleared");

	for (auto it = storageInstance->logins.begin(); it != storageInstance->logins.end(); it++) {
		delete it->second;
	}
	storageInstance->logins.clear();
	LOGGER->logDebug("StorageDestructor::~StorageDestructor: logins were cleared");

	if (storageInstance->storage == storageInstance->STORAGE_FILE) {
		for (unsigned char i = storageInstance->STORAGE_FILE_IP; i < storageInstance->STORAGE_FILE_NUMBER; i++) {
			if (i != storageInstance->STORAGE_FILE_USER && i != storageInstance->STORAGE_FILE_PASSWORD) {
				LOGGER->logDebug("StorageDestructor::~StorageDestructor: File " + storageInstance->getFileName(i) + " deleting");
				storageInstance->deleteFile(i);
				LOGGER->logDebug("StorageDestructor::~StorageDestructor: File " + storageInstance->getFileName(i) + " was deleted");
			}
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
		createLogins();
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
					knownLogins[ipNumeric] = login->second;
					LOGGER->logDebug("Storage::createKnownLogins: Found user = "
						+ login->second->getUser() + ", password = " + login->second->getPassword() + ", ipNumeric = " + std::to_string(ipNumeric));
					break;
				}
			}
		}
	}
	deleteFile(STORAGE_FILE_KNOWN_IP);
	modes[STORAGE_FILE_KNOWN_IP] = File::FILE_IO_OUT;
	openFile(STORAGE_FILE_KNOWN_IP);
	for (auto it = knownLogins.begin(); it != knownLogins.end(); it++) { // TODO: remove and uncomment in Device
		files[STORAGE_FILE_KNOWN_IP]->writeLine("i:" + TOOLS->getStringIpFromNumeric(it->first) + "u:" + it->second->getUser() + "p:" + it->second->getPassword());
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
	deleteFile(STORAGE_FILE_USER);
	LOGGER->logDebug("Storage::createLogins: File " + storageInstance->getFileName(STORAGE_FILE_USER) + " was deleted");

	password = readLineFromFile(STORAGE_FILE_PASSWORD);
	while (!password.empty()) {
		LOGGER->logDebug("Storage::createLogins: password " + password);
		passwords.push_back(password);
		password = readLineFromFile(STORAGE_FILE_PASSWORD);
	}
	deleteFile(STORAGE_FILE_PASSWORD);
	LOGGER->logDebug("Storage::createLogins: File " + storageInstance->getFileName(STORAGE_FILE_PASSWORD) + " was deleted");

	if (!users.empty() && !passwords.empty()) {
		for (auto itUsers = users.begin(); itUsers != users.end(); itUsers++) {
			user = *itUsers;
			for (auto itPasswords = passwords.begin(); itPasswords != passwords.end(); itPasswords++) {
				password = *itPasswords;
				LOGGER->logDebug("Storage::getlogins: Creating Login: " + user + ", " + password);
				logins[id] = new Login(id, user, password);
				id++;
			}
		}
	} else {
		LOGGER->logDebug("Storage::createLogins: Can not get Users or Passwords");
		DISPATCHER->stopBruteDevices();
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
		if (file == STORAGE_FILE_KNOWN_IP) {
			isFileKnownLoginsExisting = true;
		}
	}
	else {
		if (file != STORAGE_FILE_KNOWN_IP && file != STORAGE_FILE_IP_QUEUE_IN) {
			LOGGER->logError("Storage::openFile: Can not open required file " + getFileName(file));
			return false;
		}
	}
	return true;
}

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
		if (!openFile(i) && i != STORAGE_FILE_KNOWN_IP && i != STORAGE_FILE_IP_QUEUE_IN) {
			DISPATCHER->stopBruteDevices();
			break;
		}
	}
}

std::list<std::string> Storage::getIp() {
	std::string line;
	std::list<std::string> result;
	if (storage == STORAGE_FILE) {
		if (files[STORAGE_FILE_IP_QUEUE_IN]->getFileSize(getFileName(STORAGE_FILE_IP_QUEUE_IN)) != 0) {
			line = readLineFromFile(STORAGE_FILE_IP_QUEUE_IN);
			if (!line.empty()) {
				result = TOOLS->split(line, IP_QUEUE_SPLITTER);
			}
		} else {
			line = readLineFromFile(STORAGE_FILE_IP);
			if (!line.empty()) {
				result.push_back(line);
			}
		}
	}
	if (result.size() > 0) {
		LOGGER->logDebug("Storage::getIp: IP = " + result.front());
		if (result.size() > 1) {
			LOGGER->logDebug("Storage::getIp: Login = " + result.back());
		}
	} else {
		LOGGER->logDebug("Storage::getIp: Can not get IP and Login ");
	}
	return result;
}

Login* Storage::getlogin(unsigned int id) {
	LOGGER->logDebug("Storage::getlogin: id = " + std::to_string(id));
	if (storage == STORAGE_FILE) {
		if (logins.empty()) {
			createLogins();
			LOGGER->logDebug("Storage::getlogin: Logins were created ");
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

void Storage::setKnownlogin(unsigned long ip, Login* login) {
	if (storage == STORAGE_FILE) {
		files[STORAGE_FILE_KNOWN_IP]->writeLine("i:" + TOOLS->getStringIpFromNumeric(ip) + "u:" + login->getUser() + "p:" + login->getPassword());
	}
}
