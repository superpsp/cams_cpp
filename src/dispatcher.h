#pragma once
#include <string>
#include <memory>
#include <list>
#include "login.h"
#include "device_proto.h"

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
		setNumberOfDevices(unsigned long number);
		//, deviceInQueue(DeviceProto* device);
	bool run();
protected:
	Dispatcher() {}
	friend class DispatcherDestructor;
private:
	const unsigned int NUMBER_OF_DEVICES = 200;
	unsigned int
		numberOfDevices = NUMBER_OF_DEVICES;
	inline static std::unique_ptr<Dispatcher> dispatcherInstance{ nullptr };
	//std::list<Login*> logins;
	//std::list<DeviceProto*> devices;

	//void
	//	createLogins()
	//	, deleteLogins();
};