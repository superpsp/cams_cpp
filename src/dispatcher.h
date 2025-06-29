#pragma once
#include <string>
#include "source.h"

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
	static Dispatcher& getInstance();
	void setNumberOfDevices(unsigned long number);
	bool run();
protected:
	Dispatcher& operator = (Dispatcher&);

	Dispatcher() {}
	Dispatcher(const Dispatcher&);
	~Dispatcher() {}
	friend class DispatcherDestructor;
private:
	const unsigned int NUMBER_OF_DEVICES = 200;
	unsigned int numberOfDevices;
	Source* source = 0;
	void
		setDefaultParameters()
		, registerDevice();
};