#pragma once
#include <string>
#include <memory>
#include <list>
#include "login.h"

class Dispatcher;

class DispatcherDestructor {
public:
	~DispatcherDestructor();
	void initialize(Dispatcher* p);
private:
	Dispatcher* dispatcherInstance;
};

class Dispatcher {
public:
	Dispatcher(Dispatcher const&) = delete;
	Dispatcher& operator = (Dispatcher const&) = delete;
	~Dispatcher() {}
	static Dispatcher* getInstance();
	void setNumberOfDevices(unsigned long number);
	bool run();
protected:
	Dispatcher() {}
	friend class DispatcherDestructor;
private:
	const unsigned int NUMBER_OF_DEVICES = 200;
	unsigned int
		numberOfDevices = NUMBER_OF_DEVICES
		, numberOfLogins;
	inline static std::unique_ptr<Dispatcher> dispatcherInstance{ nullptr };
	std::list<Login*> logins;

	void
		deviceInQueue(unsigned int ip, unsigned int login)
		, createLogins();
};