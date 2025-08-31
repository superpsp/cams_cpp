#pragma once
#include <string>

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
	bool run();
protected:
	Dispatcher() {}
	friend class DispatcherDestructor;
private:
	const unsigned int NUMBER_OF_DEVICES = 10; // TODO: move to parameters
	unsigned int numberOfDevices = 0;
	inline static std::unique_ptr<Dispatcher> dispatcherInstance{ nullptr };
};