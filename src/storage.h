#pragma once
#include <memory>
#include <map>
#include <string>
#include <list>
#include "text_file.h"
#include "login.h"

class Storage;

class StorageDestructor {
public:
	~StorageDestructor();
	void initialize(Storage* p);
private:
	Storage* storageInstance;
};

class Storage {
public:
	static const unsigned char
		STORAGE_FILE = 0
		, STORAGE_DB = 1;

	Storage(Storage const&) = delete;
	Storage& operator = (Storage const&) = delete;
	~Storage() {}
	static Storage* getInstance();

	void setStorage(unsigned char storage);
	std::list<std::string> getIp();
	Login* getKnownlogin(unsigned long ip);
	Login* getlogin(unsigned int id);
	const std::string IP_QUEUE_SPLITTER = ":";
protected:
	Storage() {}
	friend class StorageDestructor;
private:
	inline static std::unique_ptr<Storage> storageInstance{ nullptr };

	static const unsigned char
		STORAGE_FILE_IP = 0
		, STORAGE_FILE_USER = 1
		, STORAGE_FILE_PASSWORD = 2
		, STORAGE_FILE_KNOWN_IP = 3
		, STORAGE_FILE_IP_QUEUE_IN = 4
		, STORAGE_FILE_IP_QUEUE_OUT = 5
		, STORAGE_FILE_NUMBER = 6;
	const std::string
		STORAGE_IP_FILE_NAME = "ips.txt"
		, STORAGE_USER_FILE_NAME = "users.txt"
		, STORAGE_PASSWORD_FILE_NAME = "passwords.txt"
		, STORAGE_KNOWN_IP_FILE_NAME = "known_ips.txt"
		, STORAGE_IP_QUEUE_IN_FILE_NAME = "ip_queue_in.txt"
		, STORAGE_IP_QUEUE_OUT_FILE_NAME = "ip_queue_out.txt";
	unsigned char
		storage = STORAGE_FILE;
	FileText* files[STORAGE_FILE_NUMBER];
	unsigned char modes[STORAGE_FILE_NUMBER];

	void
		initialize()
		, deleteFile(unsigned char file)
		, openFiles()
		, createLogins()
		, createKnownLogins();
	std::string
		getFileName(unsigned char storageFile)
		, readLineFromFile(unsigned char storageFile);
	bool openFile(unsigned char file);
	std::map<unsigned int, Login*> logins;
	std::map<unsigned long, Login*> knownLogins;
	bool
		isFileKnownLoginsExisting = false
		, isFileIpQueueExisting = false;
};

