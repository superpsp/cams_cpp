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
	static Dispatcher& getInstance();
	void setNumberOfDevices(unsigned long number);
	bool run();
protected:
	Dispatcher& operator = (Dispatcher&);

	Dispatcher() {}
	Dispatcher(const Dispatcher&);
	~Dispatcher(){}
	friend class DispatcherDestructor;
private:
	std::string ipFileName;
	unsigned int numberOfDevices;
	const std::string IP_FILE_NAME = "ips.txt";
	const unsigned int NUMBER_OF_DEVICES = 200;
	bool openIpFile();
	void
		setDefaultParameters()
		, registerDevice();
};