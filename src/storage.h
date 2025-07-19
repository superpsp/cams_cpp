#pragma once
#include <memory>
#include <map>
#include <string>

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
		, STORAGE_FILE_NUMBER = 4;
	const std::string
		STORAGE_IP_FILE_NAME = "ips.txt"
		, STORAGE_USER_FILE_NAME = "users.txt"
		, STORAGE_PASSWORD_FILE_NAME = "passwords.txt"
		, STORAGE_KNOWN_IP_FILE_NAME = "known_ips.txt";
	unsigned char
		storage = STORAGE_FILE;
	FileText* files[STORAGE_FILE_NUMBER];
	unsigned char modes[STORAGE_FILE_NUMBER];
	bool isDeleted = false;

	void
		initialize()
		, openFile(unsigned char file)
		, deleteFile(unsigned char file)
		, openFiles()
		, switchMode(unsigned char file, unsigned char newMode);
	std::string
		getFileName(unsigned char storageFile)
		, getNextIp()
		, getNextUser()
		, getNextPassword()
		, getNextKnownIp();
};

