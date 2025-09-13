#pragma once
#include <string>
#include <list>
#include <memory>
#include "device.h"

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
	void
		setNumberOfDevices(unsigned long number)
		, bruteDevices()
		, stopBruteDevices()
		, registerDevice(Device* device)
		, deleteDevice(Device* device);
protected:
	Dispatcher() {}
	friend class DispatcherDestructor;
private:
	const unsigned int NUMBER_OF_DEVICES = 5;
	unsigned char mode;
	unsigned int numberOfDevices = NUMBER_OF_DEVICES; // TODO: move to parameters
	inline static std::unique_ptr<Dispatcher> dispatcherInstance{ nullptr };
	std::list<Device*> devices;
	bool toContinueBrute = true;
	void createDevice(unsigned char mode);
};