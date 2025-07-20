#include "storage.h"
#include "logger.h"
#include "text_file.h"

#define LOGGER Logger::getInstance()

std::mutex storageMutex;

StorageDestructor::~StorageDestructor() {
	std::lock_guard<std::mutex> lock(storageMutex);
	for (unsigned char i = storageInstance->STORAGE_FILE_IP; i < storageInstance->STORAGE_FILE_NUMBER; i++) {
		storageInstance->deleteFile(i);
		LOGGER->logDebug("StorageDestructor::~StorageDestructor: File " + storageInstance->getFileName(i) + " was deleted");
	}
	storageInstance->isDeleted = true;
	delete storageInstance;
	LOGGER->logDebug("StorageDestructor::~StorageDestructor: Instance deleted");
}

void StorageDestructor::initialize(Storage* p) {
	LOGGER->logDebug("StorageDestructor::~initialize: initialized");
	storageInstance = p;
}

Storage* Storage::getInstance() {
	std::lock_guard<std::mutex> lock(storageMutex);
	if (!storageInstance->isDeleted) {
		if (!storageInstance) {
			storageInstance = std::unique_ptr<Storage>(new Storage());
			LOGGER->logDebug("Storage::getInstance: Instance created");
			storageInstance.get()->initialize();
		}
		return storageInstance.get();
	} else {
		return nullptr;
	}
}

void Storage::setStorage(unsigned char storage) { // TODO: implement change storage to DB
	this->storage = storage;
}

void Storage::initialize() {
	LOGGER->logDebug("Storage::initialize: Start");
	if (storage == STORAGE_FILE) {
		openFiles();
	}
	LOGGER->logDebug("Storage::initialize: End");
}

void Storage::deleteFile(unsigned char file) {
	delete files[file];
}

void Storage::openFile(unsigned char file) {
	unsigned char result = files[file]->open();
	files[file] = new FileText(getFileName(file), modes[file]);
	if (result != File::FILE_OK) {
		LOGGER->logDebug("Storage::openFile: File " + getFileName(file) + " was opened");
	}
	else {
		LOGGER->logError("Storage::openFile: Can not open file " + getFileName(file)); // TODO: process error
	}
}

void Storage::switchMode(unsigned char file, unsigned char newMode) { // TODO: Use lock in calling method
	LOGGER->logDebug("Storage::switchMode: File " + std::to_string(file) + ", mode " + std::to_string(newMode));
	if (modes[file] != newMode) {
		deleteFile(file);
		openFile(file);
		LOGGER->logDebug("Storage::switchMode: Mode was switched");
	}
}

std::string Storage::getFileName(unsigned char storageFile) {
	switch (storageFile) {
		case STORAGE_FILE_IP:
			return STORAGE_IP_FILE_NAME;
		case STORAGE_FILE_USER:
			return STORAGE_USER_FILE_NAME;
		case STORAGE_FILE_PASSWORD:
			return STORAGE_PASSWORD_FILE_NAME;
		case STORAGE_FILE_KNOWN_IP:
			return STORAGE_KNOWN_IP_FILE_NAME;
		default:
			break;
	}
	return std::string();
}

void Storage::openFiles() {
	for (unsigned char i = STORAGE_FILE_IP; i < STORAGE_FILE_NUMBER; i++) {
		LOGGER->logDebug("Storage::openFiles: File " + std::to_string(i));
		modes[i] = File::FILE_IO_IN;
		openFile(i);
	}
}

std::string Storage::getNextIp() {
	return files[STORAGE_FILE_IP]->readLine();
}

std::string Storage::getNextUser() {
	return files[STORAGE_FILE_USER]->readLine();
}

std::string Storage::getNextPassword() {
	return files[STORAGE_FILE_PASSWORD]->readLine();
}

std::string Storage::getNextKnownIp() {
	return files[STORAGE_FILE_KNOWN_IP]->readLine();
}

void Storage::setNextKnownIp(std::string line) {
	LOGGER->logDebug("Storage::setNextKnownIp: line = " + line);
	files[STORAGE_FILE_KNOWN_IP]->writeLine(line);
}
